#include <D2Template.h>
#include <DllNotify.h>
#include <d2tweaks/client/client.h>
#include <d2tweaks/client/modules/autosort/autosort_client.h>
#include <d2tweaks/common/asset_manager.h>
#include <d2tweaks/common/common.h>
#include <d2tweaks/common/protocol.h>
#include <d2tweaks/ui/controls/button.h>
#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>
#include <diablo2/d2client.h>
#include <diablo2/d2cmp.h>
#include <diablo2/d2common.h>
#include <diablo2/d2gfx.h>
#include <diablo2/d2win.h>

#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/player_data.h>
#include <diablo2/structures/unit.h>

#include <spdlog/spdlog.h>
#include <time.h>

#include <algorithm>
#include <cmath>
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

namespace d2_tweaks {
namespace client {
namespace modules {

enum ColorEnum {
  RED = ui_color_t::UI_COLOR_RED,
  LIGHT_GREEN = ui_color_t::UI_COLOR_LIGHT_GREEN,
  BLUE = ui_color_t::UI_COLOR_BLUE,
  DARK_GOLD = ui_color_t::UI_COLOR_DARK_GOLD,
  GREY = ui_color_t::UI_COLOR_GREY,
  BLACK = ui_color_t::UI_COLOR_BLACK,
  GOLD = ui_color_t::UI_COLOR_GOLD,
  ORANGE = ui_color_t::UI_COLOR_ORANGE,
  YELLOW = ui_color_t::UI_COLOR_YELLOW,
  DARK_GREEN = ui_color_t::UI_COLOR_DARK_GREEN,
  PURPLE = ui_color_t::UI_COLOR_PURPLE,
  GREEN = ui_color_t::UI_COLOR_GREEN,
  WHITE = ui_color_t::UI_COLOR_WHITE,
  BLACK2 = ui_color_t::UI_COLOR_BLACK2,
  DARK_WHITE = ui_color_t::UI_COLOR_DARK_WHITE,
  LIGHT_GREY = ui_color_t::UI_COLOR_LIGHT_GREY
};

class inventory_sort_menu : public ui::menu {
  common::asset* m_buttons_img;

  button* m_sort_inventory_btn;
  button* m_sort_stash_btn;
  button* m_sort_cube_btn;

 public:
  inventory_sort_menu() {
    menu::set_enabled(true);
    menu::set_visible(true);

    // load_xml("d2tweaks\\interfaces\\autosort.xml");
    if (DLLBASE_D2EXPRES != 0)
      load_xml("d2tweaks\\interface_d2expres\\autosort.xml");
    if (DLLBASE_SGD2FREERES != 0)
      load_xml("d2tweaks\\interface_sgd2freeres\\autosort.xml");
    if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
      load_xml("d2tweaks\\interface_vanilla\\autosort.xml");

    m_buttons_img = singleton<common::asset_manager>::instance().get_mpq_file(
        "d2tweaks\\assets\\buttons", common::MPQ_FILE_TYPE_DC6);

    m_sort_inventory_btn =
        get_button("m_sort_inventory_btn",
                   std::bind(&inventory_sort_menu::sort_inventory_click, this));
    m_sort_stash_btn =
        get_button("m_sort_stash_btn",
                   std::bind(&inventory_sort_menu::sort_stash_click, this));
    m_sort_cube_btn =
        get_button("m_sort_cube_btn",
                   std::bind(&inventory_sort_menu::sort_cube_click, this));
  }

  // Function to draw a filled rectangle using GDI
  void DrawFilledRect(
      HWND hwnd, int left, int top, int right, int bottom, COLORREF color) {
    HDC hdc = GetDC(hwnd);
    HBRUSH hBrush = CreateSolidBrush(color);
    RECT rect = {left, top, right, bottom};
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    ReleaseDC(hwnd, hdc);
  }

  ULONGLONG lastSendTime = GetTickCount64();

  // Function to split a string into words
  std::vector<std::string> split(const std::string& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (getline(ss, item, delim)) {
      tokens.push_back(item);
    }
    return tokens;
  }

  std::wstring stringToWstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
  }

  // Function to justify a line of text
  void justifyLine(const std::vector<std::string>& words,
                   int start,
                   int end,
                   int maxLineLength,
                   int& xPos,
                   int yPos,
                   int padding) {
    int lineLength = 0;
    for (int i = start; i <= end; i++) {
      lineLength += words[i].length() *
                    8;  // Assuming each character has a width of 8 pixels
    }

    int numSpaces = end - start;
    int extraSpaces = maxLineLength - lineLength - padding * 2;

    int spaceWidth = 8;
    int extraSpacePerGap = numSpaces > 0 ? extraSpaces / numSpaces : 0;
    int remainingSpaces = numSpaces > 0 ? extraSpaces % numSpaces : 0;

    for (int i = start; i <= end; i++) {
      d2_win::draw_boxed_text(
          const_cast<wchar_t*>(stringToWstring(words[i]).c_str()),
          xPos + padding,
          yPos,
          0,
          0,
          UI_COLOR_WHITE);  // Assuming paletteIndex, transTbl, and
                            // color are 0
      xPos += words[i].length() * 8 + spaceWidth + extraSpacePerGap + padding;

      if (remainingSpaces > 0) {
        xPos += 1;  // Add an extra pixel for the remaining spaces
        remainingSpaces--;
      }
    }
  }

  // Function to draw justified text inside a box with padding
  void drawJustifiedTextInBox(const std::string& text,
                              int boxX,
                              int boxY,
                              int boxWidth,
                              int boxHeight,
                              int padding) {
    std::vector<std::string> words = split(text, ' ');
    unsigned int maxLineLength = boxWidth;
    int xPos = boxX + padding;
    int yPos = boxY + padding;

    for (size_t i = 0; i < words.size();) {
      unsigned int start = i;
      unsigned int end = i;
      unsigned int lineLength = 0;

      // Find the end index for the current line
      while (end < words.size() &&
             lineLength + words[end].length() <= maxLineLength) {
        lineLength += words[end].length();
        if (end > start) {
          lineLength++;  // Add space width
        }
        end++;
      }

      justifyLine(words, start, end - 1, maxLineLength, xPos, yPos, padding);
      yPos += 16;  // Assuming each line is 16 pixels tall
      xPos = boxX + padding;
      i = end;
    }
  }

  void draw() override {
    const auto player = d2_client::get_local_player();
    auto name = player->player_data->name;

    d2_win::set_current_font(UI_FONT_16);  // Set font to FONT16

    if (!should_draw()) {
      m_sort_inventory_btn->set_enabled(false);
      m_sort_inventory_btn->set_visible(false);

      m_sort_stash_btn->set_enabled(false);
      m_sort_stash_btn->set_visible(false);

      m_sort_cube_btn->set_enabled(false);
      m_sort_cube_btn->set_visible(false);

      return;
    }

    m_sort_inventory_btn->set_enabled(true);
    m_sort_inventory_btn->set_visible(true);

    m_sort_stash_btn->set_enabled(
        d2_client::get_ui_window_state(UI_WINDOW_STASH));
    m_sort_stash_btn->set_visible(
        d2_client::get_ui_window_state(UI_WINDOW_STASH));

    m_sort_cube_btn->set_enabled(
        d2_client::get_ui_window_state(UI_WINDOW_CUBE));
    m_sort_cube_btn->set_visible(
        d2_client::get_ui_window_state(UI_WINDOW_CUBE));

    menu::draw();
  }

 private:
  static bool should_draw() {
    return d2_client::get_ui_window_state(UI_WINDOW_INVENTORY) ||
           d2_client::get_ui_window_state(UI_WINDOW_STASH) ||
           d2_client::get_ui_window_state(UI_WINDOW_CUBE) ||
           d2_client::get_ui_window_state(UI_WINDOW_NPCSHOP);
  }

  button* get_button(const std::string& name,
                     const std::function<void()>& onClick) {
    auto result = static_cast<button*>(get_control(name));

    result->set_on_click(onClick);

    result->set_enabled(false);
    result->set_visible(false);

    return result;
  }

  void sort_inventory_click() { request_sort(0x00); }

  void sort_stash_click() { request_sort(0x04); }

  void sort_cube_click() { request_sort(0x03); }

  void request_sort(const uint8_t page) {
    static common::inventory_sort_cs packet;

    const auto player = d2_client::get_local_player();

    std::vector<unit*> items;

    for (auto item = player->inventory->first_item; item != nullptr;
         item = item->item_data->pt_next_item) {
      if (item->item_data->page == page)
        items.push_back(item);
    }

    for (auto item : items)
      d2_common::inv_remove_item(player->inventory, item);

    packet.page = page;
    d2_client::send_to_server(&packet, sizeof packet);
  }
};

MODULE_INIT(autosort)

void autosort::init_early() {}

void autosort::init() {
  const char* config_path = common::get_config_path();

  if (GetPrivateProfileInt("modules", "Autosort", 1, config_path)) {
    singleton<ui::ui_manager>::instance().add_menu(new inventory_sort_menu());
    singleton<client>::instance().register_packet_handler(
        common::MESSAGE_TYPE_INVENTORY_SORT, this);
  }
}

void autosort::handle_packet(common::packet_header* packet) {
  static auto& instance = singleton<client>::instance();
  const auto inventorySort = static_cast<common::inventory_sort_sc*>(packet);

  const auto item = instance.get_client_unit(
      0x04, inventorySort->guid);  // 0x03 -> 0x04 - item

  if (item == nullptr)
    return;
  ;

  const auto player = d2_client::get_local_player();

  // Last parameter is some boolean
  const auto inventoryIndex = d2_common::get_inventory_index(
      player, inventorySort->page, d2_client::is_lod());

  item->item_data->page = inventorySort->page;

  d2_common::inv_add_item(player->inventory,
                          item,
                          inventorySort->tx,
                          inventorySort->ty,
                          inventoryIndex,
                          true,
                          item->item_data->page);
  d2_common::inv_update_item(player->inventory, item, true);
}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
