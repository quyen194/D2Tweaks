#include <d2tweaks/server/modules/autosort/autosort_server.h>

#include <common/file_ini.h>

#include <d2tweaks/server/server.h>

#include <vector>
#include <spdlog/spdlog.h>

#include <d2tweaks/common/asset_manager.h>
#include <d2tweaks/common/protocol.h>

#include <diablo2/d2common.h>
#include <diablo2/d2game.h>

#include <diablo2/structures/path.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>
#include <diablo2/structures/player_data.h>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace server {
namespace modules {

MODULE_INIT(autosort)

struct backup_item {
  Unit* item;
  uint8_t x;
  uint8_t y;

  backup_item() : item(nullptr), x(0), y(0) {}
};

struct packed_area {
  uint8_t x;
  uint8_t y;
  uint8_t w;
  uint8_t h;
};

// Define variables to store the inventory zone values
int iminValidX = 0;
int imaxValidX = 0;
int iminValidY = 0;
int imaxValidY = 0;

// Define variables to store the charm zone values
int cminValidX = 0;
int cmaxValidX = 0;
int cminValidY = 0;
int cmaxValidY = 0;

void autosort::init() {
  FileIni config(common::get_config_path());

  iminValidX = config.Int("InventoryZone", "MinValidX", 0);
  imaxValidX = config.Int("InventoryZone", "MaxValidX", 0);
  iminValidY = config.Int("InventoryZone", "MinValidY", 0);
  imaxValidY = config.Int("InventoryZone", "MaxValidY", 0);
 
  cminValidX = config.Int("CharmZone", "MinValidX", 0);
  cmaxValidX = config.Int("CharmZone", "MaxValidX", 0);
  cminValidY = config.Int("CharmZone", "MinValidY", 0);
  cmaxValidY = config.Int("CharmZone", "MaxValidY", 0);

  if (config.Int("modules", "Autosort", 1)) {
    singleton<server>::instance().register_packet_handler(
        common::MESSAGE_TYPE_INVENTORY_SORT, this);
  }
}

bool autosort::handle_packet(Game* game,
  Unit* player, common::packet_header* packet) {
  if (static_cast<common::inventory_sort_cs*>(packet)->remItem == 1) {
    d2_common::inv_remove_item(
        player->inventory,
        static_cast<common::inventory_sort_cs*>(packet)->item_to_remove);
    d2_game::update_inventory_items(game, player);

    static_cast<common::inventory_sort_cs*>(packet)->item_to_remove = nullptr;

    for (auto item = player->inventory->first_item; item != nullptr;
         item = item->item_data->pt_next_item) {
      if (item ==
          static_cast<common::inventory_sort_cs*>(packet)->item_to_remove) {
        d2_common::inv_remove_item(player->inventory, item);
        d2_game::update_inventory_items(game, player);
      }
    }

    MessageBoxA(NULL, "Item removed", "Item removed", MB_OK);
  }
  else {
    sort(game, player, static_cast<common::inventory_sort_cs*>(packet)->page);
  }
  return true;
}

bool autosort::sort(Game* game, Unit* player, uint8_t page) {
  static common::inventory_sort_sc packet;
  static auto& instance = singleton<server>::instance();

  if (player == nullptr)
    return false;

  const auto inventoryIndex =
      d2_common::get_inventory_index(player, page, game->item_format == 101);
  char data[0x18];
  d2_common::get_inventory_data(inventoryIndex, 0, data);

  const auto inventoryWidth = static_cast<uint32_t>(data[0]);
  const auto inventoryHeight = static_cast<uint32_t>(data[1]);

  std::vector<packed_area> areas{{0,
                                  0,
                                  static_cast<uint8_t>(inventoryWidth),
                                  static_cast<uint8_t>(inventoryHeight)}};
  std::vector<Unit*> items;
  std::vector<Unit*> charms;
  std::vector<backup_item> backup_items;
  std::unordered_map<uint32_t, std::vector<Unit*>> items_typed;
  std::unordered_map<uint32_t, std::vector<Unit*>> charms_typed;
  uint32_t occupied_cells = 0;

  for (auto item = player->inventory->first_item; item != nullptr;
       item = item->item_data->pt_next_item) {
    if (item->item_data->page != page)
      continue;

    const auto record = d2_common::get_item_record(item->data_record_index);
    const auto record_type = d2_common::get_item_type_record(record->type);
    bool is_charm = record_type->charm;

    backup_item backup;
    backup.item = item;
    backup.x = item->path->x;
    backup.y = item->path->y;

    backup_items.push_back(backup);

    // only check charmzone grid for inventory page 0
    // Check if item's coordinates are within the valid range
    if (is_charm) {
      // Coordinates are within the valid range, add the item to items vector
      charms_typed[item->data_record_index].push_back(item);
      charms.push_back(item);
      occupied_cells += record->inv_height * record->inv_width;
    } else {
      // Coordinates are within the valid range, add the item to items vector
      items_typed[item->data_record_index].push_back(item);
      items.push_back(item);
      occupied_cells += record->inv_height * record->inv_width;
    }

    // spdlog::info("--------------------------------");
    // spdlog::info("Item Type: {0}", record->type);
    // spdlog::info("Item Type 2: {0}", record->type2);
    // spdlog::info("Occupied Cells: {0}", occupied_cells);
    // spdlog::info("x: {0}", backup.x);
    // spdlog::info("y: {0}", backup.y);
    // spdlog::info("is_charm: {0}\n\n", is_charm);
  }

  if (occupied_cells > inventoryHeight * inventoryWidth) {
    //should never be happen in normal cases
    spdlog::warn("occupied_cells > inventoryHeight* inventoryWidth");
    return false;
  }

  // Remove all items from inventory
  for (auto item : items)
    d2_common::inv_remove_item(player->inventory, item);
  for (auto item : charms)
    d2_common::inv_remove_item(player->inventory, item);

  const auto itemsCount = items.size();
  const auto charmsCount = charms.size();

  if (itemsCount == 0)
    return true; //there's nothing to sort

  auto success = NULL;

  if (itemsCount > 0) {
    // sort items by height*width
    for (size_t i = 0; i < itemsCount - 1; i++) {
      auto swapped = false;
      for (size_t j = 0; j < itemsCount - i - 1; j++) {
        const auto record1 =
            d2_common::get_item_record(items[j]->data_record_index);
        const auto record2 =
            d2_common::get_item_record(items[j + 1]->data_record_index);

        if (record1->inv_height * record1->inv_width >
            record2->inv_height * record2->inv_width)
          continue;

        const auto temp = items[j];
        items[j] = items[j + 1];
        items[j + 1] = temp;
        swapped = true;
      }

      if (swapped == false)
        break;
    }
    success = TRUE;
    for (auto sorted_item : items) {
      // iterate through all items of the same type for grouping
      for (auto& item : items_typed[sorted_item->data_record_index]) {
        const auto record = d2_common::get_item_record(item->data_record_index);

        uint32_t tx, ty;

        if (!find_free_space(
                player->inventory, item, inventoryIndex, page, tx, ty, false)) {
          success = false;
          break;
        }

        if (!d2_common::inv_add_item(
                player->inventory, item, tx, ty, inventoryIndex, false, page)) {
          success = false;
          break;
        }

        if (d2_common::inv_update_item(player->inventory, item, false))
          continue;

        success = false;
        break;
      }

      if (!success)
        break;

      items_typed.erase(sorted_item->data_record_index);
    }
  }

  if (charmsCount > 0) {
    // sort charms by height*width
    for (size_t i = 0; i < charmsCount - 1; i++) {
      auto swapped = false;
      for (size_t j = 0; j < charmsCount - i - 1; j++) {
        const auto record1 =
            d2_common::get_item_record(charms[j]->data_record_index);
        const auto record2 =
            d2_common::get_item_record(charms[j + 1]->data_record_index);

        if (record1->inv_height * record1->inv_width >
            record2->inv_height * record2->inv_width)
          continue;

        const auto temp = charms[j];
        charms[j] = charms[j + 1];
        charms[j + 1] = temp;
        swapped = true;
      }

      if (swapped == false)
        break;
    }
    success = TRUE;
    for (auto sorted_item : charms) {
      // iterate through all items of the same type for grouping
      for (auto& item : charms_typed[sorted_item->data_record_index]) {
        const auto record = d2_common::get_item_record(item->data_record_index);

        uint32_t tx, ty;

        if (!find_free_space(
                player->inventory, item, inventoryIndex, page, tx, ty, true)) {
          success = false;
          break;
        }

        if (!d2_common::inv_add_item(
                player->inventory, item, tx, ty, inventoryIndex, false, page)) {
          success = false;
          break;
        }

        if (d2_common::inv_update_item(player->inventory, item, false))
          continue;

        success = false;
        break;
      }

      if (!success)
        break;

      charms_typed.erase(sorted_item->data_record_index);
    }
  }

  if (success) {
    while (items.size() > 0) {
      packet.page = page;
      packet.tx = items.back()->path->x;
      packet.ty = items.back()->path->y;
      packet.guid = items.back()->guid;

      items.pop_back();

      instance.send_packet(
          player->player_data->net_client, &packet, sizeof packet);
    }

    while (charms.size() > 0) {
      packet.page = page;
      packet.tx = charms.back()->path->x;
      packet.ty = charms.back()->path->y;
      packet.guid = charms.back()->guid;

      charms.pop_back();

      instance.send_packet(
          player->player_data->net_client, &packet, sizeof packet);
    }

    d2_game::update_inventory_items(game, player);
    return true;
  }

  // sorting failed, remove all items and re-insert them at old coords

  for (auto& backup_item : backup_items) {
    const auto item = backup_item.item;

    d2_common::inv_remove_item(player->inventory, item);
  }

  while (backup_items.size() > 0) {
    const auto backup = backup_items.back();
    const auto item = backup.item;
    const auto tx = backup.x;
    const auto ty = backup.y;

    packet.page = page;
    packet.tx = tx;
    packet.ty = ty;
    packet.guid = item->guid;

    d2_common::inv_add_item(
        player->inventory, item, tx, ty, inventoryIndex, false, page);
    d2_common::inv_update_item(player->inventory, item, false);

    backup_items.pop_back();

    instance.send_packet(
        player->player_data->net_client, &packet, sizeof packet);
  }

  d2_game::update_inventory_items(game, player);

  return false;
}

bool autosort::find_free_space(Inventory* inv,
                               Unit* item,
                               int32_t inventoryIndex,
                               char page,
                               uint32_t& x,
                               uint32_t& y,
                               bool isCharmZone) {
  char data[0x18];
  d2_common::get_inventory_data(inventoryIndex, 0, data);

  // inventory coords
  const auto mx = static_cast<uint32_t>(data[0]);
  const auto my = static_cast<uint32_t>(data[1]);

  const unsigned int minValidX = isCharmZone ? cminValidX : iminValidX;
  const unsigned int maxValidX = isCharmZone ? cmaxValidX : imaxValidX;
  const unsigned int minValidY = isCharmZone ? cminValidY : iminValidY;
  const unsigned int maxValidY = isCharmZone ? cmaxValidY : imaxValidY;

  // Get the item record to access the item size
  const auto record = d2_common::get_item_record(item->data_record_index);

  // Update the maximum valid Y coordinate for the inventory zone
  const unsigned int inventoryMaxValidY = isCharmZone ? maxValidY : 11;

  if (page == 0x03 || page == 0x04) {
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
  } else {
    for (x = minValidX; x <= maxValidX - record->inv_width + 1; x++) {
      for (y = minValidY; y <= inventoryMaxValidY - record->inv_height + 1;
           y++) {
        Unit* blockingUnit = nullptr;
        uint32_t blockingUnitIndex = 0;

        // Check if the item can be placed at the current position without
        // overflowing
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
}

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
