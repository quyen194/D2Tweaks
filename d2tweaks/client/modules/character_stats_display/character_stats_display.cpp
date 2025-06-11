
#include <Windows.h>

#include <atlstr.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "common/strings.h"
#include "common/file_ini.h"

#include "diablo2/structures/data/item_types_line.h"
#include "diablo2/structures/data/items_line.h"
#include "diablo2/structures/game.h"
#include "diablo2/structures/inventory.h"
#include "diablo2/structures/item_data.h"
#include "diablo2/structures/path.h"
#include "diablo2/structures/player_data.h"
#include "diablo2/structures/unit.h"

#include "diablo2/d2client.h"
#include "diablo2/d2common.h"
#include "diablo2/d2gfx.h"

#include "d2tweaks/common/asset_manager.h"
#include "d2tweaks/common/common.h"
#include "d2tweaks/common/protocol.h"

#include "d2tweaks/ui/controls/button.h"
#include "d2tweaks/ui/menu.h"
#include "d2tweaks/ui/ui_manager.h"
#include "d2tweaks/ui/ui_utils.h"

#include "d2tweaks/client/client.h"
#include "d2tweaks/client/modules/character_stats_display/character_stats_display.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

namespace d2_tweaks {
namespace client {
namespace modules {

// Define the default coordinates for the bars
#define BAR_HP_X 245    // Left coordinate of the HP bar
#define BAR_HP_Y 728    // Top coordinate of the HP bar

// Define the structure to hold stat information
struct StatEntry {
  std::wstring stat_label;
  ui_color_t colorStat, colorStatValue;
  int x1, y1, x2, y2, is_item_stat, item_type_id, stat, op,
      param = 0;  // x1,y1 stat_display_string | x2,y2 statValue
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
    {13, ui_font_t::UI_FONT_INGAMECHAT},
};

// Define the mapColorToEnum function
ui_color_t mapColorToEnum(const std::string& colorName) {
  static const std::unordered_map<std::string, ui_color_t> colorMap = {
    {"RED", ui_color_t::UI_COLOR_RED},
    {"LIGHT_GREEN", ui_color_t::UI_COLOR_LIGHT_GREEN},
    {"BLUE", ui_color_t::UI_COLOR_BLUE},
    {"DARK_GOLD", ui_color_t::UI_COLOR_DARK_GOLD},
    {"GREY", ui_color_t::UI_COLOR_GREY},
    {"BLACK", ui_color_t::UI_COLOR_BLACK},
    {"GOLD", ui_color_t::UI_COLOR_GOLD},
    {"ORANGE", ui_color_t::UI_COLOR_ORANGE},
    {"YELLOW", ui_color_t::UI_COLOR_YELLOW},
    {"DARK_GREEN", ui_color_t::UI_COLOR_DARK_GREEN},
    {"PURPLE", ui_color_t::UI_COLOR_PURPLE},
    {"GREEN", ui_color_t::UI_COLOR_GREEN},
    {"WHITE", ui_color_t::UI_COLOR_WHITE},
    {"BLACK2", ui_color_t::UI_COLOR_BLACK2},
    {"DARK_WHITE", ui_color_t::UI_COLOR_DARK_WHITE},
    {"LIGHT_GREY", ui_color_t::UI_COLOR_LIGHT_GREY}
  };

  auto it = colorMap.find(colorName);
  if (it != colorMap.end()) {
    return it->second;
  }
  // Default color if not found
  return ui_color_t::UI_COLOR_WHITE;
}

class character_stats_menu : public ui::menu {
  int m_StatsFont;

  int m_PlayerBarsWidth;
  int m_PlayerBarsHeight;

  int m_PlayerBarHpEnable;
  int m_PlayerBarHpX;
  int m_PlayerBarHpY;

  int m_PlayerBarManaEnable;
  int m_PlayerBarManaX;
  int m_PlayerBarManaY;

  std::vector<StatEntry> globalStatsVector;

 public:
  character_stats_menu()
      : m_StatsFont(0),
        m_PlayerBarsWidth(200),
        m_PlayerBarsHeight(16),
        m_PlayerBarHpEnable(1),
        m_PlayerBarHpX(BAR_HP_X),
        m_PlayerBarHpY(BAR_HP_Y),
        m_PlayerBarManaEnable(1),
        m_PlayerBarManaX(BAR_HP_X),
        m_PlayerBarManaY(1) {
    FileIni config(common::get_config_path());

    menu::set_enabled(true);
    menu::set_visible(true);

    // TODO: Add config for health/mana bar as button in xml files
    // if (DLLBASE_D2EXPRES != 0)
    //   load_xml("d2tweaks\\interface_d2expres\\character_stats.xml");
    // if (DLLBASE_SGD2FREERES != 0)
    //   load_xml("d2tweaks\\interface_sgd2freeres\\character_stats.xml");
    // if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
    //   load_xml("d2tweaks\\interface_vanilla\\character_stats.xml");

    m_StatsFont = config.Int("CharacterStatsDisplay", "StatsFont", 0);

    // Define the dimensions for the bars
    m_PlayerBarsWidth = config.Int("CharacterStatsDisplay", "PlayerBarsWidth", 200);
    m_PlayerBarsHeight = config.Int("CharacterStatsDisplay", "PlayerBarsHeight", 16);

    // Get player_bars_enabled from [Options] in d2tweaks.ini
    m_PlayerBarHpEnable = config.Int("CharacterStatsDisplay", "PlayerBarHpEnable", 1);
    m_PlayerBarHpX = config.Int("CharacterStatsDisplay", "PlayerBarHpX", BAR_HP_X);
    m_PlayerBarHpY = config.Int("CharacterStatsDisplay", "PlayerBarHpY", BAR_HP_Y);

    m_PlayerBarManaEnable = config.Int("CharacterStatsDisplay", "PlayerBarManaEnable", 1);
    m_PlayerBarManaX = config.Int("CharacterStatsDisplay", "PlayerBarManaX", BAR_HP_X);
    int iDefBarManaY = BAR_HP_Y + m_PlayerBarsHeight + 4;
    m_PlayerBarManaY = config.Int("CharacterStatsDisplay", "PlayerBarManaY", iDefBarManaY);

    // read stats settings
    int iStatSectionCount = config.Int("StatSections", "SectionCount", 0);

    StatEntry entry;
    std::string section;
    std::wstring section_w;
    std::string key;
    std::string text;

    for (int i = 0; i < iStatSectionCount; i++) {
      key = utils::string_format("Section%02d", i);
      section = config.String("StatSections", key, 255);
      section_w = utils::ToWString(section);

      int iEnable = config.Int(section, "Enable", 0);
      if (!iEnable) {
        continue;
      }

      entry.stat = config.Int(section, "Stat", 0);
      entry.stat_label = config.String(section_w, L"StatLabel", 255);
      text = config.String(section, "StatLabelColor", 0);
      entry.colorStat = mapColorToEnum(text.c_str());
      text = config.String(section, "StatValueColor", 0);
      entry.colorStatValue = mapColorToEnum(text.c_str());
      entry.x1 = config.Int(section, "StatLabelX", 0);
      entry.y1 = config.Int(section, "StatLabelY", 0);
      entry.x2 = config.Int(section, "StatValueX", 0);
      entry.y2 = config.Int(section, "StatValueY", 0);
      entry.is_item_stat = config.Int(section, "IsItemStat", 0);
      entry.item_type_id = config.Int(section, "ItemTypeId", 0);
      entry.op = config.Int(section, "op", 0);
      entry.param = config.Int(section, "param", 0);

      globalStatsVector.push_back(entry);
    }
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

  void draw() override {
    const auto player = d2_client::get_local_player();

    auto stats = globalStatsVector;
    int textOffset = 40;  // Initial offset for the first line

    // Add all items to vector
    std::vector<Unit*> items;
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
            stat.stat_label);  // .append(L" " + statValueStr);

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
          d2_win::set_current_font(fontMap[m_StatsFont]);  // Set font to FONT16
          d2_win::draw_text(const_cast<wchar_t*>(statText.c_str()),
                            stat.x1,
                            stat.y1 + textOffset,
                            stat.colorStat,
                            0);

          d2_win::set_current_font(fontMap[m_StatsFont]);  // Set font to FONT16
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

    // Define the coordinates for the bars
    // Top coordinate of the Mana bar with separator
    int iBarManaY = BAR_HP_Y + m_PlayerBarsHeight + 4;
    // Top coordinate of the Stamina bar with separator
    int iBarStaminaY = iBarManaY + m_PlayerBarsHeight + 4;

    std::wstring life = strHP + L" / " + strMaxHP;
    std::wstring mana = strMana + L" / " + strMaxMana;
    std::wstring stamina = strStamina + L" / " + strMaxStamina;

    // Calculate the filled widths of the bars
    int filledHPWidth = static_cast<int>(healthPercentage * m_PlayerBarsWidth);
    int filledManaWidth = static_cast<int>(manaPercentage * m_PlayerBarsWidth);
    int filledStaminaWidth = static_cast<int>(staminaPercentage * m_PlayerBarsWidth);

    // at 345 we need to minus the width of the text
    sWCenter =
        BAR_HP_X + 100 -
        (d2_win::get_text_pixel_width(const_cast<wchar_t*>(mana.c_str())) / 2);

    // HWND diabloIIWnd = ui::FindDiabloIIWindow();

    if (m_PlayerBarHpEnable == 1) {
      // Draw the filled HP bar
      d2_gfx::draw_filled_rect(m_PlayerBarHpX,
                               m_PlayerBarHpY,
                               BAR_HP_X + filledHPWidth,
                               BAR_HP_Y + m_PlayerBarsHeight,
                               barColor,
                               255);
      // DrawFilledRect(diabloIIWnd,
      //                BAR_HP_X,
      //                BAR_HP_Y,
      //                BAR_HP_X + filledHPWidth,
      //                BAR_HP_Y + barHeight,
      //                RGB(255, 0, 0));  // Red color for HP
      d2_win::draw_text(const_cast<wchar_t*>(life.c_str()),
                        sWCenter,
                        BAR_HP_Y + 15,
                        UI_COLOR_WHITE,
                        0);
    }
    if (m_PlayerBarManaEnable == 1) {
      // Draw the filled Mana bar
      d2_gfx::draw_filled_rect(m_PlayerBarManaX,
                               m_PlayerBarManaY,
                               BAR_HP_X + filledManaWidth,
                               iBarManaY + m_PlayerBarsHeight,
                               140,
                               255);
      // DrawFilledRect(diabloIIWnd,
      //                BAR_HP_X,
      //                iBarManaY,
      //                BAR_HP_X + filledManaWidth,
      //                iBarManaY + barHeight,
      //                RGB(100, 100, 255));  // Blue color for Mana
      d2_win::draw_text(const_cast<wchar_t*>(mana.c_str()),
                        sWCenter,
                        iBarManaY + 15,
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
    //   int columnX = BAR_HP_X + (column * columnOffset);
    // 
    //   // Draw the filled Mana bars for this column
    //   for (int i = 0; i < colorsPerColumn; ++i) {
    //     int colorIndex = column * colorsPerColumn + i;
    //     int separatorY =
    //         iBarManaY + barHeight + i * (separatorHeight + barHeight);
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
    // d2_gfx::draw_filled_rect(BAR_HP_X,
    //                          iBarStaminaY,
    //                          BAR_HP_X + filledStaminaWidth,
    //                          iBarStaminaY + barHeight,
    //                          12,
    //                          255);
    // DrawFilledRect(diabloIIWnd,
    //                BAR_HP_X,
    //                iBarStaminaY,
    //                BAR_HP_X + filledStaminaWidth,
    //                iBarStaminaY + barHeight,
    //                RGB(255, 255, 0));  // Green color for Stamina

    int statLevel =
        d2_common::get_stat(player, static_cast<unit_stats_t>(12), NULL);
    int statLastExp =
        d2_common::get_stat(player, static_cast<unit_stats_t>(29), NULL);
    int statNextExp =
        d2_common::get_stat(player, static_cast<unit_stats_t>(30), NULL);

    d2_win::set_current_font(UI_FONT_16);  // Set font to FONT16

    menu::draw();
  }

 private:
  button* get_button(const std::string& name,
                     const std::function<void()>& onClick) {
    auto result = static_cast<button*>(get_control(name));

    result->set_on_click(onClick);

    result->set_enabled(false);
    result->set_visible(false);

    return result;
  }
};

MODULE_INIT(character_stats_display)

void character_stats_display::init_early() {}

void character_stats_display::init() {
  FileIni ini(common::get_config_path());

  if (ini.Int("modules", "CharacterStatsDisplay", 0)) {
    ui::Manager::instance().add_menu(new character_stats_menu());
  }
}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
