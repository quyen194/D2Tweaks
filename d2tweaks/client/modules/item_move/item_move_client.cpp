#include <common/file_ini.h>
#include <common/hooking.h>
#include <common/ptr_wrapper.h>
#include <d2tweaks/client/client.h>
#include <d2tweaks/client/modules/item_move/item_move_client.h>
#include <d2tweaks/common/asset_manager.h>
#include <d2tweaks/common/protocol.h>
#include <diablo2/d2client.h>
#include <diablo2/d2common.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data_tables.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/unit.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace client {
namespace modules {

MODULE_INIT(item_move)

int32_t(__fastcall* g_item_click_original)(Unit* playerUnit,
                                           inventory* inventory,
                                           int mouse_x,
                                           int mouse_y,
                                           uint8_t flag,
                                           void* a6,
                                           unsigned int page);

char get_target_page(char currentPage) {
  if (currentPage == 0) { //item is in inventory
    if (d2_client::get_ui_window_state(UI_WINDOW_STASH))
      return 4;

    if (d2_client::get_ui_window_state(UI_WINDOW_CUBE))
      return 3;
  }

  return 0;
}

void request_item_move(Unit* item, char targetPage) {
  static common::item_move_cs packet;

  packet.item_guid = item->guid;
  packet.target_page = targetPage;

  d2_client::send_to_server(&packet, sizeof packet);
}

int32_t __fastcall item_click(Unit* owner,
                              inventory* inventory,
                              int mouse_x,
                              int mouse_y,
                              uint8_t flag,
                              char* a6,
                              unsigned int page) {
  const auto player = d2_client::get_local_player();

  if (owner->guid != player->guid)
    return g_item_click_original(owner, inventory, mouse_x, mouse_y, flag, a6, page);

  if ((static_cast<uint16_t>(GetAsyncKeyState(VK_CONTROL)) >> 8 & 0x80u) == 0)
    return g_item_click_original(owner, inventory, mouse_x, mouse_y, flag, a6, page);

  if (!d2_client::get_ui_window_state(UI_WINDOW_CUBE) &&
      !d2_client::get_ui_window_state(UI_WINDOW_STASH)) {
    return g_item_click_original(owner, inventory, mouse_x, mouse_y, flag, a6, page);
  }

  //code below taken from IDA directly, so that's why it is so ugly
  const auto coefx1 = *(reinterpret_cast<uint32_t*>(a6) + 1);
  const auto coefx2 = static_cast<unsigned int>(a6[20]);

  const auto coefy1 = *(reinterpret_cast<uint32_t*>(a6) + 3);
  const auto coefy2 = static_cast<unsigned int>(a6[21]);

  const auto itemx = (mouse_x - coefx1) / coefx2;
  const auto itemy = (mouse_y - coefy1) / coefy2;

  Unit* cubeItem = nullptr;

  uint32_t px, py;

  const auto currentInventoryIndex =
      d2_common::get_inventory_index(player, page, d2_client::is_lod());
  const auto clickedItem = d2_common::get_item_at_cell(
      player->inventory, itemx, itemy, &px, &py, currentInventoryIndex, page);

  for (auto item = player->inventory->first_item; item != nullptr;
       item = item->item_data->pt_next_item) {
    auto record = d2_common::get_item_record(item->data_record_index);

    //auto flags1 = d2_common::get_item_record(item->item_data->flags1);
    //auto flags2 = d2_common::get_item_record(item->item_data->flags2);

    if (record->string_code[0] == 'b' &&
      record->string_code[1] == 'o' &&
      record->string_code[2] == 'x') { //Cube
      cubeItem = item;
      break;
    }
  }

  static wrap_value<data_tables*> dataTables(0x96A20, d2_common::get_base());
  auto sgptdataTables = *dataTables;

  if (clickedItem != nullptr) {
    const auto itemid = d2_common::get_item_unique_index(clickedItem);
    const auto itemRecord = d2_common::get_item_record(clickedItem->data_record_index);
    if (itemid != 0) {
      if ((itemid >= 0) && (itemid < (unsigned int)sgptdataTables->nUniqueItems)) {
        unique_items* uniqueItems = sgptdataTables->pUniqueItems + itemid;
        if (uniqueItems && (uniqueItems->carry1 == 1)) {
          //spdlog::warn("ItemID int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", itemid);
          d2_client::play_sound(itemRecord->drop_sound, nullptr, 0, 0, 0);
          return g_item_click_original(owner, inventory, mouse_x, mouse_y, flag, a6, page);
        }
      }
    }
  }

  if (clickedItem == nullptr) {
    return g_item_click_original(
        owner, inventory, mouse_x, mouse_y, flag, a6, page);
  }

  const auto targetPage = get_target_page(page);

  if (targetPage == 0x03 && clickedItem == cubeItem)
    return g_item_click_original(
        owner, inventory, mouse_x, mouse_y, flag, a6, page);

  request_item_move(clickedItem, targetPage);
  return 0;
}

void item_move::init_early() {
}

void item_move::init() {
  FileIni config(common::get_config_path());

  if (config.Int("modules", "ItemMover", 1)) {
    hooking::hook(d2_client::get_base() + 0x475C0,
                  item_click,
                  reinterpret_cast<void**>(&g_item_click_original));
    singleton<client>::instance().register_packet_handler(
        common::MESSAGE_TYPE_ITEM_MOVE, this);
  }
}

// handle packet coming from the server
void item_move::handle_packet(common::packet_header* packet) {
  static auto& instance = singleton<client>::instance();

  const auto itemMove = static_cast<common::item_move_sc*>(packet);
  const auto item = instance.get_client_unit(
      0x04, itemMove->item_guid);  // 0x03 -> 0x04 - item

  if (item == nullptr)
    return;

  const auto player = d2_client::get_local_player();

  //Last parameter is some boolean
  const auto inventoryIndex = d2_common::get_inventory_index(
      player, itemMove->target_page, d2_client::is_lod());

  item->item_data->page = itemMove->target_page;

  d2_common::inv_add_item(player->inventory,
                          item,
                          itemMove->tx,
                          itemMove->ty,
                          inventoryIndex,
                          true,
                          item->item_data->page);
  d2_common::inv_update_item(player->inventory, item, true);

  const auto itemRecord = d2_common::get_item_record(item->data_record_index);

  if (itemRecord != nullptr)
    d2_client::play_sound(itemRecord->drop_sound, nullptr, 0, 0, 0);
  else
    d2_client::play_sound(4, nullptr, 0, 0, 0);
}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
