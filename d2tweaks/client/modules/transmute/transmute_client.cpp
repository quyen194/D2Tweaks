#include <d2tweaks/client/modules/transmute/transmute_client.h>
#include <d2tweaks/client/client.h>

#include <common/hooking.h>
#include <d2tweaks/common/common.h>
#include <d2tweaks/common/protocol.h>
#include <d2tweaks/common/asset_manager.h>

#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>
#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/controls/button.h>

#include <spdlog/spdlog.h>

#include <diablo2/d2common.h>
#include <diablo2/d2lang.h>
#include <diablo2/d2client.h>
#include <diablo2/d2win.h>
#include <diablo2/d2gfx.h>
#include <diablo2/d2common.h>

//#include <diablo2/structures/data_tables.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>
#include <diablo2/structures/inventory.h>
#include <common/autopickup_lootfilter.h>

#include <common/ini.h>
#include <DllNotify.h>
#include <D2Template.h>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

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

static const uint32_t m_nCountKeys = 10;

static uint32_t m_nCountItemListAll = 0;
static char m_aacItemList[m_nCountKeys][MAX_STRING_LENGHT] = { 0 };
static char m_acItemListAll[MAX_STRING_LENGHT * m_nCountKeys] = { 0 };
static char m_acItemListAllTemp[MAX_STRING_LENGHT * m_nCountKeys] = { 0 };

static uint32_t m_nCountItemTypesAll = 0;
static char m_aacItemTypes[m_nCountKeys][MAX_STRING_LENGHT] = { 0 };
static char m_acItemTypesAll[MAX_STRING_LENGHT * m_nCountKeys] = { 0 };
static char m_acItemTypesAllTemp[MAX_STRING_LENGHT * m_nCountKeys] = { 0 };

static char m_acBuffer[1024] = { 0 };

static item_code* m_stItemList;
static item_type* m_stItemTypes;

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

 public:
  auto_transmute_menu() {
    menu::set_enabled(true);
    menu::set_visible(true);

    //load_xml("d2tweaks\\interfaces\\autotransmute.xml");
    if (DLLBASE_D2EXPRES != 0)
      load_xml("d2tweaks\\interface_d2expres\\autotransmute.xml");
    if (DLLBASE_SGD2FREERES != 0)
      load_xml("d2tweaks\\interface_sgd2freeres\\autotransmute.xml");
    if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
      load_xml("d2tweaks\\interface_vanilla\\autotransmute.xml");

    m_buttons_img = singleton<common::asset_manager>::instance().get_mpq_file("d2tweaks\\assets\\buttons", common::MPQ_FILE_TYPE_DC6);
    m_auto_transmute_btn = get_button("m_auto_transmute_btn", std::bind(&auto_transmute_menu::auto_transmute_click, this));
  }

  void draw() override {
    if (m_bToggleTransmute) {
      //d2_client::print_chat(L"AUTO TRANSMUTE ON", 1);
      m_auto_transmute_btn->set_current_frame(5); // index of frame in buttons.dc6
    } else {
      //d2_client::print_chat(L"AUTO TRANSMUTE OFF", 2);
      m_auto_transmute_btn->set_current_frame(4);
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
  char szDir[MAX_PATH];
  char szPath[MAX_PATH];
  uint32_t dwLenght = 0;
  const char szConfig[] = "d2tweaks.ini";

  GetCurrentDirectory(MAX_PATH, szDir);
  sprintf_s(szPath, MAX_PATH, "%s\\%s", szDir, szConfig);
  CIni config(szPath);

  // const char szRecipes[] = "d2tweaks.recipes.ini";
  // snprintf(szPath, MAX_PATH, "%s\\%s", szDir, szRecipes);
  // CIni recipes(szPath);
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
  // for (char* pSection = pszBuffer; pSection[0];
  //      pSection = &pSection[strlen(pSection) + 1]) {
  // }

  if (config.GetInt("modules", "AutoTransmute", 1) != FALSE) {
    m_nDelayFrames = config.GetUInt("AutoTransmute", "DelayInFrames", 10);
    m_nTransmuteSound = config.GetUInt("AutoTransmute", "EnableTransmuteSound", 0);

    for (uint32_t i = 0; i < m_nCountKeys; i++) {
      sprintf_s(m_acBuffer, sizeof(m_acBuffer), "ItemList%d", i + 1);
      dwLenght = config.GetString("AutoTransmute", m_acBuffer, m_aacItemList[i], MAX_STRING_LENGHT - 1);
      if (dwLenght != 0) {
        lstrcat(m_acItemListAll, m_aacItemList[i]);
        lstrcat(m_acItemListAll, "|");
      }
    }

    for (uint32_t i = 0; i < m_nCountKeys; i++) {
      sprintf_s(m_acBuffer, sizeof(m_acBuffer), "ItemTypeList%d", i + 1);
      dwLenght = config.GetString("AutoTransmute", m_acBuffer, m_aacItemTypes[i], MAX_STRING_LENGHT - 1);
      if (dwLenght != 0) {
        lstrcat(m_acItemTypesAll, m_aacItemTypes[i]);
        lstrcat(m_acItemTypesAll, "|");
      }
    }

    /////// Parse ItemCode
    dwLenght = lstrlen(m_acItemListAll);
    memcpy(m_acItemListAllTemp, m_acItemListAll, dwLenght + 1);
    // Count the total number of all items
    char* token_string_itemcode = strtok(m_acItemListAllTemp, " ,|");
    while (token_string_itemcode) {
      m_nCountItemListAll++;
      token_string_itemcode = strtok(NULL, " ,|");
    }

    m_stItemList = (item_code*)malloc(m_nCountItemListAll * sizeof(item_code));
    memset(m_stItemList, 0, m_nCountItemListAll * sizeof(item_code));

    memcpy(m_acItemListAllTemp, m_acItemListAll, dwLenght + 1);
    token_string_itemcode = strtok(m_acItemListAllTemp, " ,|");

    for (uint32_t i = 0; token_string_itemcode != 0; i++) {
      uint32_t cur_string_length = lstrlen(token_string_itemcode);
      m_stItemList[i].code0 = token_string_itemcode[0];
      m_stItemList[i].code1 = token_string_itemcode[1];
      m_stItemList[i].code2 = token_string_itemcode[2];

      if (token_string_itemcode[3] == ':') {
        // first index quality = 1
        for (uint32_t q = 1; q <= 9; q++) {
          m_stItemList[i].qualityinclude[q] = FALSE;
        }

        if (cur_string_length <= 13) { //for example jew:123456789
          // p = 4 is first digit after ':'
          for (uint32_t p = 4; p <= cur_string_length; p++) {
            if (token_string_itemcode[p] == 0) {
              break;
            }
            if (token_string_itemcode[p] >= '0' && token_string_itemcode[p] <= '9') {
              m_stItemList[i].qualityinclude[token_string_itemcode[p] - 0x30] = TRUE;
            }
          }
        }
      }

      if (token_string_itemcode[3] == '-') {
        // first index quality = 1
        for (uint32_t q = 1; q <= 9; q++) {
          m_stItemList[i].qualityinclude[q] = TRUE;
        }

        if (cur_string_length <= 13) { //for example jew:123456789
          // p = 4 is first digit after '-'
          for (uint32_t p = 4; p <= cur_string_length; p++) {
            if (token_string_itemcode[p] == 0) {
              break;
            }
            if (token_string_itemcode[p] >= '0' && token_string_itemcode[p] <= '9') {
              m_stItemList[i].qualityinclude[token_string_itemcode[p] - 0x30] = FALSE;
            }
          }
        }
      }

      if (token_string_itemcode[3] != '-' && token_string_itemcode[3] != ':') {
        // first index quality = 1
        for (uint32_t q = 1; q <= 9; q++) {
          m_stItemList[i].qualityinclude[q] = TRUE;
        }
      }

      token_string_itemcode = strtok(NULL, " ,|");
    }

    /////// Parse ItemType
    dwLenght = lstrlen(m_acItemTypesAll);
    memcpy(m_acItemTypesAllTemp, m_acItemTypesAll, dwLenght + 1);
    char* token_string_itemtype_code = strtok(m_acItemTypesAllTemp, ",|");
    while (token_string_itemtype_code) {
      m_nCountItemTypesAll++;
      token_string_itemtype_code = strtok(NULL, ",|");
    }

    m_stItemTypes = (item_type*)malloc(m_nCountItemTypesAll * sizeof(item_type));
    memset(m_stItemTypes, 0, m_nCountItemTypesAll * sizeof(item_type));

    memcpy(m_acItemTypesAllTemp, m_acItemTypesAll, dwLenght + 1);
    token_string_itemtype_code = strtok(m_acItemTypesAllTemp, ",|");
    for (uint32_t i = 0; token_string_itemtype_code != 0; i++) {
      uint32_t cur_itemtypes_string_length = lstrlen(token_string_itemtype_code);
      //m_stItemTypes[i].type0 = token_string_itemtype_code[0];
      //m_stItemTypes[i].type1 = token_string_itemtype_code[1];
      //m_stItemTypes[i].type2 = token_string_itemtype_code[2];
      //m_stItemTypes[i].type3 = token_string_itemtype_code[3];

      m_stItemTypes[i].dwtype = *(DWORD*)token_string_itemtype_code;

      if (token_string_itemtype_code[4] == ':') {
        // first index quality = 1
        for (uint32_t q = 1; q <= 9; q++) {
          m_stItemTypes[i].qualityinclude[q] = FALSE;
        }

        if (cur_itemtypes_string_length <= 14) { //for example tors:123456789
          // p = 5 is first digit after ':'
          for (uint32_t p = 5; p <= cur_itemtypes_string_length; p++) {
            if (token_string_itemtype_code[p] == 0) {
              break;
            }
            if (token_string_itemtype_code[p] >= '0' && token_string_itemtype_code[p] <= '9') {
              m_stItemTypes[i].qualityinclude[token_string_itemtype_code[p] - 0x30] = TRUE;
            }
          }
        }
      }

      if (token_string_itemtype_code[4] == '-') {
        // first index quality = 1
        for (uint32_t q = 1; q <= 9; q++) {
          m_stItemTypes[i].qualityinclude[q] = TRUE;
        }

        if (cur_itemtypes_string_length <= 14) { //for example armo:123456789
          // p = 5 is first digit after '-'
          for (uint32_t p = 5; p <= cur_itemtypes_string_length; p++) {
            if (token_string_itemtype_code[p] == 0) {
              break;
            }
            if (token_string_itemtype_code[p] >= '0' && token_string_itemtype_code[p] <= '9') {
              m_stItemTypes[i].qualityinclude[token_string_itemtype_code[p] - 0x30] = FALSE;
            }
          }
        }
      }

      if (token_string_itemtype_code[4] != '-' && token_string_itemtype_code[4] != ':') {
        // first index quality = 1
        for (uint32_t q = 1; q <= 9; q++) {
          m_stItemTypes[i].qualityinclude[q] = TRUE;
        }
      }

      token_string_itemtype_code = strtok(NULL, ",|");
    }

    if (m_nTransmuteSound == false) {
      hooking::hook(d2_client::get_base() + 0xB5820,
                    hook_play_sound,
                    reinterpret_cast<void**>(&fn_hook_play_sound));
    }

    hooking::hook(d2_client::get_base() + 0xB528,
                  hook_game_end_asm,
                  reinterpret_cast<void**>(&fn_hook_game_end));

    singleton<ui::ui_manager>::instance().add_menu(new auto_transmute_menu());
    singleton<client>::instance().register_tick_handler(this);
    singleton<client>::instance().register_packet_handler(common::message_types_t::MESSAGE_TYPE_TRANSMUTE, this);
  }
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

        for (uint32_t i = 0; i < m_nCountItemTypesAll; i++) {
          for (uint32_t count = 0; count < index_arr_itemtype; count++) {
            if (*(DWORD*)arr_itemtype_codestr_equivstr[count] == (DWORD)m_stItemTypes[i].dwtype) {
              if (m_stItemTypes[i].qualityinclude[quality] == TRUE) {
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

        for (uint32_t i = 0; i < m_nCountItemListAll; i++) {
          if (record->string_code[0] == m_stItemList[i].code0 &&
            record->string_code[1] == m_stItemList[i].code1 &&
            record->string_code[2] == m_stItemList[i].code2) {
            if (m_stItemList[i].qualityinclude[quality] == TRUE) {
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
  static auto& instance = singleton<client>::instance();
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
