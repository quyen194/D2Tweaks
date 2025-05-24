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
#include <diablo2/structures/data/item_types_line.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/path.h>
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

// Mapping from int to ui_font_t
std::map<int, ui_font_t> fontMap = {
    {0, ui_font_t::UI_FONT_8},
    {1, ui_font_t::UI_FONT_16},
    {2, ui_font_t::UI_FONT_30},
    {3, ui_font_t::UI_FONT_42},
    {4, ui_font_t::UI_FONT_FORMAL10},
    {5, ui_font_t::UI_FONT_FORMAL12},
    {6, ui_font_t::UI_FONT_6},
    {7, ui_font_t::UI_FONT_24},
    {8, ui_font_t::UI_FONT_FORMAL11},
    {9, ui_font_t::UI_FONT_EXOCET10},
    {10, ui_font_t::UI_FONT_RIDICULOUS},
    {11, ui_font_t::UI_FONT_EXOCET8},
    {12, ui_font_t::UI_FONT_REALLYTHELASTSUCKER},
    {13, ui_font_t::UI_FONT_INGAMECHAT}};

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

  // Function to find the Diablo II window handle
  HWND FindDiabloIIWindow() { return FindWindow(NULL, "Diablo II"); }

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

  int statsFont =
      GetPrivateProfileIntA("Options", "statsFont", 0, "./d2tweaks.ini");

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
    auto stats = globalStatsVector;
    int textOffset = 40;  // Initial offset for the first line

    const auto player = d2_client::get_local_player();
    auto name = player->player_data->name;

    // Add all items to vector
    std::vector<unit*> items;
    for (auto item = player->inventory->first_item; item != nullptr;
         item = item->item_data->pt_next_item) {
      const auto record = d2_common::get_item_record(item->data_record_index);
      if (record->type == 104 || record->type == 123) {
        items.push_back(item);
      }
    }

    // Initialize statValue
    int32_t statValue = 0;

    // if (d2_client::get_ui_window_state(UI_WINDOW_STASH)) {
    //   d2_gfx::draw_filled_rect(130, 48, 640, 155, 5, 50);
    // }

    if (m_stats_enabled) {
      for (const auto& stat : stats) {
        int param = stat.param;
        int op = stat.op;
        auto statline = d2_common::get_item_stat_cost_record(stat.stat);
        auto opBase = statline->wOpBase;
        auto opStat = statline->wOpStat[0];
        auto opBaseValue = d2_common::get_stat(
            player, static_cast<unit_stats_t>(opBase), NULL);
        auto opStatValue = d2_common::get_stat(
            player, static_cast<unit_stats_t>(opStat), NULL);

        if (stat.is_item_stat == 0) {
          int32_t statvalue = d2_common::get_stat(
              player, static_cast<unit_stats_t>(stat.stat), NULL);
          int basevalue = 1;
          switch (op) {
            case 0:
              statValue = d2_common::get_stat(
                  player, static_cast<unit_stats_t>(stat.stat), NULL);
              break;
            case 1:  // Formula: opstatbasevalue * statvalue / 100
              statValue = static_cast<int32_t>(opBaseValue) / 100;
              break;
            case 2:  // Formula: (statvalue * basevalue) / (2 ^ param)
              statValue = static_cast<int32_t>((opBaseValue) / pow(op, param));
              break;
            case 3:  // Percentage-based version of op #2
              statValue =
                  static_cast<int32_t>((opBaseValue) / pow(op, param)) / 100;
              break;
            case 4:  // Item-based stat increase
              statValue = static_cast<int32_t>((opBaseValue) / pow(op, param));
              break;
            case 5:  // Percentage-based item increase
              statValue =
                  static_cast<int32_t>((opBaseValue) / pow(op, param)) / 100;
              break;
            case 11:  // Similar to op #1 and #13
              statValue =
                  static_cast<int32_t>((opBaseValue) / pow(op, param)) / 100;
              break;
            case 13:
              statValue =
                  static_cast<int32_t>((opBaseValue) / pow(op, param)) / 100;
              break;
            default:
              statValue = d2_common::get_stat(
                  player, static_cast<unit_stats_t>(stat.stat), NULL);
              break;
          }
        } else {
          for (auto item : items) {
            const auto record =
                d2_common::get_item_record(item->data_record_index);
            if (record->type == stat.item_type_id) {
              statValue = d2_common::get_stat(
                  item, static_cast<unit_stats_t>(stat.stat), NULL);
            }
          }
        }

        // write code to get player name and display it using MessageBox
        const auto player = d2_client::get_local_player();
        auto name = player->player_data->name;

        auto statValueStr = std::to_wstring(statValue);
        std::wstring statText = std::wstring(
            stat.stat_display_string);  // .append(L" " + statValueStr);

        if (!d2_client::get_ui_window_state(UI_WINDOW_STASH) &&
            !d2_client::get_ui_window_state(UI_WINDOW_CHARACTER) &&
            !d2_client::get_ui_window_state(UI_WINDOW_CHAT) &&
            !d2_client::get_ui_window_state(UI_WINDOW_NPCMENU) &&
            !d2_client::get_ui_window_state(UI_WINDOW_MAINMENU) &&
            !d2_client::get_ui_window_state(UI_WINDOW_CONFIG) &&
            !d2_client::get_ui_window_state(UI_WINDOW_NPCSHOP) &&
            !d2_client::get_ui_window_state(UI_WINDOW_ANVIL) &&
            !d2_client::get_ui_window_state(UI_WINDOW_QUEST) &&
            !d2_client::get_ui_window_state(UI_WINDOW_QUESTLOG) &&
            !d2_client::get_ui_window_state(UI_WINDOW_STATUSAREA) &&
            !d2_client::get_ui_window_state(UI_WINDOW_WPMENU) &&
            !d2_client::get_ui_window_state(UI_WINDOW_PARTY) &&
            !d2_client::get_ui_window_state(UI_WINDOW_TRADE) &&
            !d2_client::get_ui_window_state(UI_WINDOW_MSGS) &&
            !d2_client::get_ui_window_state(UI_WINDOW_CUBE) &&
            !d2_client::get_ui_window_state(UI_WINDOW_BELT) &&
            !d2_client::get_ui_window_state(UI_WINDOW_HELP) &&
            !d2_client::get_ui_window_state(UI_WINDOW_MERC) &&
            !d2_client::get_ui_window_state(UI_WINDOW_SCROLL)) {
          // Draw stats
          d2_win::set_current_font(fontMap[statsFont]);  // Set font to FONT16
          d2_win::draw_text(const_cast<wchar_t*>(statText.c_str()),
                            stat.x1,
                            stat.y1 + textOffset,
                            stat.colorStat,
                            0);

          d2_win::set_current_font(fontMap[statsFont]);  // Set font to FONT16
          d2_win::draw_text(const_cast<wchar_t*>(statValueStr.c_str()),
                            stat.x2,
                            stat.y2 + textOffset,
                            stat.colorStatValue,
                            0);

          // auto runeword = d2_common::get_runes_txt_record(169);
          //
          // // Assuming szName is a null-terminated C-string.
          // // Ensure this buffer is large enough to hold the converted string.
          // wchar_t wText[1024];
          // // Buffer for each line size_t convertedChars = 0;
          // wchar_t wLine[128];
          //
          // // Initialize vertical offset
          // int yOffset = 200;
          //
          // // Convert and draw each field
          // mbstowcs_s(&convertedChars, wText, "szName: ", _TRUNCATE);
          // mbstowcs_s(&convertedChars, wLine, runeword->szName, _TRUNCATE);
          // swprintf(wText, L"%s%s", wText, wLine);
          // d2_win::draw_text(wText, 200, yOffset, UI_COLOR_GOLD, 0);
          // yOffset += 14;
          //
          // mbstowcs_s(&convertedChars, wText, "szRuneName: ", _TRUNCATE);
          // mbstowcs_s(&convertedChars, wLine, runeword->szRuneName, _TRUNCATE);
          // swprintf(wText, L"%s%s", wText, wLine);
          // d2_win::draw_text(wText, 200, yOffset, UI_COLOR_GOLD, 0);
          // yOffset += 14;
          //
          // mbstowcs_s(&convertedChars, wText, "nComplete: ", _TRUNCATE);
          // swprintf(wText, L"%s%d", wText, runeword->nComplete);
          // d2_win::draw_text(wText, 200, yOffset, UI_COLOR_GOLD, 0);
          // yOffset += 14;
          //
          // mbstowcs_s(&convertedChars, wText, "nServer: ", _TRUNCATE);
          // swprintf(wText, L"%s%d", wText, runeword->nServer);
          // d2_win::draw_text(wText, 200, yOffset, UI_COLOR_GOLD, 0);
          // yOffset += 14;
          //
          // mbstowcs_s(&convertedChars, wText, "wStringId: ", _TRUNCATE);
          // swprintf(wText, L"%s%d", wText, runeword->wStringId);
          // d2_win::draw_text(wText, 200, yOffset, UI_COLOR_GOLD, 0);
          // yOffset += 14;
          //
          // mbstowcs_s(&convertedChars, wText, "pad0x84: ", _TRUNCATE);
          // swprintf(wText, L"%s%d", wText, runeword->pad0x84);
          // d2_win::draw_text(wText, 200, yOffset, UI_COLOR_GOLD, 0);
          // yOffset += 14;
          //
          // mbstowcs_s(&convertedChars, wText, "wIType: ", _TRUNCATE);
          // for (int i = 0; i < 6; ++i) {
          //   swprintf(wLine, L"%d ", runeword->wIType[i]);
          //   wcscat_s(wText, wLine);
          // }
          // d2_win::draw_text(wText, 200, yOffset, UI_COLOR_GOLD, 0);
          // yOffset += 14;
          //
          // mbstowcs_s(&convertedChars, wText, "wEType: ", _TRUNCATE);
          // for (int i = 0; i < 3; ++i) {
          //   swprintf(wLine, L"%d ", runeword->wEType[i]);
          //   wcscat_s(wText, wLine);
          // }
          // d2_win::draw_text(wText, 200, yOffset, UI_COLOR_GOLD, 0);
          // yOffset += 14;
          //
          // mbstowcs_s(&convertedChars, wText, "nRune: ", _TRUNCATE);
          // for (int i = 0; i < 6; ++i) {
          //   items_line* rItem =
          //       d2_common::get_items_txt_record(runeword->nRune[i]);
          //   auto code = rItem->string_code;
          //
          //   // Ensure code is not null before attempting to use it
          //   if (code) {
          //     // Format code and add to wLine
          //     // swprintf(wLine, L"%ls ", code);
          //
          //     // Concatenate wLine to wText
          //     wcscat_s(wText, wLine);
          //   }
          // }
          // d2_win::draw_text(wText, 200, yOffset, UI_COLOR_GOLD, 0);
          // yOffset += 14;
        }

        // d2_win::draw_boxed_text(const_cast<wchar_t*>(statText.c_str()),
        //                         stat.x1,
        //                         stat.y1 + textOffset,
        //                         1,
        //                         0,
        //                         stat.colorStat);
        // d2_win::draw_boxed_text(const_cast<wchar_t*>(statValueStr.c_str()),
        //                         stat.x2,
        //                         stat.y2 + textOffset,
        //                         1,
        //                         4,
        //                         stat.colorStatValue);
        // d2_win::set_current_font(UI_FONT_16);  // Set font to FONT16
        // d2_cmp::init_gfx_data(&g_gfxdata);
        // d2_gfx::draw_image(&g_gfxdata, 200, 200, 1, 5, 0);
        // // instead try to load direct jpg with gdi
        // // and insetad of loading jpg file,
        // // specify it bb64 encoded and decode it.
      }
    }

    if (m_help_enabled) {
      // const int windowWidth = 1280;
      // const int windowHeight = 768;
      // const int boxWidth = 1000;
      // const int boxHeight = 680;
      // const int boxX = (windowWidth - boxWidth) / 2;
      // const int boxY = (windowHeight - boxHeight) / 2;
      // const std::string helpText =
      //     "This is a sample help screen! You can put help text in here!?";
      // // Draw filled background box
      // d2_gfx::draw_filled_rect(
      //     boxX, boxY, boxX + boxWidth, boxY + boxHeight, 0, 255);
      // // Draw justified text inside the box with padding
      // drawJustifiedTextInBox(helpText, boxX, boxY, boxWidth, boxHeight, 0);
    }

    ui_color_t::UI_COLOR_WHITE;

    // print player health, mana, and stamina bars, lastexp, nextexp, and level
    // Get current HP, Mana, and Stamina along with their maximum values
    int statHP =
        d2_common::get_stat(player, static_cast<unit_stats_t>(6), NULL) / 256;
    int statMaxHP =
        d2_common::get_stat(player, static_cast<unit_stats_t>(7), NULL) / 256;
    int statMana =
        d2_common::get_stat(player, static_cast<unit_stats_t>(8), NULL) / 256;
    int statMaxMana =
        d2_common::get_stat(player, static_cast<unit_stats_t>(9), NULL) / 256;
    int statStamina =
        d2_common::get_stat(player, static_cast<unit_stats_t>(10), NULL) / 256;
    int statMaxStamina =
        d2_common::get_stat(player, static_cast<unit_stats_t>(11), NULL) / 256;

    // Convert the integer values to wide character strings
    std::wstring strHP = std::to_wstring(statHP);
    std::wstring strMaxHP = std::to_wstring(statMaxHP);
    std::wstring strMana = std::to_wstring(statMana);
    std::wstring strMaxMana = std::to_wstring(statMaxMana);
    std::wstring strStamina = std::to_wstring(statStamina);
    std::wstring strMaxStamina = std::to_wstring(statMaxStamina);

    d2_win::set_current_font(UI_FONT_16);  // Set font to FONT16

    // Calculate the percentages of current HP, Mana, and Stamina
    float healthPercentage =
        static_cast<float>(statHP) / static_cast<float>(statMaxHP);
    float manaPercentage =
        static_cast<float>(statMana) / static_cast<float>(statMaxMana);
    float staminaPercentage =
        static_cast<float>(statStamina) / static_cast<float>(statMaxStamina);

    int sHeight = 768;
    int sWidth = 1280;

    int sHCenter = sHeight / 2;
    int sWCenter = sWidth / 2;

    // Define default bar color
    DWORD barColor = 0;

    // Determine bar color based on health percentage
    if (healthPercentage > .80) {
      barColor = 118;
    } else if (healthPercentage > .50) {
      barColor = 13;
    } else {
      barColor = 5;
    }

    // Define the dimensions for the bars
    int pbarWidth =
        GetPrivateProfileIntA("Options", "pbarWidth", 200, "./d2tweaks.ini");
    int pbarHeight =
        GetPrivateProfileIntA("Options", "pbarHeight", 16, "./d2tweaks.ini");

    // Define the coordinates for the bars
    // Left coordinate of the bars
    int barX = 245;
    // Top coordinate of the HP bar
    int barY_HP = 728;
    // Top coordinate of the Mana bar with separator
    int barY_Mana = barY_HP + pbarHeight + 4;
    // Top coordinate of the Stamina bar with separator
    int barY_Stamina = barY_Mana + pbarHeight + 4;

    std::wstring life = strHP + L" / " + strMaxHP;
    std::wstring mana = strMana + L" / " + strMaxMana;
    std::wstring stamina = strStamina + L" / " + strMaxStamina;

    // Calculate the filled widths of the bars
    int filledHPWidth = static_cast<int>(healthPercentage * pbarWidth);
    int filledManaWidth = static_cast<int>(manaPercentage * pbarWidth);
    int filledStaminaWidth = static_cast<int>(staminaPercentage * pbarWidth);

    // at 345 we need to minus the width of the text
    sWCenter =
        barX + 100 -
        (d2_win::get_text_pixel_width(const_cast<wchar_t*>(mana.c_str())) / 2);

    HWND diabloIIWnd = FindDiabloIIWindow();

    // Get player_bars_enabled from [Options] in d2tweaks.ini
    int player_bars_enabled_hp = GetPrivateProfileIntA(
        "Options", "player_bars_hp_enabled", 1, "./d2tweaks.ini");
    int player_bars_X_hp = GetPrivateProfileIntA(
        "Options", "player_bars_hp_X", barX, "./d2tweaks.ini");
    int player_bars_Y_hp = GetPrivateProfileIntA(
        "Options", "player_bars_hp_Y", barY_HP, "./d2tweaks.ini");

    int player_bars_enabled_mana = GetPrivateProfileIntA(
        "Options", "player_bars_mana_enabled", 1, "./d2tweaks.ini");
    int player_bars_X_mana = GetPrivateProfileIntA(
        "Options", "player_bars_mana_X", barX, "./d2tweaks.ini");
    int player_bars_Y_mana = GetPrivateProfileIntA(
        "Options", "player_bars_mana_Y", barY_Mana, "./d2tweaks.ini");

    if (player_bars_enabled_hp == 1) {
      // Draw the filled HP bar
      d2_gfx::draw_filled_rect(player_bars_X_hp,
                               player_bars_Y_hp,
                               barX + filledHPWidth,
                               barY_HP + pbarHeight,
                               barColor,
                               255);
      // DrawFilledRect(diabloIIWnd,
      //                barX,
      //                barY_HP,
      //                barX + filledHPWidth,
      //                barY_HP + barHeight,
      //                RGB(255, 0, 0));  // Red color for HP
      d2_win::draw_text(const_cast<wchar_t*>(life.c_str()),
                        sWCenter,
                        barY_HP + 15,
                        UI_COLOR_WHITE,
                        0);
    }
    if (player_bars_enabled_mana == 1) {
      // Draw the filled Mana bar
      d2_gfx::draw_filled_rect(player_bars_X_mana,
                               player_bars_Y_mana,
                               barX + filledManaWidth,
                               barY_Mana + pbarHeight,
                               140,
                               255);
      // DrawFilledRect(diabloIIWnd,
      //                barX,
      //                barY_Mana,
      //                barX + filledManaWidth,
      //                barY_Mana + barHeight,
      //                RGB(100, 100, 255));  // Blue color for Mana
      d2_win::draw_text(const_cast<wchar_t*>(mana.c_str()),
                        sWCenter,
                        barY_Mana + 15,
                        UI_COLOR_WHITE,
                        0);
    }

    // // Define the number of separators
    // int numColors = 256;
    // int numColumns = 4;
    // int colorsPerColumn = numColors / numColumns;
    // int separatorHeight = 1;  // Height of each separator
    // int columnOffset = 200;   // Offset for each column
    // 
    // // Iterate over each column
    // for (int column = 0; column < numColumns; ++column) {
    //   // Calculate the starting X coordinate for this column
    //   int columnX = barX + (column * columnOffset);
    // 
    //   // Draw the filled Mana bars for this column
    //   for (int i = 0; i < colorsPerColumn; ++i) {
    //     int colorIndex = column * colorsPerColumn + i;
    //     int separatorY =
    //         barY_Mana + barHeight + i * (separatorHeight + barHeight);
    // 
    //     // Draw the filled Mana bar
    //     d2_gfx::draw_filled_rect(columnX,
    //                              separatorY,
    //                              columnX + filledManaWidth,
    //                              separatorY + barHeight,
    //                              colorIndex,
    //                              255);
    // 
    //     // Draw the index number at the same coordinates as the bar
    //     std::wstring indexStr = std::to_wstring(colorIndex);
    //     d2_win::draw_text(const_cast<wchar_t*>(indexStr.c_str()),
    //                       columnX - 20,
    //                       separatorY,
    //                       UI_COLOR_DARK_GOLD,
    //                       0);
    //   }
    // }

    // // Draw the filled Stamina bar
    // d2_gfx::draw_filled_rect(barX,
    //                          barY_Stamina,
    //                          barX + filledStaminaWidth,
    //                          barY_Stamina + barHeight,
    //                          12,
    //                          255);
    // DrawFilledRect(diabloIIWnd,
    //                barX,
    //                barY_Stamina,
    //                barX + filledStaminaWidth,
    //                barY_Stamina + barHeight,
    //                RGB(255, 255, 0));  // Green color for Stamina

    int statLevel =
        d2_common::get_stat(player, static_cast<unit_stats_t>(12), NULL);
    int statLastExp =
        d2_common::get_stat(player, static_cast<unit_stats_t>(29), NULL);
    int statNextExp =
        d2_common::get_stat(player, static_cast<unit_stats_t>(30), NULL);

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
  char acPathToIni[MAX_PATH] = {0};
  const char* pcIniFile = "\\d2tweaks.ini";

  GetCurrentDirectory(MAX_PATH, acPathToIni);
  lstrcat(acPathToIni, pcIniFile);

  if (GetPrivateProfileInt("modules", "Autosort", 1, acPathToIni) != FALSE) {
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
