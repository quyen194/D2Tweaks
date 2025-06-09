#include <d2tweaks/client/client.h>

#include <common/hooking.h>
#include <common/strings.h>

#include <spdlog/spdlog.h>

#include <d2tweaks/common/protocol.h>
#include <d2tweaks/client/modules/client_module.h>
#include <d2tweaks/ui/ui_manager.h>

#include <diablo2/structures/unit.h>
#include <diablo2/structures/client_unit_list.h>
#include <WinBase.h>
//debug junk
//#include <iostream>
#include <string>
#include <Windows.h>

#include <sstream>
#include <vector>
#include <map>
#include <codecvt>
#include <regex>
#include <regex>

#include <d2tweaks/common/asset_manager.h>
#include <d2tweaks/common/common.h>

#include <diablo2/d2common.h>
#include <diablo2/d2client.h>
#include <diablo2/d2cmp.h>
#include <diablo2/d2win.h>
#include <diablo2/d2gfx.h>
#include <diablo2/d2game.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/player_data.h>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

#define STB_IMAGE_IMPLEMENTATION

GfxData g_gfxdata; // global gfxdata

// Define a struct to hold key-value pairs within a section
struct Section {
  std::map<std::string, std::string> assignments;
};

#include <D2Template.h>

static void(__fastcall* g_handle_packet)(common::packet_header* packet, size_t size);
static void(__fastcall* g_handle_packet_standart)(common::packet_header* packet, size_t size);
static void(__fastcall* g_handle_cs_packet)(common::packet_header* packet, size_t size);
static int32_t(__stdcall* g_draw_game_ui_original)();
static int32_t(__fastcall* g_game_tick_original)(int32_t a1);
//void(__fastcall* g_game_loop_start)();

//static uint32_t g_ret;

namespace d2_tweaks {
namespace client {

client::client(token) {
  m_module_id_counter = 0;
  m_tick_handler_id_counter = 0;
}

__declspec(naked) void __stdcall game_tick_wrapper() {
  // usercall, uses ecx = param1, esi = param2
  __asm {
    pushad
    pushfd
    call[client::game_tick]
    popfd
    popad

    push[g_game_tick_original]
    ret
  }
}

__declspec (naked) void handle_cs_packet_wrapper() {
  __asm {
    pushad;
    pushfd;
    call[client::handle_cs_packet]
      popfd;
    popad;
    // original instructions
    sub esp, 0x200;
  }
  RET_TO_RVA(DLLBASE_D2CLIENT, 0xD856);
}

__declspec (naked) void handle_sc_standart_packet_wrapper() {
  __asm {
    pushad;
    pushfd;
    call[client::handle_standart_packet]
      popfd;
    popad;
    // original instructions
    sub esp, 0x54;
    push ebx;
    push ebp;
  }
  RET_TO_RVA(DLLBASE_D2CLIENT, 0x150B5);
}

static const DLLPatchStrc gpt_handle_cs_packet[] = {
  {D2DLL_D2CLIENT, 0xD850 + 0, PATCH_JMP, FALSE, 0x1},
  {D2DLL_D2CLIENT, 0xD850 + 1, (DWORD)handle_cs_packet_wrapper, TRUE, 0x0},
  {D2DLL_D2CLIENT, 0xD850 + 5, (DWORD)PATCH_NOPBLOCK, FALSE, 0x1},
  {D2DLL_INVALID}
};

static const DLLPatchStrc gpt_handle_sc_standart_packet[] = {
  {D2DLL_D2CLIENT, 0x150B0 + 0, PATCH_JMP, FALSE, 0x1},
  {D2DLL_D2CLIENT, 0x150B0 + 1, (DWORD)handle_sc_standart_packet_wrapper, TRUE, 0x0},
  {D2DLL_INVALID}
};

void client::init() {
  // handle packet processes the packet before GamePacketReceivedIntercept
  detour::hook(d2_client::get_base() + 0x11CB0, handle_packet, reinterpret_cast<void**>(&g_handle_packet));
  detour::hook(d2_client::get_base() + 0x9640, game_tick_wrapper, reinterpret_cast<void**>(&g_game_tick_original));
  detour::hook(d2_client::get_base() + 0x5E650, draw_game_ui, reinterpret_cast<void**>(&g_draw_game_ui_original));
  //detour::hook(d2_client::get_base() + 0x150B0, handle_standart_packet, reinterpret_cast<void**>(&g_handle_packet_standart));

  D2TEMPLATE_ApplyPatch(gpt_handle_cs_packet);
  //D2TEMPLATE_ApplyPatch(gpt_handle_sc_standart_packet);

  for (auto& m_module : m_modules) {
    if (m_module == nullptr)
      break;

    m_module->init_early();
  }
}

static int32_t g_ebp_send_to_client;
void client::handle_cs_packet(common::packet_header* packet, size_t size) {
#ifndef NDEBUG
  __asm {
    push[ebp + 0x2C];
    pop[g_ebp_send_to_client];
  }
  spdlog::warn("[d2client SEND] Packet {:02X} Message {:02X} Size {} CallFrom {} Dump {}",
               packet->d2_packet_type,
               packet->message_type,
               size,
               (void*) g_ebp_send_to_client,
               utils::BytesToHexString(utils::ToBytes(packet, size)));
#endif

  static common::packet_header dummy;
  static auto& instance = singleton<client>::instance();

  if (size == -1)
    return;

  auto handler = instance.m_packet_cs_handlers[packet->d2_packet_type];

  if (!handler)
    return;

  handler->handle_cs_packet(packet);
}

void client::handle_standart_packet(common::packet_header* packet,
                                    size_t size) {
  if (size == -1)
    return;

#ifndef NDEBUG
  spdlog::info("[d2client RECV] Packet {} Message {} Size {}",
               (void*) packet->d2_packet_type,
               (void*) packet->message_type,
               size);
#endif

  return;
}

void client::handle_packet(common::packet_header* packet, size_t size) {
  static common::packet_header dummy;
  static auto& instance = singleton<client>::instance();

  if (size == -1)
    return;

#ifndef NDEBUG
  spdlog::info("[d2client RECV] Packet {} Message {} Size {}",
               (void*) packet->d2_packet_type,
               (void*) packet->message_type,
               size);
#endif

  if (packet->d2_packet_type != dummy.d2_packet_type) {
    g_handle_packet(packet, size);
    return;
  }

  auto handler = instance.m_packet_handlers[packet->message_type];

  if (!handler)
    return;

  handler->handle_packet(packet);
}

static bool g_is_init = false;
void client::game_tick() {
  static auto& instance = singleton<client>::instance();  /// conflict with text on d2 gl

  if (g_is_init == false) {
    D2TEMPLATE_Init();

    for (auto& m_module : instance.m_modules) {
      if (m_module == nullptr)
        break;

      m_module->init();
    }
    g_is_init = true;
  }

  for (auto& tick_handler : instance.m_tick_handlers) {
    if (tick_handler == nullptr)
      break;

    tick_handler->tick();
  }

  return;
}

int32_t client::draw_game_ui() {
  static auto& ui = singleton<ui::ui_manager>::instance();

  const auto result = g_draw_game_ui_original();

  ui.draw();

  return result;
}

void client::register_module(client_module* module) {
  m_modules[m_module_id_counter++] = module;
}

void client::register_tick_handler(client_module* module) {
  m_tick_handlers[m_tick_handler_id_counter++] = module;
}

void client::register_packet_cs_handler(common::packet_types_cs_t packet,
                                        common::message_types_t type,
                                        client_module* module) {
  if (m_packet_cs_handlers[packet] != nullptr) {
    spdlog::warn("Clientside packet cs handler for {0} is already registered!",
                 type);
  }

  m_packet_cs_handlers[packet] = module;
}

void client::register_packet_handler(common::message_types_t type,
                                     client_module* module) {
  if (m_packet_handlers[type] != nullptr) {
    spdlog::warn("Clientside packet handler for {0} is already registered!",
                 type);
  }

  m_packet_handlers[type] = module;
}

Unit* client::get_client_unit(uint32_t type, uint32_t guid) {
  static auto units = d2_client::get_client_unit_list();

  const auto index = guid & 127;

  auto result = units->unit_list[type][index];

  while (result != nullptr && result->guid != guid) {
    result = result->prev_unit;
  }

  return result;
}

}  // namespace client
}  // namespace d2_tweaks
