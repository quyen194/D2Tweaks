
#include <Windows.h>

#include <spdlog/spdlog.h>

#include "common/file_ini.h"

#include "diablo2/structures/data/items_line.h"
#include "diablo2/structures/player_data.h"
#include "diablo2/structures/room.h"
#include "diablo2/structures/unit.h"

#include "diablo2/d2common.h"
#include "diablo2/d2game.h"

#include "d2tweaks/common/asset_manager.h"

#include "d2tweaks/server/server.h"
#include "d2tweaks/server/auto_gold_pickup/auto_gold_pickup_server.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace server {

MODULE_INIT(AutoGoldPickup)

void AutoGoldPickup::init() {
  FileIni config(common::get_config_path());

  if (config.Int("modules", "AutoGoldPickup", 1)) {
    Server::instance().register_packet_handler(MessageType::kGoldPickupInfo,
                                               this);
    // Server::instance().register_tick_handler(this);
  }
}

bool AutoGoldPickup::handle_packet(Game* game,
                                   Unit* player,
                                   packet_header* packet) {
  const auto income_packet_cs =
      static_cast<gold_pickup_info_cs*>(packet);
  static auto& instance = Server::instance();
  const auto item =
      instance.get_server_unit(game,
                               income_packet_cs->item_guid,
                               unit_type_t::UNIT_TYPE_ITEM);  // 0x4 = item

  if (item == nullptr)
    return true; //block further packet processing

  AutoGoldPickup::au_pickup_gold(game, player, item);

  return true;
}

bool AutoGoldPickup::au_pickup_gold(Game* game, Unit* pUnit, Unit* item) {
  static gold_pickup_info_sc packet;

  const auto currentGold = d2_common::get_stat(pUnit, UNIT_STAT_GOLD, 0);
  const auto goldToPickup = d2_common::get_stat(item, UNIT_STAT_GOLD, 0);
  const auto maxGold = d2_common::get_maximum_character_gold(pUnit);

  if (static_cast<unsigned int>(currentGold + goldToPickup) > maxGold)
    return false;

  d2_game::pickup_gold_pile(game, pUnit, item);
  packet.gold = goldToPickup;

  Server::instance().send_packet(
      pUnit->player_data->net_client, &packet, sizeof packet);
  return true;
}

void AutoGoldPickup::tick(Game* game, Unit* unit) {
  // static gold_pickup_info_sc packet;
  // static auto& instance = Server::instance();
  // if (!game || !unit)
  //   return;
  //
  // if (unit->type != unit_type_t::UNIT_TYPE_PLAYER)
  //   return;
  //
  // const auto room = d2_common::get_room_from_unit(unit);
  //
  // if (!room)
  //   return;
  //
  // for (auto item = room->unit; item; item = item->prev_unit_in_room) {
  //   if (!item)
  //     continue;
  //
  //   if (item->type != unit_type_t::UNIT_TYPE_ITEM)
  //     continue;
  //
  //   auto record = d2_common::get_item_record(item->data_record_index);
  //
  //   if (!record)
  //     continue;
  //
  //   const auto distance = d2_common::get_distance_between_units(unit, item);
  //   if (distance > g_distance)
  //     continue;
  //
  //   // cannot pick up more than one item per tick, otherwise the item will disappear
  //   if (record->string_code[0] == 'g' &&
  //     record->string_code[1] == 'l' &&
  //     record->string_code[2] == 'd') {
  //     auto_gold_pickup::au_pickup_gold(game, unit, item);
  //     break;
  //   }
  //
  //   continue;
  // }
}

}  // namespace server
}  // namespace d2_tweaks
