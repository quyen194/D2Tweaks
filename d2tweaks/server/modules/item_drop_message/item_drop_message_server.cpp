
#include <Windows.h>

#include <spdlog/spdlog.h>

#include "common/autopickup_lootfilter.h"
#include "common/file_ini.h"
#include "common/strings.h"

#include "diablo2/structures/data/item_types_line.h"
#include "diablo2/structures/data/items_line.h"
#include "diablo2/structures/player_data.h"
#include "diablo2/structures/room.h"
#include "diablo2/structures/unit.h"

#include "diablo2/d2common.h"
#include "diablo2/d2game.h"
#include "diablo2/d2lang.h"

#include "d2tweaks/common/asset_manager.h"

#include "d2tweaks/server/server.h"
#include "d2tweaks/server/modules/item_drop_message/item_drop_message_server.h"


using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace server {
namespace modules {

MODULE_INIT(item_drop_message)

static std::vector<ItemCode> m_stItemList;
static std::vector<ItemType> m_stItemTypes;

void item_drop_message::init() {
  FileIni config(common::get_config_path());
  uint32_t dwLenght = 0;

  if (!config.Int("modules", "ItemDropMessage", 1)) {
    return;
  }

  std::string m_acItemListAll;
  std::string m_acItemTypesAll;

  std::string key;
  uint32_t key_count;
  std::string buffer;

  key_count = config.Int("ItemDropMessage", "ItemListCount", 10);
  for (uint32_t i = 0; i < key_count; i++) {
    key = utils::string_format("ItemList%d", i + 1);
    buffer = config.String("ItemDropMessage", key, MAX_STRING_LENGHT - 1);
    if (!buffer.empty()) {
      if (m_acItemListAll.empty()) {
        m_acItemListAll += buffer;
      } else {
        m_acItemListAll += "|";
        m_acItemListAll += buffer;
      }
    }
  }

  key_count = config.Int("ItemDropMessage", "ItemTypeListCount", 10);
  for (uint32_t i = 0; i < key_count; i++) {
    key = utils::string_format("ItemTypeList%d", i + 1);
    buffer = config.String("ItemDropMessage", key, MAX_STRING_LENGHT - 1);
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

  // server::instance().register_tick_handler(this);
  server::instance().register_packet_handler(common::MESSAGE_TYPE_ITEM_DROPPED_INFO, this);
}

void item_drop_message::tick(Game* game, Unit* unit) {
  static common::item_pickup_info_sc packet;
  static auto& instance = server::instance();
  if (!game || !unit)
    return;

  if (unit->type != unit_type_t::UNIT_TYPE_PLAYER)
    return;
}

bool item_drop_message::handle_packet(Game* game,
                                      Unit* player,
                                      common::packet_header* packet) {
  const auto income_item_dropped_packet =
      static_cast<common::item_dropped_info_cs*>(packet);
  static common::item_dropped_info_sc response_item_dropped_packet;

  switch (income_item_dropped_packet->message_type) {
  case common::message_types_t::MESSAGE_TYPE_ITEM_DROPPED_INFO:
      auto current_unit =
          d2_game::get_server_unit(game,
                                   unit_type_t::UNIT_TYPE_ITEM,
                                   income_item_dropped_packet->item_id);
    if (current_unit != 0) {
      if (current_unit->type == unit_type_t::UNIT_TYPE_ITEM) {
        if (current_unit->item_data != 0) {
          auto record = d2_common::get_item_record(current_unit->data_record_index);
          uint32_t quality = d2_common::get_item_quality(current_unit);
          wchar_t* string_wc = d2_lang::get_string_from_index(record->name_str);
          static char string_mb[260] = { 0 };

          const auto itemtype_record = d2_common::get_item_type_record(record->type);
          auto itemtype_record_equiv1 = d2_common::get_item_type_record(itemtype_record->equiv1);
          auto itemtype_record_equiv2 = d2_common::get_item_type_record(itemtype_record->equiv2);

          wcstombs(string_mb, string_wc, 256);

          memset(response_item_dropped_packet.arr_itemtype_codestr_equivstr, 0, sizeof response_item_dropped_packet.arr_itemtype_codestr_equivstr);
          // Item type code is always the first element in the array
          *(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[0] = *(DWORD*)itemtype_record->code;
          // Index of the second code in the array
          uint32_t index_arr_itemtype = 1;

          if (itemtype_record_equiv1) {
            //
            if (*(DWORD*)itemtype_record_equiv1->code != 0x20202020) {
              // Save the first previously obtained equiv1
              *(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv1->code;
              index_arr_itemtype++;
              // Expand all equiv1 into the array
              for (index_arr_itemtype; itemtype_record_equiv1->equiv1 != 0; index_arr_itemtype++) {
                // Get the next one
                itemtype_record_equiv1 = d2_common::get_item_type_record(itemtype_record_equiv1->equiv1);
                if (*(DWORD*)itemtype_record_equiv1->code != 0x20202020) {
                  *(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv1->code;
                }
                else break;
              }
            }
          }

          if (itemtype_record_equiv2) {
            if (*(DWORD*)itemtype_record_equiv2->code != 0x20202020) {
              // Save the first previously obtained equiv2
              *(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv2->code;
              index_arr_itemtype++;
              // Expand all equiv2 into the array
              for (index_arr_itemtype; itemtype_record_equiv2->equiv2 != 0; index_arr_itemtype++) {
                // Get the next equiv2
                itemtype_record_equiv2 = d2_common::get_item_type_record(itemtype_record_equiv2->equiv2);
                if (*(DWORD*)itemtype_record_equiv2->code != 0x20202020) {
                  *(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv2->code;
                }
                else break;
              }
            }
          }

          if (GetKeyState(VK_SCROLL) != 0) {
            response_item_dropped_packet.item = income_item_dropped_packet->item_id;
            response_item_dropped_packet.code[0] = record->string_code[0];
            response_item_dropped_packet.code[1] = record->string_code[1];
            response_item_dropped_packet.code[2] = record->string_code[2];

            // total number of entries in the itemtypes array
            response_item_dropped_packet.index_arr_itemtype = index_arr_itemtype;

            response_item_dropped_packet.quality = quality;
            response_item_dropped_packet.showthis = TRUE;

            memcpy(response_item_dropped_packet.namestr, string_mb, 128);
            // spdlog::debug("[item_drop_message_s] Code={} Type.Code={} Type.Equiv1={} Type.Equiv2={}", response_item_dropped_packet.code, response_item_dropped_packet.itemtype_code, response_item_dropped_packet.itemtype_equiv1, response_item_dropped_packet.itemtype_equiv2);
            server::instance().send_packet(player->player_data->net_client, &response_item_dropped_packet, sizeof response_item_dropped_packet);
            break;
          }

          for (uint32_t i = 0; i < m_stItemTypes.size(); i++) {
            for (uint32_t count = 0; count < index_arr_itemtype; count++) {
              if (*(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[count] == (DWORD)m_stItemTypes[i].dwtype) {
                if (m_stItemTypes[i].qualityinclude[quality] == TRUE) {
                  response_item_dropped_packet.item = income_item_dropped_packet->item_id;
                  response_item_dropped_packet.code[0] = record->string_code[0];
                  response_item_dropped_packet.code[1] = record->string_code[1];
                  response_item_dropped_packet.code[2] = record->string_code[2];

                  response_item_dropped_packet.index_arr_itemtype = index_arr_itemtype;
                  response_item_dropped_packet.quality = quality;
                  response_item_dropped_packet.showthis = TRUE;

                  memcpy(response_item_dropped_packet.namestr, string_mb, 128);
                  //spdlog::debug("[item_drop_message_s_itemtypes] Code={} Type.Code={} Type.Equiv1={} Type.Equiv2={}", response_item_dropped_packet.code, response_item_dropped_packet.itemtype_code, response_item_dropped_packet.itemtype_equiv1, response_item_dropped_packet.itemtype_equiv2);
                  server::instance().send_packet(player->player_data->net_client, &response_item_dropped_packet, sizeof response_item_dropped_packet);
                  goto L1;
                } else {
                  response_item_dropped_packet.showthis = FALSE;
                }
              }
            }
          }

          for (uint32_t i = 0; i < m_stItemList.size(); i++) {
            if (record->string_code[0] == m_stItemList[i].code0 &&
                record->string_code[1] == m_stItemList[i].code1 &&
                record->string_code[2] == m_stItemList[i].code2) {
              if (m_stItemList[i].qualityinclude[quality] == TRUE) {
                response_item_dropped_packet.item = income_item_dropped_packet->item_id;
                response_item_dropped_packet.code[0] = record->string_code[0];
                response_item_dropped_packet.code[1] = record->string_code[1];
                response_item_dropped_packet.code[2] = record->string_code[2];

                response_item_dropped_packet.index_arr_itemtype = index_arr_itemtype;
                response_item_dropped_packet.quality = quality;
                response_item_dropped_packet.showthis = TRUE;

                memcpy(response_item_dropped_packet.namestr, string_mb, 128);
                // spdlog::debug(
                //     "[item_drop_message_s] Code={} Type.Code={} Type.Equiv1={} Type.Equiv2={}",
                //     response_item_dropped_packet.code,
                //     response_item_dropped_packet.itemtype_code,
                //     response_item_dropped_packet.itemtype_equiv1,
                //     response_item_dropped_packet.itemtype_equiv2);
                server::instance().send_packet(
                    player->player_data->net_client,
                    &response_item_dropped_packet,
                    sizeof response_item_dropped_packet);
              }
            } else {
              response_item_dropped_packet.showthis = FALSE;
            }
          }

        L1:;
        }
      }
    }
    break;
  }

  return true;
}

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
