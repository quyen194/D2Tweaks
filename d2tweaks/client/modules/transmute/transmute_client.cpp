
#include <Windows.h>

#include <DllNotify.h>
#include <D2Template.h>

#include <spdlog/spdlog.h>

#include "common/autopickup_lootfilter.h"
#include "common/debug.h"
#include "common/file_ini.h"
#include "common/hooking.h"
#include "common/strings.h"
#include "common/timer.h"

#include "diablo2/structures/data/item_types_line.h"
#include "diablo2/structures/data/items_line.h"
#include "diablo2/structures/inventory.h"
#include "diablo2/structures/item_data.h"
#include "diablo2/structures/unit.h"

#include "diablo2/d2client.h"
#include "diablo2/d2common.h"
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
#include "d2tweaks/ui/ui_utils.h"

#include "d2tweaks/client/client.h"
#include "d2tweaks/client/modules/transmute/transmute_client.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;
using namespace timer;

namespace d2_tweaks {
namespace client {
namespace modules {

MODULE_INIT(transmute)

static uint32_t m_nTransmuteSound = 0;
static bool m_bToggleTransmute = 0;

static uint32_t m_nDelayFrames = 10;
static uint32_t m_nCountFrames = 0;

static bool m_game_init_done = false;

static const uint8_t PAGE_CUBE = 3;
static const uint8_t PAGE_STASH = 4;
static const uint8_t PAGE_INVENTORY = 0;

static std::vector<ItemCode> m_stItemList;
static std::vector<ItemType> m_stItemTypes;

static void(__fastcall* fn_hook_play_sound)(uint32_t soundId, uint32_t* unit, const uint32_t ticks, const BOOL prePick, const uint32_t cache);
static void(__fastcall* fn_hook_game_end)();

enum transmute_command {
  COMMAND_NULL,
  COMMAND_TRANSMUTE_START,
  COMMAND_TRANSMUTE_END,
  COMMAND_CALL_TRANSMUTE,
  COMMAND_MOVE_ITEM,
  COMMAND_ABORT
};

class auto_transmute_menu : public ui::menu {
  common::asset* m_buttons_img;
  button* m_auto_transmute_btn;
#ifndef NDEBUG
  CountdownTimer reload_setting_timer;
#endif

 public:
  auto_transmute_menu() {
    menu::set_enabled(true);
    menu::set_visible(true);

    load_settings();

    m_buttons_img = common::asset_manager::instance().get_mpq_file("d2tweaks\\assets\\buttons", common::MPQ_FILE_TYPE_DC6);
    m_auto_transmute_btn = get_button("m_auto_transmute_btn", std::bind(&auto_transmute_menu::auto_transmute_click, this));

#ifndef NDEBUG
    reload_setting_timer.setDuration(1000);
#endif
  }

  void load_settings() {
    //load_xml("d2tweaks\\interfaces\\autotransmute.xml");
    if (DLLBASE_D2EXPRES != 0)
      load_xml("d2tweaks\\interface_d2expres\\autotransmute.xml");
    if (DLLBASE_SGD2FREERES != 0)
      load_xml("d2tweaks\\interface_sgd2freeres\\autotransmute.xml");
    if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
      load_xml("d2tweaks\\interface_vanilla\\autotransmute.xml");
  }

  void draw() override {
#ifndef NDEBUG
    if (!reload_setting_timer.isRunning()) {
      reload_setting_timer.setDuration(1000);
      reload_setting_timer.start();
    }

    if (reload_setting_timer.isExpired()) {
      load_settings();
      reload_setting_timer.restart();
    }
#endif

    if (m_bToggleTransmute) {
      // d2_client::print_chat(L"AUTO TRANSMUTE ON", 1);
      // index of frame in buttons.dc6
      // m_auto_transmute_btn->set_current_frame(m_auto_transmute_btn->get_frame_down());
    } else {
      // d2_client::print_chat(L"AUTO TRANSMUTE OFF", 2);
      // index of frame in buttons.dc6
      // m_auto_transmute_btn->set_current_frame(m_auto_transmute_btn->get_frame_up());
    }

    if (!should_draw()) {
      m_auto_transmute_btn->set_enabled(false);
      m_auto_transmute_btn->set_visible(false);
      return;
    }

    m_auto_transmute_btn->set_enabled(d2_client::get_ui_window_state(UI_WINDOW_CUBE));
    m_auto_transmute_btn->set_visible(d2_client::get_ui_window_state(UI_WINDOW_CUBE));

    menu::draw();
  }

 private:
  static bool should_draw() {
    return d2_client::get_ui_window_state(UI_WINDOW_CUBE);
  }

  button* get_button(const std::string& name, const std::function<void()>& onClick) {
    auto result = static_cast<button*>(get_control(name));

    result->set_on_click(onClick);

    result->set_enabled(false);
    result->set_visible(false);

    return result;
  }

  void auto_transmute_click() {
    send_request(0x00);
  }

  void send_request(const uint8_t page) {
    static common::transmute_info_cs request_packet_cs;

    m_bToggleTransmute ^= true;

    //request_packet_cs.transmute_start_flag = m_bToggleTransmute;
    //request_packet_cs.command = COMMAND_TRANSMUTE_START;
    //d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);

    if (m_bToggleTransmute) {
      d2_client::print_chat(L"AUTO TRANSMUTE ON", 1);
    } else {
      d2_client::print_chat(L"AUTO TRANSMUTE OFF", 2);
    }
  }
};

void __fastcall hook_play_sound(uint32_t soundId, uint32_t* unit, const uint32_t ticks, const BOOL prePick, const uint32_t cache) {
  if (m_bToggleTransmute) {
    if (soundId == 0xB)
      soundId = 0;
  }

  fn_hook_play_sound(soundId, unit, ticks, prePick, cache);
  return;
}

void __stdcall hook_game_end() {
  m_game_init_done = false;
  m_bToggleTransmute = false;
  return;
}

__declspec (naked) void hook_game_end_asm() {
  __asm {
    pushad;
    pushfd;
    call hook_game_end;
    popfd;
    popad;

    jmp[fn_hook_game_end];
  }
}

void transmute::init_early() {
}

void transmute::init() {
  FileIni config(common::get_config_path());

  // CIni recipes(config_path);
  // // key enlarging the buffer unless we make sure all section names are loaded.
  // DWORD dwBufSize = 1024;
  // char* pszBuffer = new char[dwBufSize];
  // DWORD dwCopied = recipes.GetSectionNames(pszBuffer, dwBufSize - 1);
  // while (dwCopied + 128 >= dwBufSize) {
  //   dwBufSize += 1024;
  //   delete[] pszBuffer;
  //   pszBuffer = new char[dwBufSize];
  //   dwCopied = recipes.GetSectionNames(pszBuffer, dwBufSize - 1);
  // }
  // for (char* pSection = pszBuffer; pSection[0]; pSection = &pSection[strlen(pSection) + 1]) {
  // }

  if (!config.Int("modules", "AutoTransmute", 1)) {
    return;
  }

  m_nDelayFrames = config.Int("AutoTransmute", "DelayInFrames", 10);
  m_nTransmuteSound = config.Int("AutoTransmute", "EnableTransmuteSound", 0);

  std::string m_acItemListAll;
  std::string m_acItemTypesAll;

  std::string key;
  uint32_t key_count;
  std::string buffer;

  key_count = config.Int("AutoTransmute", "ItemListCount", 10);
  for (uint32_t i = 0; i < key_count; i++) {
    key = utils::string_format("ItemList%d", i + 1);
    buffer = config.String("AutoTransmute", key, MAX_STRING_LENGHT - 1);
    if (!buffer.empty()) {
      if (m_acItemListAll.empty()) {
        m_acItemListAll += buffer;
      } else {
        m_acItemListAll += "|";
        m_acItemListAll += buffer;
      }
    }
  }

  key_count = config.Int("AutoTransmute", "ItemTypeListCount", 10);
  for (uint32_t i = 0; i < key_count; i++) {
    key = utils::string_format("ItemTypeList%d", i + 1);
    buffer = config.String("AutoTransmute", key, MAX_STRING_LENGHT - 1);
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

  if (m_nTransmuteSound == false) {
    detour::hook(d2_client::get_base() + 0xB5820,
                 hook_play_sound,
                 reinterpret_cast<void**>(&fn_hook_play_sound));
  }

  detour::hook(d2_client::get_base() + 0xB528,
               hook_game_end_asm,
               reinterpret_cast<void**>(&fn_hook_game_end));

  ui::ui_manager::instance().add_menu(new auto_transmute_menu());
  client::instance().register_tick_handler(this);
  client::instance().register_packet_handler(common::message_types_t::MESSAGE_TYPE_TRANSMUTE, this);
}

void transmute::tick() {
  const auto unit = d2_client::get_local_player();

  if (m_game_init_done == false) {
    m_game_init_done = true;
    m_bToggleTransmute = false;
  }

  if (!unit)
    return;

  if (unit->type != unit_type_t::UNIT_TYPE_PLAYER)
    return;

  if (!m_bToggleTransmute)
    return;

  if (m_nCountFrames > m_nDelayFrames) {
    m_nCountFrames = 0;

    auto item = d2_common::get_first_inventory_item(unit->inventory);

    for (item; item != 0; item = d2_common::get_next_inventory_item(item)) {
      auto currentpage = d2_common::get_item_page(item);
      if (currentpage == PAGE_INVENTORY) {
        const auto record = d2_common::get_item_record(item->data_record_index);

        if (*(DWORD*)record->string_code == *(DWORD*)"box ") {
          continue;
        }

        const auto itemtype_record = d2_common::get_item_type_record(record->type);
        auto itemtype_record_equiv1 = d2_common::get_item_type_record(itemtype_record->equiv1);
        auto itemtype_record_equiv2 = d2_common::get_item_type_record(itemtype_record->equiv2);

        uint32_t quality = d2_common::get_item_quality(item);
        // Clear the array
        char arr_itemtype_codestr_equivstr[20][5] = { 0 };

        // Item type code is always the first element in the array
        *(DWORD*)arr_itemtype_codestr_equivstr[0] = *(DWORD*)itemtype_record->code;
        // Index of the second code in the array
        uint32_t index_arr_itemtype = 1;

        // Prepare equiv1 array
        if (itemtype_record_equiv1) {
          if (*(DWORD*)itemtype_record_equiv1->code != 0x20202020) {
            // Save the first previously obtained equiv1
            *(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv1->code;
            index_arr_itemtype++;
            // Expand all equiv1 into the array
            for (; itemtype_record_equiv1->equiv1 != 0; index_arr_itemtype++) {
              // Get the next one
              itemtype_record_equiv1 = d2_common::get_item_type_record(itemtype_record_equiv1->equiv1);
              if (*(DWORD*)itemtype_record_equiv1->code != 0x20202020) {
                *(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv1->code;
              }
              else break;
            }
          }
        }

        // Prepare equiv2 array
        if (itemtype_record_equiv2) {
          if (*(DWORD*)itemtype_record_equiv2->code != 0x20202020) {
            // Save the first previously obtained equiv2
            *(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv2->code;
            index_arr_itemtype++;
            // Expand all equiv2 into the array
            for (; itemtype_record_equiv2->equiv2 != 0; index_arr_itemtype++) {
              // Get the next one
              itemtype_record_equiv2 = d2_common::get_item_type_record(itemtype_record_equiv2->equiv2);
              if (*(DWORD*)itemtype_record_equiv2->code != 0x20202020) {
                *(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv2->code;
              }
              else break;
            }
          }
        }

        for (uint32_t i = 0; i < m_stItemTypes.size(); i++) {
          for (uint32_t count = 0; count < index_arr_itemtype; count++) {
            if (*(DWORD*)arr_itemtype_codestr_equivstr[count] == (DWORD)m_stItemTypes[i].dwtype) {
              if (m_stItemTypes[i].qualityinclude[quality] == true) {
                static common::transmute_info_cs request_packet_cs;
                request_packet_cs.command = COMMAND_MOVE_ITEM;
                request_packet_cs.item_guid = item->guid;
                request_packet_cs.target_page = PAGE_CUBE;
                d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
                goto L1;
              }
            }
          }
        }

        for (uint32_t i = 0; i < m_stItemList.size(); i++) {
          if (record->string_code[0] == m_stItemList[i].code0 &&
            record->string_code[1] == m_stItemList[i].code1 &&
            record->string_code[2] == m_stItemList[i].code2) {
            if (m_stItemList[i].qualityinclude[quality] == true) {
              static common::transmute_info_cs request_packet_cs;
              request_packet_cs.command = COMMAND_MOVE_ITEM;
              request_packet_cs.item_guid = item->guid;
              request_packet_cs.target_page = PAGE_CUBE;
              d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
              goto L1;
            }
          }
        }
      }
    }
  }

L1:;
  m_nCountFrames++;
}

void transmute::handle_packet(common::packet_header* packet) {
  static auto& instance = client::instance();
  const auto income_packet_sc = static_cast<common::transmute_info_sc*>(packet);
  static common::transmute_info_cs request_packet_cs;

  if (income_packet_sc->command == COMMAND_MOVE_ITEM) {
    const auto item = instance.get_client_unit(0x04, income_packet_sc->item_guid); //0x03 -> 0x04 - item

    if (item == nullptr)
      return;

    const auto player = d2_client::get_local_player();

    //Last parameter is some boolean
    const auto inventoryIndex = d2_common::get_inventory_index(
        player, income_packet_sc->target_page, d2_client::is_lod());

    item->item_data->page = income_packet_sc->target_page;

    d2_common::inv_add_item(player->inventory,
                            item,
                            income_packet_sc->tx,
                            income_packet_sc->ty,
                            inventoryIndex,
                            true,
                            item->item_data->page);
    d2_common::inv_update_item(player->inventory, item, true);

    // send 'transmute' command
    request_packet_cs.command = COMMAND_CALL_TRANSMUTE;
    d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);

    //d2_client::send_to_server_7(0x4F, 0x18, 0, 0);
  }

  if (income_packet_sc->command == COMMAND_ABORT) {
    m_bToggleTransmute = false;
    d2_client::print_chat(L"WRONG RECIPE, AUTO TRANSMUTE OFF", 2);
  }
}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
