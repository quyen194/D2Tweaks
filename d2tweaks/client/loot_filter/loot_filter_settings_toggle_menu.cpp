
#include <Windows.h>

#include <CommCtrl.h> // Include for edit control
#include <shellapi.h> // For ShellExecute
#include <time.h>

#include <algorithm>
#include <cmath>
#include <cstdlib> // For system function
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip> // For std::setw
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <stdexcept> // For std::invalid_argument
#include <string>
#include <unordered_map>
#include <vector>

#include <DllNotify.h>
#include <D2Template.h>

#include <spdlog/spdlog.h>

#include "common/file_ini.h"
#include "common/hooking.h"

#include "diablo2/structures/data/item_types_line.h"
#include "diablo2/structures/data/items_line.h"
#include "diablo2/structures/game.h"
#include "diablo2/structures/inventory.h"
#include "diablo2/structures/item_data.h"
#include "diablo2/structures/path.h"
#include "diablo2/structures/player_data.h"
#include "diablo2/structures/unit.h"

#include "diablo2/d2client.h"
#include "diablo2/d2cmp.h"
#include "diablo2/d2common.h"
#include "diablo2/d2gfx.h"
#include "diablo2/d2win.h"

#include "d2tweaks/common/asset_manager.h"
#include "d2tweaks/common/common.h"
#include "d2tweaks/common/protocol.h"

#include "d2tweaks/ui/controls/button.h"
#include "d2tweaks/ui/menu.h"
#include "d2tweaks/ui/ui_manager.h"
#include "d2tweaks/ui/ui_utils.h"

#include "d2tweaks/client/client.h"
#include "d2tweaks/client/loot_filter/loot_filter_settings_toggle_menu.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

bool m_stats_enabled = true;
bool m_help_enabled = false;
bool m_cube_enabled = false;
bool m_stash_enabled = true;

namespace d2_tweaks {
namespace client {

#pragma pack(push, 1)

loot_filter_settings_toggle_menu::loot_filter_settings_toggle_menu(token) {
  FileIni config(common::get_config_path());

  m_show = false;

  menu::set_enabled(true);
  menu::set_visible(true);

  // Read the key from the ./d2tweaks.ini file from [ExPanel] section, key: key
  std::string key_buff = config.String("ExPanel", "key", 1, "E");
  if (!key_buff.empty()) {
    key_buff = toupper(key_buff[0]);
    shortcut_key = key_buff[0];
  }

  // read the url from the ./d2tweaks.ini file
  help_url = config.String("Options", "help_url", 256);

  //load_xml("d2tweaks\\interfaces\\loot_filter_settings_toggle_menu.xml");
  if (DLLBASE_D2EXPRES != 0)
    load_xml("d2tweaks\\interface_d2expres\\loot_filter_settings_toggle_menu.xml");
  if (DLLBASE_SGD2FREERES != 0)
    load_xml("d2tweaks\\interface_sgd2freeres\\loot_filter_settings_toggle_menu.xml");
  if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
    load_xml("d2tweaks\\interface_vanilla\\loot_filter_settings_toggle_menu.xml");

  m_toggle_filter_settings_btn =
      static_cast<button*>(get_control("m_toggle_filter_settings_btn"));
  m_toggle_filter_settings_btn->set_enabled(false);
  m_toggle_filter_settings_btn->set_visible(false);
  m_toggle_filter_settings_btn->set_on_click(std::bind(
      &loot_filter_settings_toggle_menu::toggle_filter_settings_click, this));

  m_filter_settings_menu = ui::Manager::instance().get_menu(
      "loot_filter_settings_menu");

  // toggle stats button
  m_btn_toggle_stats = static_cast<button*>(get_control("m_toggle_stats"));
  m_btn_toggle_stats->set_enabled(true);
  m_btn_toggle_stats->set_visible(true);
  m_btn_toggle_stats->set_on_click(std::bind(
      &loot_filter_settings_toggle_menu::toggle_stats_settings_click, this));

  if (!config.Int("LootFilter", "ShowButtonToggleStats", 0)) {
    m_btn_toggle_stats->set_enabled(false);
    m_btn_toggle_stats->set_visible(false);
  }

  // help button
  m_btn_toggle_help = static_cast<button*>(get_control("m_toggle_help"));
  m_btn_toggle_help->set_enabled(true);
  m_btn_toggle_help->set_visible(true);
  m_btn_toggle_help->set_on_click(
      std::bind(&loot_filter_settings_toggle_menu::toggle_help_click, this));

  if (!config.Int("LootFilter", "ShowButtonHelp", 0)) {
    m_btn_toggle_help->set_enabled(false);
    m_btn_toggle_help->set_visible(false);
  }

  // toggle open/close cube button
  m_btn_toggle_cube = static_cast<button*>(get_control("m_toggle_cube"));
  m_btn_toggle_cube->set_enabled(true);
  m_btn_toggle_cube->set_visible(true);
  m_btn_toggle_cube->set_on_click(
      std::bind(&loot_filter_settings_toggle_menu::toggle_cube_click, this));

  auto player = d2_client::get_local_player();

  //iterate over all items in player inventory
  for (auto item = player->inventory->first_item; item != nullptr;
       item = item->item_data->pt_next_item) {
    const auto record = d2_common::get_item_record(item->data_record_index);
    char* normCode1 = record->string_code;
    if (strncmp(normCode1, "box", 3) == 0) {
      m_btn_toggle_cube->set_enabled(true);
      m_btn_toggle_cube->set_visible(true);
      break;
    }
    else {
      m_btn_toggle_cube->set_enabled(false);
      m_btn_toggle_cube->set_visible(false);
    }
  }

  if (!config.Int("LootFilter", "ShowButtonToggleCube", 0)) {
    m_btn_toggle_cube->set_enabled(false);
    m_btn_toggle_cube->set_visible(false);
  }

  // toggle open/close stash button
  m_btn_toggle_stash = static_cast<button*>(get_control("m_toggle_stash"));
  m_btn_toggle_stash->set_enabled(true);
  m_btn_toggle_stash->set_visible(true);
  m_btn_toggle_stash->set_on_click(
      std::bind(&loot_filter_settings_toggle_menu::toggle_stash_click, this));

  // iterate over all items in player inventory
  for (auto item = player->inventory->first_item; item != nullptr;
       item = item->item_data->pt_next_item) {
    const auto record = d2_common::get_item_record(item->data_record_index);
    char* normCode1 = record->string_code;
    if (strncmp(normCode1, "st0", 3) == 0) {
      m_btn_toggle_stash->set_enabled(true);
      m_btn_toggle_stash->set_visible(true);
      break;
    }
    else {
      m_btn_toggle_stash->set_enabled(false);
      m_btn_toggle_stash->set_visible(false);
    }
  }

  if (!config.Int("LootFilter", "ShowButtonToggleStash", 0)) {
    m_btn_toggle_stash->set_enabled(false);
    m_btn_toggle_stash->set_visible(false);
  }
}

void loot_filter_settings_toggle_menu::toggle_filter_settings_click() {
  m_show = !m_show;
  m_stats_enabled = !m_stats_enabled;

  m_filter_settings_menu->set_enabled(m_show);
  m_filter_settings_menu->set_visible(m_show);

  // m_btn_toggle_stats->set_enabled(true);
  // m_btn_toggle_stats->set_visible(true);

  // toggle_cube_click();
}

void loot_filter_settings_toggle_menu::toggle_stats_settings_click() {
  m_stats_enabled = !m_stats_enabled;
}

void loot_filter_settings_toggle_menu::toggle_cube_click() {
  m_cube_enabled = !m_cube_enabled;
  if (!d2_client::get_ui_window_state(UI_WINDOW_CUBE)) {
    const auto player = d2_client::get_local_player();
    int32_t boxGuid = 0;
    uint32_t boxX = 0;
    uint32_t boxY = 0;
    Unit* box{};
    for (auto item = player->inventory->first_item; item != nullptr;
         item = item->item_data->pt_next_item) {
      const auto record = d2_common::get_item_record(item->data_record_index);
      char* normCode1 = record->string_code;
      if (strncmp(normCode1, "box", 3) == 0) {
        box = item;
        boxGuid = box->guid;
        boxX = player->path->mapx;
        boxY = player->path->mapy;
      }
    }
    struct D2GSPacketClt20 {
      uint8_t PacketId;   // 0x01
      // uint8_t MsgID;      // 0x02
      uint32_t guid;      // 0x06
      uint32_t tx;        // 0x07
      uint32_t ty;        // 0x09
    };
    D2GSPacketClt20 packet;
    packet.PacketId = 0x20;
    packet.guid = boxGuid;
    packet.tx = boxX;
    packet.ty = boxY;
    d2_client::send_to_server(&packet, sizeof packet);
  }
  else {
    d2_client::set_ui_toggle(0x1A, 1, FALSE);
    // send to server7 to close cube packet 0x4F
    d2_client::send_to_server_7(0x4F, 0x17, 0, 0);
  }
}

void loot_filter_settings_toggle_menu::toggle_stash_click() {
  m_stash_enabled = !m_stash_enabled;
  if (!d2_client::get_ui_window_state(UI_WINDOW_STASH)) {
    const auto player = d2_client::get_local_player();
    int32_t st0Guid = 0;
    uint32_t st0X = 0;
    uint32_t st0Y = 0;
    Unit* box{};
    for (auto item = player->inventory->first_item; item != nullptr;
         item = item->item_data->pt_next_item) {
      const auto record = d2_common::get_item_record(item->data_record_index);
      char* st0Code = record->string_code;
      if (strncmp(st0Code, "st0", 3) == 0) {
        box = item;
        st0Guid = box->guid;
        st0X = player->path->mapx;
        st0Y = player->path->mapy;
      }
    }
    struct D2GSPacketClt20 {
      uint8_t PacketId;  // 0x01
      uint32_t guid;     // 0x06
      uint32_t tx;       // 0x07
      uint32_t ty;       // 0x09
    };
    D2GSPacketClt20 packet;
    packet.PacketId = 0x20;
    packet.guid = st0Guid;
    packet.tx = st0X;
    packet.ty = st0Y;
    d2_client::send_to_server(&packet, sizeof packet);

    // SERIALIZED_OBJECT_ON
    // struct D2GSPacketClt20 {
    //   uint8_t PacketId;   // 0x01
    //   uint32_t uk1;       // 0x06
    //   uint32_t uk2;       // 0x06
    // };
    // SERIALIZED_OBJECT_OFF
    //
    // D2GSPacketClt20 packet;
    // packet.PacketId = 0x13;
    // packet.uk1 = 0x02;
    // packet.uk2 = 0x15;
    // d2_client::send_to_server(&packet, sizeof packet);
  }
  else {
    d2_client::set_ui_toggle(0x19, 1, FALSE);
    // send to server7 to close cube packet 0x4F
    d2_client::send_to_server_7(0x4F, 0x17, 0, 0);
  }
}

void loot_filter_settings_toggle_menu::toggle_help_click() {
  //m_help_enabled = !m_help_enabled;
  // Open the default OS browser to the URL

  if (!help_url.empty()) {
    ShellExecute(nullptr, "open", help_url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
  }
}

void loot_filter_settings_toggle_menu::draw() {
  // m_btn_toggle_stats->set_enabled(d2_client::get_ui_window_state(UI_WINDOW_INTERFACE));
  // m_btn_toggle_stats->set_visible(d2_client::get_ui_window_state(UI_WINDOW_INTERFACE));
  m_toggle_filter_settings_btn->set_enabled(d2_client::get_ui_window_state(UI_WINDOW_INTERFACE));
  m_toggle_filter_settings_btn->set_visible(d2_client::get_ui_window_state(UI_WINDOW_INTERFACE));

  menu::draw();
}

bool loot_filter_settings_toggle_menu::key_event(uint32_t key, bool up) {
  if (key == shortcut_key &&
      up &&
      !d2_client::get_ui_window_state(UI_WINDOW_MSGS) &&
      !d2_client::get_ui_window_state(UI_WINDOW_CHAT)) {
    m_show = !m_show;
    m_stats_enabled = !m_stats_enabled;

    m_filter_settings_menu->set_enabled(m_show);
    m_filter_settings_menu->set_visible(m_show);

    m_btn_toggle_stats->set_enabled(true);
    m_btn_toggle_stats->set_visible(true);

    toggle_cube_click();

    return true; // Block the key stroke
  }

  if (key == VK_ESCAPE && m_show) {
    m_show = false;

    m_filter_settings_menu->set_enabled(m_show);
    m_filter_settings_menu->set_visible(m_show);

    toggle_cube_click();

    return true; //block escape key stroke
  }

  return menu::key_event(key, up);
}

#pragma pack(pop)

}  // namespace client
}  // namespace d2_tweaks
