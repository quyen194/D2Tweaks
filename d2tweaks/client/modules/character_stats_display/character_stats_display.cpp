#include <d2tweaks/client/client.h>
#include <d2tweaks/client/modules/character_stats_display/character_stats_display.h>
#include <d2tweaks/common/asset_manager.h>
#include <d2tweaks/common/common.h>
#include <d2tweaks/common/protocol.h>
#include <d2tweaks/ui/controls/button.h>
#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>
#include <diablo2/d2client.h>
#include <diablo2/d2common.h>
#include <diablo2/d2gfx.h>

#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

namespace d2_tweaks {
namespace client {
namespace modules {

class character_stats_menu : public ui::menu {

 public:
  character_stats_menu() {
    menu::set_enabled(true);
    menu::set_visible(true);

    // TODO: Add config for health/mana bar as button in xml files
    // if (DLLBASE_D2EXPRES != 0)
    //   load_xml("d2tweaks\\interface_d2expres\\character_stats.xml");
    // if (DLLBASE_SGD2FREERES != 0)
    //   load_xml("d2tweaks\\interface_sgd2freeres\\character_stats.xml");
    // if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
    //   load_xml("d2tweaks\\interface_vanilla\\character_stats.xml");

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

  void draw() override {
    const char* config_path = common::get_config_path();
    const auto player = d2_client::get_local_player();

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
        GetPrivateProfileIntA("Options", "pbarWidth", 200, config_path);
    int pbarHeight =
        GetPrivateProfileIntA("Options", "pbarHeight", 16, config_path);

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
        "Options", "player_bars_hp_enabled", 1, config_path);
    int player_bars_X_hp = GetPrivateProfileIntA(
        "Options", "player_bars_hp_X", barX, config_path);
    int player_bars_Y_hp = GetPrivateProfileIntA(
        "Options", "player_bars_hp_Y", barY_HP, config_path);

    int player_bars_enabled_mana = GetPrivateProfileIntA(
        "Options", "player_bars_mana_enabled", 1, config_path);
    int player_bars_X_mana = GetPrivateProfileIntA(
        "Options", "player_bars_mana_X", barX, config_path);
    int player_bars_Y_mana = GetPrivateProfileIntA(
        "Options", "player_bars_mana_Y", barY_Mana, config_path);

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
  const char* config_path = common::get_config_path();

  if (GetPrivateProfileInt("modules", "CharacterStatsDisplay", 0, config_path)) {
    singleton<ui::ui_manager>::instance().add_menu(new character_stats_menu());
  }
}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
