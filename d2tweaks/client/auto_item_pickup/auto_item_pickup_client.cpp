
#include <Windows.h>

#include <D2Template.h>
#include <DllNotify.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "common/autopickup_lootfilter.h"
#include "common/strings.h"
#include "common/file_ini.h"
#include "common/hooking.h"

#include "diablo2/structures/data/item_types_line.h"
#include "diablo2/structures/data/items_line.h"
#include "diablo2/structures/inventory.h"
#include "diablo2/structures/item_data.h"
#include "diablo2/structures/player_data.h"
#include "diablo2/structures/room.h"
#include "diablo2/structures/unit.h"

#include "diablo2/d2client.h"
#include "diablo2/d2common.h"
#include "diablo2/d2gfx.h"
#include "diablo2/d2lang.h"
#include "diablo2/d2win.h"

#include "d2tweaks/common/asset_manager.h"
#include "d2tweaks/common/common.h"
#include "d2tweaks/common/protocol.h"

#include "d2tweaks/ui/controls/button.h"
#include "d2tweaks/ui/menu.h"
#include "d2tweaks/ui/ui_manager.h"

#include "d2tweaks/client/client.h"
#include "d2tweaks/client/auto_item_pickup/auto_item_pickup_client.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

static int32_t m_iDistance = 0;
static uint32_t m_bAutoItemPickupEnabled = 0;
static uint32_t m_nTick = 0;

static bool m_bToggleAutoItemPickup = false;
static bool m_bInventoryFull = false;

static std::vector<ItemCode> m_stItemList;
static std::vector<ItemType> m_stItemTypes;

void ReloadFilters(std::string szPathToIni) {
  FileIni config(szPathToIni);

  std::string m_acItemListAll;
  std::string m_acItemTypesAll;

  std::string key;
  uint32_t key_count;
  std::string buffer;

  key_count = config.Int("AutoItemPickup", "ItemListCount", 30);
  for (uint32_t i = 0; i < key_count; i++) {
    key = utils::string_format("ItemList%d", i + 1);
    buffer = config.String("AutoItemPickup", key, MAX_STRING_LENGHT - 1);
    if (!buffer.empty()) {
      if (m_acItemListAll.empty()) {
        m_acItemListAll += buffer;
      } else {
        m_acItemListAll += "|";
        m_acItemListAll += buffer;
      }
    }
  }

  key_count = config.Int("AutoItemPickup", "ItemTypeListCount", 10);
  for (uint32_t i = 0; i < key_count; i++) {
    key = utils::string_format("ItemTypeList%d", i + 1);
    buffer = config.String("AutoItemPickup", key, MAX_STRING_LENGHT - 1);
    if (m_acItemTypesAll.empty()) {
      m_acItemTypesAll += buffer;
    } else {
      m_acItemTypesAll += "|";
      m_acItemTypesAll += buffer;
    }
  }

  const std::string& delimiters = " ,|";  // ' ' or ',' or '|'
  size_t start, end;
  std::string input;
  std::string token;

  /////// Parse ItemCode
  m_stItemList.clear();

  ItemCode item_code;
  input = m_acItemListAll;
  start = input.find_first_not_of(delimiters);
  while (start != std::string::npos) {
    end = input.find_first_of(delimiters, start);
    token = input.substr(start, end - start);
    start = input.find_first_not_of(delimiters, end);

    if (token.empty() || token.length() < 3) {
      continue;
    }

    memset(&item_code, 0, sizeof(item_code));

    item_code.code0 = token[0];
    item_code.code1 = token[1];
    item_code.code2 = token[2];

    if (token.length() > 3 && token[3] == ':') {
      // first index quality = 1
      memset(item_code.qualityinclude, 0, sizeof(item_code.qualityinclude));

      if (token.length() <= 13) { //for example jew:123456789
        // p = 4 is first digit after ':'
        for (uint32_t p = 4; p <= token.length(); p++) {
          if (token[p] >= '0' && token[p] <= '9') {
            item_code.qualityinclude[token[p] - '0'] = true;
          }
        }
      }
    }

    if (token.length() > 3 && token[3] == '-') {
      // first index quality = 1
      memset(item_code.qualityinclude, 1, sizeof(item_code.qualityinclude));

      if (token.length() <= 13) { //for example jew:123456789
        // p = 4 is first digit after '-'
        for (uint32_t p = 4; p <= token.length(); p++) {
          if (token[p] >= '0' && token[p] <= '9') {
            item_code.qualityinclude[token[p] - '0'] = false;
          }
        }
      }
    }

    if ((token.length() == 3) || (token[3] != '-' && token[3] != ':')) {
      // first index quality = 1
      memset(item_code.qualityinclude, 1, sizeof(item_code.qualityinclude));
    }

    m_stItemList.push_back(item_code);
  }

  /////// Parse ItemType
  m_stItemTypes.clear();

  ItemType item_type;
  input = m_acItemTypesAll;
  start = input.find_first_not_of(delimiters);
  while (start != std::string::npos) {
    end = input.find_first_of(delimiters, start);
    token = input.substr(start, end - start);
    start = input.find_first_not_of(delimiters, end);

    if (token.empty() || token.length() < 4) {
      continue;
    }

    memset(&item_type, 0, sizeof(item_type));

    item_type.dwtype = *(uint32_t *) &token[0];

    if (token.length() > 4 && token[4] == ':') {
      // first index quality = 1
      memset(item_type.qualityinclude, 0, sizeof(item_type.qualityinclude));

      if (token.length() <= 14) { //for example tors:123456789
        // p = 5 is first digit after ':'
        for (uint32_t p = 5; p <= token.length(); p++) {
          if (token[p] >= '0' && token[p] <= '9') {
            item_type.qualityinclude[token[p] - '0'] = true;
          }
        }
      }
    }

    if (token.length() > 4 && token[4] == '-') {
      // first index quality = 1
      memset(item_type.qualityinclude, 1, sizeof(item_type.qualityinclude));

      if (token.length() <= 14) { //for example armo:123456789
        // p = 5 is first digit after '-'
        for (uint32_t p = 5; p <= token.length(); p++) {
          if (token[p] >= '0' && token[p] <= '9') {
            item_type.qualityinclude[token[p] - '0'] = false;
          }
        }
      }
    }

    if ((token.length() == 4) || (token[4] != '-' && token[4] != ':')) {
      // first index quality = 1
      memset(item_type.qualityinclude, 1, sizeof(item_type.qualityinclude));
    }

    m_stItemTypes.push_back(item_type);
  }
}

namespace d2_tweaks {
namespace client {

class auto_item_pickup_menu : public ui::menu {
  common::asset* m_buttons_img;
  button* m_auto_pickup_btn;

 public:
  auto_item_pickup_menu() {
    menu::set_enabled(true);
    menu::set_visible(true);

    // load_xml("d2tweaks\\interfaces\\autopickup.xml");
    if (DLLBASE_D2EXPRES != 0)
      load_xml("d2tweaks\\interface_d2expres\\autopickup.xml");
    if (DLLBASE_SGD2FREERES != 0)
      load_xml("d2tweaks\\interface_sgd2freeres\\autopickup.xml");
    if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
      load_xml("d2tweaks\\interface_vanilla\\autopickup.xml");

    m_buttons_img = common::asset_manager::instance().get_mpq_file(
        "d2tweaks\\assets\\buttons", common::MPQ_FILE_TYPE_DC6);
    m_auto_pickup_btn = get_button(
        "m_auto_pickup_btn",
        std::bind(&auto_item_pickup_menu::auto_item_pickup_click, this));
  }

  void draw() override {
    if (m_bToggleAutoItemPickup) {
      // d2_client::print_chat(L"AUTO TRANSMUTE ON", 1);
      m_auto_pickup_btn->set_current_frame(8);  // index of frame in buttons.dc6
    } else if (!m_bToggleAutoItemPickup) {
      // d2_client::print_chat(L"AUTO TRANSMUTE OFF", 2);
      m_auto_pickup_btn->set_current_frame(6);
    }

    if (m_bToggleAutoItemPickup && m_bInventoryFull) {
      m_auto_pickup_btn->set_current_frame(7);
    }

    // if (d2_gfx::get_resolution_mode() == 0) {
    //   m_auto_pickup_btn->set_x(g_btn_hires_posx);
    //   m_auto_pickup_btn->set_y(g_btn_hires_posy);
    // } else {
    //   m_auto_pickup_btn->set_x(g_btn_lowres_posx);
    //   m_auto_pickup_btn->set_y(g_btn_lowres_posy);
    // }

    if (!should_draw()) {
      m_auto_pickup_btn->set_enabled(false);
      m_auto_pickup_btn->set_visible(false);
      return;
    }

    m_auto_pickup_btn->set_enabled(true);
    m_auto_pickup_btn->set_visible(true);

    menu::draw();
  }

 private:
  static bool should_draw() {
    return d2_client::get_ui_window_state(UI_WINDOW_INTERFACE);
  }

  button* get_button(const std::string& name,
                     const std::function<void()>& onClick) {
    auto result = static_cast<button*>(get_control(name));

    result->set_on_click(onClick);

    result->set_enabled(false);
    result->set_visible(false);

    return result;
  }

  void auto_item_pickup_click() {
    m_bToggleAutoItemPickup ^= true;

    if (m_bToggleAutoItemPickup) {
      d2_client::print_chat(L"AUTO PICKUP ON", 1);
    } else {
      d2_client::print_chat(L"AUTO PICKUP OFF", 2);
    }
  }
};

MODULE_INIT(auto_item_pickup)

void auto_item_pickup::init_early() {}

void auto_item_pickup::init() {
  FileIni config(common::get_config_path());

  if (config.Int("modules", "AutoItemPickup", 1)) {
    m_iDistance = config.Int("AutoItemPickup", "PickupDistance", 4);

    ReloadFilters(common::get_config_path());

    ui::Manager::instance().add_menu(new auto_item_pickup_menu());
    Client::instance().register_tick_handler(this);
    Client::instance().register_packet_handler(
        common::message_types_t::MESSAGE_TYPE_ITEM_PICKUP_INFO, this);
  }
}

bool find_free_space(Inventory* inv,
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

void auto_item_pickup::tick() {
  static common::item_pickup_info_sc packet;
  const auto unit = d2_client::get_local_player();

  // spdlog::debug("g_tick_between_item_pickup {0}",
  // g_tick_between_item_pickup);
  m_nTick++;

  if (!m_bToggleAutoItemPickup)
    return;

  if (!unit)
    return;

  if (unit->type != unit_type_t::UNIT_TYPE_PLAYER)
    return;

  const auto room = d2_common::get_room_from_unit(unit);

  if (!room)
    return;

  for (auto item = room->unit; item; item = item->prev_unit_in_room) {
    if (!item)
      continue;

    if (item->type != unit_type_t::UNIT_TYPE_ITEM)
      continue;

    const auto record = d2_common::get_item_record(item->data_record_index);

    if (!record)
      continue;

    const auto distance = d2_common::get_distance_between_units(unit, item);
    if (distance > m_iDistance)
      continue;

    // // Only one item can be picked up per frame; picking more will cause items
    // // to disappear.
    // if (record->string_code[0] == 'g' &&
    //     record->string_code[1] == 'l' &&
    //     record->string_code[2] == 'd') {
    //   static common::gold_pickup_info_cs request_packet_cs;
    //   request_packet_cs.item_guid = item->guid;
    //   d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
    //   continue;
    // }

    const auto itemtype_record = d2_common::get_item_type_record(record->type);
    auto itemtype_record_equiv1 =
        d2_common::get_item_type_record(itemtype_record->equiv1);
    auto itemtype_record_equiv2 =
        d2_common::get_item_type_record(itemtype_record->equiv2);

    uint32_t quality = d2_common::get_item_quality(item);

    // Clear the array
    char arr_itemtype_codestr_equivstr[20][5] = {0};

    // Item type code is always the first element in the array
    *(DWORD*) arr_itemtype_codestr_equivstr[0] =
        *(DWORD*) itemtype_record->code;
    // index second code in array
    uint32_t index_arr_itemtype = 1;

    if (itemtype_record_equiv1) {
      if (*(DWORD*) itemtype_record_equiv1->code != 0x20202020) {
        // Save the first previously obtained equiv1
        *(DWORD*) arr_itemtype_codestr_equivstr[index_arr_itemtype] =
            *(DWORD*) itemtype_record_equiv1->code;
        index_arr_itemtype++;
        // Unpack all equiv1 into an array
        for (index_arr_itemtype; itemtype_record_equiv1->equiv1 != 0;
             index_arr_itemtype++) {
          // Get the next one
          itemtype_record_equiv1 =
              d2_common::get_item_type_record(itemtype_record_equiv1->equiv1);
          if (*(DWORD*) itemtype_record_equiv1->code != 0x20202020) {
            *(DWORD*) arr_itemtype_codestr_equivstr[index_arr_itemtype] =
                *(DWORD*) itemtype_record_equiv1->code;
          } else
            break;
        }
      }
    }

    if (itemtype_record_equiv2) {
      if (*(DWORD*) itemtype_record_equiv2->code != 0x20202020) {
        // Save the first previously obtained equiv1
        *(DWORD*) arr_itemtype_codestr_equivstr[index_arr_itemtype] =
            *(DWORD*) itemtype_record_equiv2->code;
        index_arr_itemtype++;
        // Expand all equiv1 into an array
        for (index_arr_itemtype; itemtype_record_equiv2->equiv2 != 0;
             index_arr_itemtype++) {
          // Get the next one
          itemtype_record_equiv2 =
              d2_common::get_item_type_record(itemtype_record_equiv2->equiv2);
          if (*(DWORD*) itemtype_record_equiv2->code != 0x20202020) {
            *(DWORD*) arr_itemtype_codestr_equivstr[index_arr_itemtype] =
                *(DWORD*) itemtype_record_equiv2->code;
          } else
            break;
        }
      }
    }

    static common::item_pickup_info_cs request_packet_cs;
    request_packet_cs.item_guid = 0;

    for (uint32_t i = 0; i < m_stItemTypes.size(); i++) {
      for (uint32_t count = 0; count < index_arr_itemtype; count++) {
        if (*(DWORD*) arr_itemtype_codestr_equivstr[count] ==
            (DWORD) m_stItemTypes[i].dwtype) {
          if (m_stItemTypes[i].qualityinclude[quality] == TRUE) {
            const auto inventoryIndex =
                d2_common::get_inventory_index(unit, 0, 0x65);
            uint32_t tx = 0, ty = 0;
            if (!find_free_space(
                    unit->inventory, item, inventoryIndex, 0, tx, ty)) {
              m_bInventoryFull = true;
              if (m_nTick >= 500) {
                d2_client::play_sound(0xB76, unit, 0, 0, 0);
                m_nTick = 0;
              }
              goto L1;
            } else {
              m_bInventoryFull = false;
            }

            request_packet_cs.item_guid = item->guid;
            d2_client::send_to_server(&request_packet_cs,
                                      sizeof request_packet_cs);
            goto L1;
          }
        }
      }
    }

    for (uint32_t i = 0; i < m_stItemList.size(); i++) {
      if (record->string_code[0] == m_stItemList[i].code0 &&
          record->string_code[1] == m_stItemList[i].code1 &&
          record->string_code[2] == m_stItemList[i].code2) {
        if (m_stItemList[i].qualityinclude[quality] == TRUE) {
          const auto inventoryIndex =
              d2_common::get_inventory_index(unit, 0, 0x65);
          uint32_t tx = 0, ty = 0;
          if (!find_free_space(
                  unit->inventory, item, inventoryIndex, 0, tx, ty)) {
            m_bInventoryFull = true;
            if (m_nTick >= 500) {
              d2_client::play_sound(0xB76, unit, 0, 0, 0);
              m_nTick = 0;
            }
            goto L1;
          } else {
            m_bInventoryFull = false;
          }

          request_packet_cs.item_guid = item->guid;
          d2_client::send_to_server(&request_packet_cs,
                                    sizeof request_packet_cs);
          goto L1;
        }
      }
    }
  L1:;
  }
}

void auto_item_pickup::handle_packet(common::packet_header* packet) {
  const auto info = static_cast<common::item_pickup_info_sc*>(packet);

  // if (info->inventory_full == true) {
  //   m_nTick = 0;
  // }
  // else {
  //   g_tick_between_item_pickup = g_delay_between_item_pickup;
  // }
  // g_value += info->gold;
}

}  // namespace client
}  // namespace d2_tweaks
