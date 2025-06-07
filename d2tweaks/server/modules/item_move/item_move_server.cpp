#include <d2tweaks/common/asset_manager.h>

#include <d2tweaks/server/modules/item_move/item_move_server.h>
#include <d2tweaks/server/server.h>

#include <diablo2/d2net.h>
#include <diablo2/d2game.h>
#include <diablo2/d2common.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/net_client.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/player_data.h>
#include <diablo2/structures/path.h>

#include <spdlog/spdlog.h>
#include <common/file_ini.h>

#include <fstream>
#include <iostream>
#include <diablo2/d2client.h>

#include <iomanip> // For std::setw
#include <sstream>
#include <string>
#include <stdexcept> // For std::invalid_argument

#include <windows.h> // Include Windows API header for MessageBox

#include <iomanip> // For std::setw

static uint32_t reverseDWORD(uint32_t dw) {
  return ((dw << 24) & 0xFF000000) | ((dw << 8) & 0x00FF0000) |
         ((dw >> 8) & 0x0000FF00) | ((dw >> 24) & 0x000000FF);
}

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace server {
namespace modules {

MODULE_INIT(item_move)

void item_move::init() {
  FileIni config(common::get_config_path());

  if (config.Int("modules", "ItemMover", 1)) {
    singleton<server>::instance().register_packet_handler(
        common::MESSAGE_TYPE_ITEM_MOVE, this);
  }
}

// handle packet coming from the client
bool item_move::handle_packet(Game* game,
  Unit* player, common::packet_header* packet) {
  static common::item_move_sc resp;
  static auto& instance = singleton<server>::instance();

  const auto itemMove = static_cast<common::item_move_cs*>(packet);
  const auto key = static_cast<common::item_move_cs*>(packet)->item_code;

  // Display key in a message box
  // MessageBox(NULL, key, "Item code", MB_OK | MB_ICONINFORMATION);

  const auto item = instance.get_server_unit(
      game, itemMove->item_guid, unit_type_t::UNIT_TYPE_ITEM);  // 0x4 = item

  // Get player room
  Room* room = d2_common::get_room_from_unit(player);


  const auto record = d2_common::get_item_record(item->data_record_index);

  const char* itemcode = itemMove->item_code;

  // display itemMove->bag_guid in a messagebox
  std::string bag_guid = std::to_string(itemMove->bag_guid);
  //MessageBox(NULL, bag_guid.c_str(), "Bag GUID", MB_OK | MB_ICONINFORMATION);

  const auto bag = instance.get_server_unit(
      game, itemMove->bag_guid, unit_type_t::UNIT_TYPE_ITEM);  // 0x4 = item

  // display bag guid in a messagebox
  bag_guid = std::to_string(itemMove->bag_guid);
  //MessageBox(NULL, bag_guid.c_str(), "Bag GUID", MB_OK | MB_ICONINFORMATION);

  // display itemMove->iCode in a messagebox
  std::string iCode = std::to_string(itemMove->iCode);
  //MessageBox(NULL, iCode.c_str(), "iCode", MB_OK | MB_ICONINFORMATION);

  if (itemMove->extract == 1) {
    // display bag guid in a messagebox
    std::string bag_guid = std::to_string(itemMove->bag_guid);
    //MessageBox(NULL, bag_guid.c_str(), "Bag GUID", MB_OK | MB_ICONINFORMATION);

    D2PropertyStrc itemProperty = {};
    itemProperty.nProperty = itemMove->prop; // Adjust the property ID
    itemProperty.nLayer = 0;
    itemProperty.nMin = itemMove->val;
    itemProperty.nMax = itemMove->val;

    // Add the gem property to the gem bag
    d2_common::add_property(bag, &itemProperty, 0);

    d2_game::QUESTS_CreateItem(
        game, player, reverseDWORD(itemMove->iCode), 1, ITEMQUAL_NORMAL, true);
    return true;
  }
  else if (itemMove->tmog == 1) {
    const auto item = instance.get_server_unit(
        game, itemMove->item_guid, unit_type_t::UNIT_TYPE_ITEM);  // 0x4 = item
    d2_game::D2GAME_Transmogrify_6FC4A660(game, player, item);
  }
  else if (itemMove->updateBag == 1) {
    D2PropertyStrc itemProperty = {};
    itemProperty.nProperty = itemMove->prop;
    itemProperty.nLayer = 0;
    itemProperty.nMin = itemMove->val;
    itemProperty.nMax = itemMove->val;
    d2_common::add_property(bag, &itemProperty, 1);

    d2_common::inv_remove_item(player->inventory, item);
  }
  else {
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
    resp.item_guid = itemMove->item_guid;
    resp.target_page = itemMove->target_page;
    resp.tx = tx;
    resp.ty = ty;
    const auto client = player->player_data->net_client;
    d2_net::send_to_client(1, client->client_id, &resp, sizeof resp);
    return true;
  }

  return false;
}

bool item_move::find_free_space(Inventory* inv,
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
