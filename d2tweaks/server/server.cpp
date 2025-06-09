#include <d2tweaks/server/server.h>

#include <Windows.h>

#include <spdlog/spdlog.h>

#include <d2tweaks/common/protocol.h>

#include <diablo2/d2game.h>
#include <diablo2/d2net.h>

#include <diablo2/structures/game.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/net_client.h>

#include <d2tweaks/common/common.h>
#include <d2tweaks/server/modules/server_module.h>
#include <common/hooking.h>
#include <D2Template.h>
#include <filesystem>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

static int32_t(__fastcall* g_get_incoming_packet_info_original)(
    common::packet_header* data,
    unsigned int dataSize,
    size_t* packetSizeOut,
    size_t* someOffset,
    int* packetGroup,
    int32_t* a6,
    int a7,
    int a8);

static int32_t(__fastcall* g_handle_packet_original)(
    Game* game, Unit* player, common::packet_header* data, size_t size);
static int32_t(__fastcall* g_net_tick_original)(Game*, Unit*, int32_t, int32_t);

//returns some kind of processing type (i.e. resultGroup == 0x04 means drop packet)
static int32_t __fastcall get_incoming_packet_info(common::packet_header* data,
                                                   unsigned int dataSize,
                                                   size_t* packetSizeOut,
                                                   size_t* someOffset,
                                                   int* packetGroup,
                                                   int32_t* a6,
                                                   int a7,
                                                   int a8) {
  static common::packet_header dummy;
  static auto& instance = singleton<common::common>::instance();

  const auto resultGroup = g_get_incoming_packet_info_original(
      data, dataSize, packetSizeOut, someOffset, packetGroup, a6, a7, a8);

  if (data->d2_packet_type == dummy.d2_packet_type &&
      resultGroup == 0x04) {  // 0x04 - drop packet
    size_t size;

    if (!instance.get_packet_size_cs(data, size))
      return resultGroup;

    *packetSizeOut = size;
    *packetGroup = 1;
    *a6 = 100;
    return 2;
  }

  return resultGroup;
}

static int32_t __fastcall handle_packet(Game* game,
                                        Unit* player,
                                        common::packet_header* data,
                                        size_t size) {
  static common::packet_header dummy;
  static auto& instance = singleton<server::server>::instance();

  if (data->d2_packet_type == dummy.d2_packet_type) {
    if (!instance.handle_packet(game, player, data))
      return g_handle_packet_original(game, player, data, size);

    return 1;
  }

  return g_handle_packet_original(game, player, data, size);
}

namespace d2_tweaks {
namespace server {

server::server(token) {
  m_module_id_counter = 0;
  m_tick_handler_id_counter = 0;
}

static int32_t g_ebp_send_to_client;
void __fastcall hook_sc_packet_before_sent(uint32_t* client_strc,
                                           common::packet_header* packet,
                                           size_t size) {
#ifndef NDEBUG
  __asm {
    push[ebp + 0x30];
    pop[g_ebp_send_to_client];
  }

  if (size == -1)
    return;

  spdlog::error("[d2game SEND] Packet {} Message {} Size {} CallFrom {}",
                (void*) packet->d2_packet_type,
                (void*) packet->message_type,
                size,
                (void*) g_ebp_send_to_client);
#endif

  return;
}

__declspec (naked) void hook_sc_packet_before_sent_wrapper() {
  __asm {
    pushad;
    pushfd;
    push[esp + 0x28]
    call[hook_sc_packet_before_sent]
    popfd;
    popad;
    // original instructions
    push ecx
    push ebp
    push esi
    mov esi, ecx
  }
  RET_TO_RVA(DLLBASE_D2GAME, 0xC715);
}

// d2game:$0xC710
static const DLLPatchStrc gpt_hook_sc_packet_before_sent[] =
{
  {D2DLL_D2GAME, 0xC710 + 0, PATCH_JMP, FALSE, 0x1},
  {D2DLL_D2GAME, 0xC710 + 1, (DWORD)hook_sc_packet_before_sent_wrapper, TRUE, 0x0},
  {D2DLL_INVALID}
};

void server::init() {
  detour::hook(reinterpret_cast<void*>(d2_game::get_base() + 0x59320),
               ::handle_packet,
               &g_handle_packet_original);
  detour::hook(reinterpret_cast<void *>(d2_game::get_base() + 0x50F80),
               net_tick,
               &g_net_tick_original);
  detour::hook(reinterpret_cast<void *>(d2_net::get_base() + 0x1FE0),
               get_incoming_packet_info,
               &g_get_incoming_packet_info_original);

  // D2TEMPLATE_ApplyPatch(gpt_hook_sc_packet_before_sent);

  // disable outgoing packet type checks
  DWORD oldProtect;
  if (VirtualProtect(d2_net::get_base() + 0x18B2,
                     0x02,
                     PAGE_EXECUTE_READWRITE,
                     &oldProtect))
    *reinterpret_cast<uint16_t*>(d2_net::get_base() + 0x18B2) = 0xBC3C;

  if (VirtualProtect(d2_net::get_base() + 0x18D0,
                     0x02,
                     PAGE_EXECUTE_READWRITE,
                     &oldProtect))
    *reinterpret_cast<uint16_t*>(d2_net::get_base() + 0x18D0) = 0xBC3C;

  for (size_t i = 0; i < sizeof m_modules / sizeof(void*); i++) {
    if (m_modules[i] == nullptr)
      break;

    m_modules[i]->init();
  }
}

void server::send_packet(NetClient* client,
                         common::packet_header* packet,
                         size_t size) {
  d2_game::enqueue_packet(client, packet, size);
}

bool server::handle_packet(Game* game,
                           Unit *player,
                           common::packet_header *packet) {
  auto handler = m_packet_handlers[packet->message_type];

  if (!handler)
    return false;

  return handler->handle_packet(game, player, packet);
}

void server::register_module(modules::server_module* module) {
  m_modules[m_module_id_counter++] = module;
}

void server::register_tick_handler(modules::server_module* module) {
  m_tick_handlers[m_tick_handler_id_counter++] = module;
}

void server::register_packet_handler(common::message_types_t type,
                                     modules::server_module* module) {
  if (m_packet_handlers[type] != nullptr) {
    spdlog::warn("Serverside packet handler for {0} is already registered!",
                 type);
  }

  m_packet_handlers[type] = module;
}

Unit* server::get_server_unit(Game* game, uint32_t guid, unit_type_t type) {
  if (game == nullptr)
    return nullptr;

  auto typeIndex = static_cast<uint32_t>(type);

  if (type == unit_type_t::UNIT_TYPE_MISSILE)
    typeIndex = 4;

  if (type == unit_type_t::UNIT_TYPE_ITEM)
    typeIndex = 3;

  const auto index = guid & 127;

  auto result = game->unit_list[typeIndex][index];

  while (result != nullptr && result->guid != guid) {
    result = result->prev_unit;
  }

  return result;
}

void server::iterate_server_units(Game* game, unit_type_t type,
                                  const std::function<bool(Unit*)>& cb) {
  if (!cb)
    return;

  if (!game)
    return;

  auto typeIndex = static_cast<uint32_t>(type);

  if (type == unit_type_t::UNIT_TYPE_MISSILE)
    typeIndex = 4;

  if (type == unit_type_t::UNIT_TYPE_ITEM)
    typeIndex = 3;

  for (size_t index = 0; index < 128; index++) {
    auto unit = game->unit_list[typeIndex][index];

    while (unit != nullptr) {
      if (!cb(unit))
        return;

      unit = unit->prev_unit;
    }
  }
}

int32_t server::net_tick(Game* game, Unit* unit, int32_t a3, int32_t a4) {
  static auto& instance = singleton<server>::instance();

  for (size_t i = 0; i < sizeof instance.m_modules / sizeof(void*); i++) {
    if (instance.m_tick_handlers[i] == nullptr)
      break;

    instance.m_tick_handlers[i]->tick(game, unit);
  }

  return g_net_tick_original(game, unit, a3, a4);
}

}  // namespace server
}  // namespace d2_tweaks
