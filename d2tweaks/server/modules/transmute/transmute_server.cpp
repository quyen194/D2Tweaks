
#include <Windows.h>

#include <spdlog/spdlog.h>

#include "common/autopickup_lootfilter.h"
#include "common/file_ini.h"

#include "diablo2/structures/data/item_types_line.h"
#include "diablo2/structures/data/items_line.h"
#include "diablo2/structures/game.h"
#include "diablo2/structures/inventory.h"
#include "diablo2/structures/item_data.h"
#include "diablo2/structures/net_client.h"
#include "diablo2/structures/path.h"
#include "diablo2/structures/player_data.h"
#include "diablo2/structures/room.h"
#include "diablo2/structures/unit.h"

#include "diablo2/d2common.h"
#include "diablo2/d2game.h"
#include "diablo2/d2lang.h"
#include "diablo2/d2net.h"

#include "d2tweaks/common/asset_manager.h"
#include "d2tweaks/common/protocol.h"

#include "d2tweaks/server/server.h"
#include "d2tweaks/server/modules/transmute/transmute_server.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace server {
namespace modules {

MODULE_INIT(transmute)

enum transmute_command {
  COMMAND_NULL,
  COMMAND_TRANSMUTE_START,
  COMMAND_TRANSMUTE_END,
  COMMAND_CALL_TRANSMUTE,
  COMMAND_MOVE_ITEM,
  COMMAND_ABORT
};

void transmute::init() {
  FileIni config(common::get_config_path());

  if (config.Int("modules", "AutoTransmute", 0)) {
    // singleton<server>::instance().register_tick_handler(this);
    singleton<server>::instance().register_packet_handler(common::MESSAGE_TYPE_TRANSMUTE, this);
  }
}

// transmute function d2game
// topfunc d2game:$62130 int __fastcall CRAFT_Transmogrify(D2GameStrc* pGame, D2UnitStrc* pPlayer)
// subfunc d2game:$0x60010 int __fastcall CRAFT_Transmogrify(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2CubemainTXT* pCubeTxt, void* pUnknown)

void transmute::tick(Game* game, Unit* unit) {
  return;
}

inline uint64_t TimeStart() {
  _asm {
    cpuid
    rdtsc
  }
}

inline uint64_t TimeEnd() {
  _asm {
    rdtsc
  }
}

bool transmute::handle_packet(Game* game,
                              Unit* player,
                              common::packet_header* packet) {
  const auto income_packet_cs = static_cast<common::transmute_info_cs*>(packet);
  static common::transmute_info_sc response_packet_sc;

  // process request from client
  if (income_packet_cs->command == COMMAND_CALL_TRANSMUTE) {
#ifndef NDEBUG
    uint64_t time = TimeStart();
#endif
    // returns the generated item, or getmaxcuberecipes if unsuccessful
    if (d2_game::transmogrify(game, player) == d2_common::get_max_cube_recipes()) {
      response_packet_sc.command = COMMAND_ABORT;
      singleton<server>::instance().send_packet(player->player_data->net_client,
                                                &response_packet_sc,
                                                sizeof response_packet_sc);
      // d2_net::send_to_client(
      //     1, client->client_id, &response_packet_sc, sizeof response_packet_sc);
    }
#ifndef NDEBUG
    time = (TimeEnd() - time);
    spdlog::debug("Time {}", time);
#endif
  }

  if (income_packet_cs->command == COMMAND_MOVE_ITEM) {
    transmute::move_item_to(game, player, packet);
  }

  return true;
}

bool transmute::move_item_to(Game* game,
                             Unit* player,
                             common::packet_header* packet) {
  static common::transmute_info_sc resp;
  static auto& instance = singleton<server>::instance();
  const auto itemMove = static_cast<common::transmute_info_cs*>(packet);
  const auto item = instance.get_server_unit(
      game, itemMove->item_guid, unit_type_t::UNIT_TYPE_ITEM);  // 0x4 = item

  if (item == nullptr)
    return true; //block further packet processing

  const auto inventoryIndex = d2_common::get_inventory_index(
      player, itemMove->target_page, game->item_format == 101);

  uint32_t tx, ty;

  if (!find_free_space(player->inventory,
                       item,
                       inventoryIndex,
                       itemMove->target_page,
                       tx,
                       ty))
    return true; //block further packet processing

  item->item_data->page = itemMove->target_page;

  d2_common::inv_add_item(player->inventory,
                          item,
                          tx,
                          ty,
                          inventoryIndex,
                          false,
                          item->item_data->page);
  d2_common::inv_update_item(player->inventory, item, false);
  d2_game::update_inventory_items(game, player);

  //send update packet
  resp.command = COMMAND_MOVE_ITEM;
  resp.item_guid = itemMove->item_guid;
  resp.target_page = itemMove->target_page;
  resp.tx = tx;
  resp.ty = ty;

  const auto client = player->player_data->net_client;
  //d2_net::send_to_client(1, client->client_id, &resp, sizeof resp);
  singleton<server>::instance().send_packet(
      player->player_data->net_client, &resp, sizeof resp);
  return true;
}

bool transmute::find_free_space(Inventory* inv,
                                Unit* item,
                                int32_t inventoryIndex,
                                char page,
                                uint32_t& x,
                                uint32_t& y) {
  char data[0x18];

  d2_common::get_inventory_data(inventoryIndex, 0, data);

  const auto mx = static_cast<uint32_t>(data[0]);
  const auto my = static_cast<uint32_t>(data[1]);

  for (x = 0; x < mx; x++) {
    for (y = 0; y < my; y++) {
      Unit* blockingUnit = nullptr;
      uint32_t blockingUnitIndex = 0;

      if (d2_common::can_put_into_slot(inv,
                                       item,
                                       x,
                                       y,
                                       inventoryIndex,
                                       &blockingUnit,
                                       &blockingUnitIndex,
                                       page))
        return true;
    }
  }

  return false;
}

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
