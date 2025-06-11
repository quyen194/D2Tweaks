
#include <Windows.h>

#include <spdlog/spdlog.h>

#include "fw/pool.h"

#include "common/hooking.h"
#include "common/file_ini.h"

#include "diablo2/structures/gfxdata.h"
#include "diablo2/structures/gfxcell.h"
#include "diablo2/structures/monster_data.h"
#include "diablo2/structures/unit.h"

#include "diablo2/utils/screen.h"

#include "diablo2/d2cmp.h"
#include "diablo2/d2client.h"
#include "diablo2/d2common.h"
#include "diablo2/d2gfx.h"
#include "diablo2/d2win.h"

#include "d2tweaks/common/asset_manager.h"
#include "d2tweaks/common/protocol.h"

#include "d2tweaks/ui/ui_manager.h"

#include "d2tweaks/client/client.h"
#include "d2tweaks/client/modules/damage_display/damage_display_client.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace client {
namespace modules {

MODULE_INIT(damage_display)

struct damage_label {
  bool screen_space;
  ui_color_t color;
  uint32_t x;
  uint32_t y;
  uint32_t unit_width;
  uint32_t unit_height;
  uint64_t start;
  int32_t damage;
  int32_t currentHp;  // New field for current hit points
  int32_t maxHp;      // New field for maximum hit points
  int32_t text_width;
  wchar_t text[64];
  bool isChampion;
  bool isUnique;
  bool isSuperUnique;

  damage_label(uint32_t x,
               uint32_t y,
               uint32_t uw,
               uint32_t uh,
               int32_t damage,
               int32_t hp,
               int32_t maxHp)
      : screen_space(false),
        color(ui_color_t::UI_COLOR_WHITE),
        x(x),
        y(y),
        unit_width(uw),
        unit_height(uh),
        start(GetTickCount64()),
        damage(damage),
        currentHp(hp),
        maxHp(maxHp),
        text_width(42),
        isChampion(false),
        isUnique(false),
        isSuperUnique(false) {
    memset(text, 0x00, sizeof text);
  }

  damage_label()
      : screen_space(false),
        color(UI_COLOR_WHITE),
        x(0),
        y(0),
        unit_width(0),
        unit_height(0),
        start(GetTickCount64()),
        damage(0),
        currentHp(0),
        maxHp(0),
        text_width(0),
        isChampion(false),
        isUnique(false),
        isSuperUnique(false) {
    memset(text, 0x00, sizeof text);
  }
};

static growing_object_pool<damage_label> g_label_pool([]() {
  return new damage_label();
});

static const size_t DAMAGE_LABELS_SIZE = 1023;
static damage_label* g_labels[DAMAGE_LABELS_SIZE]{ nullptr };
static size_t g_labels_count = 0;
static uint32_t DISPLAY_TIME = 500;

static float lerp(const float v0, const float v1, const float t) {
  return v0 + t * (v1 - v0);
}

static bool add_label(damage_label* label) {
  for (size_t i = 0; i < DAMAGE_LABELS_SIZE; i++) {
    if (g_labels[i] != nullptr)
      continue;

    g_labels[i] = label;
    g_labels_count++;
    return true;
  }

  return false;
}

static void remove_label(damage_label* label) {
  for (size_t i = 0; i < DAMAGE_LABELS_SIZE; i++) {
    if (g_labels[i] != label)
      continue;

    g_labels[i] = nullptr;
    g_labels_count--;
    return;
  }
}

static void remove_label(size_t index) {
  if (index < 0 || index >= DAMAGE_LABELS_SIZE)
    return;

  g_labels[index] = nullptr;
  g_labels_count--;
}

static unsigned int g_bar_height_enemy = 0;
static unsigned int g_font_enemy = 0;
static unsigned int g_font_player = 1;
static unsigned int g_player_label_posx = 70;
static unsigned int g_player_label_posy = 500;

// Function to draw the health bar using Windows GDI
void drawHealthBar(HWND hWnd,
                   int x,
                   int y,
                   int maxWidth,
                   int height,
                   float healthPercentage,
                   COLORREF fillColor,
                   COLORREF outlineColor) {
  HDC hdc = GetDC(hWnd);

  // Calculate the width of the filled portion of the health bar
  int barWidth = static_cast<int>(maxWidth * healthPercentage);

  // Draw the outline of the health bar
  HPEN hOutlinePen = CreatePen(PS_SOLID, 1, outlineColor);
  SelectObject(hdc, hOutlinePen);
  Rectangle(hdc, x, y, x + maxWidth, y + height);

  // Draw the filled portion of the health bar
  HBRUSH hFillBrush = CreateSolidBrush(fillColor);
  SelectObject(hdc, hFillBrush);
  Rectangle(hdc, x, y, x + barWidth, y + height);

  // Clean up resources
  DeleteObject(hOutlinePen);
  DeleteObject(hFillBrush);

  ReleaseDC(hWnd, hdc);
}

int frame = 0;
unsigned long long nEndTime = 0;
int nTip = 0;
long DURATION = 500;

void OnLoad() {
  srand(static_cast<unsigned int>(time(nullptr)));
}

static void onDraw(HWND hWnd,
                   int x,
                   int y,
                   int maxWidth,
                   int height,
                   float healthPercentage,
                   COLORREF fillColor,
                   COLORREF outlineColor) {
  if (GetTickCount64() >= nEndTime) {
    nEndTime = GetTickCount64() + DURATION;
  }
  drawHealthBar(
      hWnd, x, y, maxWidth, height, healthPercentage, fillColor, outlineColor);
}

static void draw_damage_labels() {
  const auto player = d2_client::get_local_player();

  if (!player)
    return;

  if (g_labels_count == 0)
    return;

  const auto currentTime = GetTickCount64();

  size_t updatedLabels = 0;
  auto mx = 0, my = 0;

  const auto font = d2_win::get_current_font();

  for (size_t i = 0; i < DAMAGE_LABELS_SIZE; i++) {
    const auto label = g_labels[i];

    if (label == nullptr) {
      auto next = g_labels[i + 1];

      if (updatedLabels == g_labels_count)
        break;

      continue;
    }

    const auto delta = currentTime - label->start;

    if (delta >= DISPLAY_TIME) {
      remove_label(i);
      g_label_pool.put(label);
      continue;
    }

    updatedLabels++;

    int32_t mx = label->x;
    int32_t my = label->y;

    if (label->screen_space) {
      utils::screen::world_to_screen(label->x, label->y, mx, my);
    }
    else {
      utils::screen::world_to_screen(label->x, label->y, mx, my);
    }

    mx -= label->text_width / 2;
    mx -= label->unit_width / 2;
    my -= label->unit_height;

    // Draw different labels based on screen_space flag
    if (label->screen_space) {
      const auto offset = static_cast<int32_t>(lerp(static_cast<float>(label->unit_height) + 5.f, static_cast<float>(label->unit_height) + 30.f, static_cast<float>(delta) / static_cast<float>(DISPLAY_TIME)));
      mx -= 10;
      my -= 50;
      my -= offset;

      // Draw player label
      std::wstring dmgText = std::to_wstring(label->damage);
      const wchar_t* dmgTextPtr = dmgText.c_str();
      d2_win::set_current_font(UI_FONT_16);
      d2_win::draw_text(const_cast<wchar_t*>(dmgTextPtr), mx, my, label->color, 0);
    }
    else {
      // Draw monster healthbar
      if (label->currentHp > 0) {
        // Calculate the percentage of currentHp relative to maxHp
        float healthPercentage = (label->maxHp != 0)
                                     ? static_cast<float>(label->currentHp) /
                                           static_cast<float>(label->maxHp)
                                     : 0.0f;

        // Determine the color based on healthPercentage
        ui_color_t textColor;

        textColor =
            healthPercentage >= 0.8f
                ? UI_COLOR_GREEN
                : (healthPercentage <= 0.5f ? UI_COLOR_RED : UI_COLOR_YELLOW);

        // Construct the health fraction string
        std::wstring fractionStr = std::to_wstring(label->currentHp) + L"/" +
                                   std::to_wstring(label->maxHp);

        // Combine the strings for health percentage and bar text
        std::wstring combinedText =
            std::to_wstring(static_cast<int>(healthPercentage * 100.0f)) +
            L"% [" + fractionStr + L"]";
        const WCHAR* combinedTextPtr = combinedText.c_str();

        // Calculate text position for the combined text
        uint32_t textX = mx;
        uint32_t textY = my;

        // Define default bar color
        int barColor;

        // Determine bar color based on health percentage
        if (healthPercentage > .80) {
          barColor = 132;
        }
        else if (healthPercentage > .50) {
          barColor = 12;
        }
        else {
          barColor = 10;
        }

        // Get the width of the combinedText string
        int32_t combinedTextWidth =
            d2_win::get_text_pixel_width(const_cast<wchar_t*>(combinedTextPtr));

        // // Draw the combined text (health percentage and bar text)
        // d2_win::draw_text(
        //     const_cast<wchar_t*>(combinedTcombinedTextext.c_str()),
        //     textX,
        //     textY,
        //     textColor,
        //     0);
        // d2_win::draw_boxed_text(
        //     const_cast<wchar_t*>(fractionStr.c_str()),
        //     textX + label->unit_width / 2,
        //     textY - 12,
        //     0,
        //     0,
        //     textColor);
        // d2_win::set_current_font(
        //     UI_FONT_6);  // Set font to FONT16
        // d2_win::draw_text(
        //     const_cast<wchar_t*>(fractionStr.c_str()),
        //     textX + d2_win::get_text_pixel_width(
        //                 const_cast<wchar_t*>(combinedTextPtr)) /
        //                 2,
        //     textY - 12,
        //     textColor,
        //     0);
        // d2_win::set_current_font(
        //     UI_FONT_6);  // Set font to FONT6
        // d2_win::draw_text(const_cast<wchar_t*>(combinedText.c_str()),
        //                            textX,
        //                            textY,
        //                            textColor,
        //                            0);
        // drawHealthBar(hWndDiabloII,
        //               textX,
        //               textY,
        //               _barWidth,
        //               _barHeight,
        //               healthPercentage,
        //               RGB(255, 0, 0),
        //               RGB(0, 0, 0));
        // onDraw(hWndDiabloII,
        //        textX,
        //        textY,
        //        _barWidth,
        //        _barHeight,
        //        healthPercentage,
        //        RGB(255, 0, 0),
        //        RGB(0, 0, 0));
        // d2_win::draw_boxed_text(
        //     const_cast<wchar_t*>(combinedText.c_str()),
        //     textX,
        //     textY,
        //     0,
        //     3,
        //     textColor);

        d2_win::set_current_font(UI_FONT_6); // Set font to FONT16
        d2_win::draw_text(const_cast<wchar_t*>(combinedText.c_str()),
                          textX,
                          textY,
                          textColor,
                          0);
        d2_gfx::draw_filled_rect(
            textX,
            textY,
            textX + static_cast<int>(healthPercentage * combinedTextWidth),
            textY + g_bar_height_enemy,
            barColor,
            255);

        const auto offset = static_cast<int32_t>(
            lerp(static_cast<float>(label->unit_height) + 5.f,
                 static_cast<float>(label->unit_height) + 30.f,
                 static_cast<float>(delta) / static_cast<float>(DISPLAY_TIME)));
        my -= offset;

        // Draw damage label
        std::wstring dmgText = L" " + std::to_wstring(label->damage) + L" ";
        const wchar_t* dmgTextPtr = dmgText.c_str();
        d2_win::set_current_font(UI_FONT_6);
        d2_win::draw_text(const_cast<wchar_t*>(dmgTextPtr),
                          textX + d2_win::get_text_pixel_width(
                                      const_cast<wchar_t*>(combinedTextPtr)) /
                                      2,
                          my + label->unit_height / 2,
                          textColor,
                          0);
      }
    }
  }

  d2_win::set_current_font(font);
}

static void(__fastcall* g_draw_game_ui)(int32_t a1);
static void __fastcall draw_game_ui(int32_t a1) {
  draw_damage_labels();
  g_draw_game_ui(a1);
}

static ui_color_t damage_type_to_color(common::damage_type_t type) {
  switch (type) {
  case common::DAMAGE_TYPE_PHYSICAL: return UI_COLOR_GREY;
  case common::DAMAGE_TYPE_COLD: return UI_COLOR_BLUE;
  case common::DAMAGE_TYPE_FIRE: return UI_COLOR_RED;
  case common::DAMAGE_TYPE_LIGHTNING: return UI_COLOR_YELLOW;
  case common::DAMAGE_TYPE_POISON: return UI_COLOR_GREEN;
  case common::DAMAGE_TYPE_MAGIC: return UI_COLOR_PURPLE;
  default: return UI_COLOR_BLACK;
  }
}

void damage_display::init_early() {
}

void damage_display::init() {
  FileIni config(common::get_config_path());

  if (config.Int("modules", "DamageDisplay", 1)) {
    g_bar_height_enemy = config.Int("DamageDisplay", "EnemyBarHeight", 6);
    g_font_enemy = config.Int("DamageDisplay", "EnemyDamageFont", 0);
    g_font_player = config.Int("DamageDisplay", "PlayerDamageFont", 1);
    g_player_label_posx = config.Int("DamageDisplay", "PlayerDamagePosx", 70);
    g_player_label_posy = config.Int("DamageDisplay", "PlayerDamagePosy", 500);
    DISPLAY_TIME = config.Int("DamageDisplay", "DisplayTime", 1000);
    Client::instance().register_packet_handler(common::MESSAGE_TYPE_DAMAGE_INFO, this);
    detour::hook(d2_client::get_base() + 0x80A30,
                 draw_game_ui,
                 reinterpret_cast<void **>(&g_draw_game_ui));
  }
}

void damage_display::handle_packet(common::packet_header* packet) {
  static auto& instance = Client::instance();
  static GfxData gfxdata;
  const auto player = d2_client::get_local_player();
  const auto info = static_cast<common::damage_info_sc*>(packet);

  if (player == nullptr)
    return;

  const auto px = d2_common::get_unit_x(player->path);
  const auto py = d2_common::get_unit_y(player->path);

  if (info->unit_type == 0x00 && info->guid == player->guid) {
    const auto label = g_label_pool.get();
    label->screen_space = true;
    label->color = damage_type_to_color(info->damage_type);
    label->unit_width = 0;
    label->unit_height = 0;
    label->x = px;
    label->y = py;
    label->damage = info->damage;
    label->currentHp = info->currentHp;  // Set currentHp value
    label->maxHp = info->maxHp;          // Set maxHp value
    label->start = GetTickCount64();
    swprintf_s(label->text, L"%u", label->damage);
    label->text_width = d2_win::get_text_pixel_width(label->text);

    if (add_label(label))
      return;

    g_label_pool.put(label); //prevent memory leak if there's no room for another label
    return;
  }

  if (info->unit_type != 0x01)
    return;

  const auto monsterUnit = instance.get_client_unit(0x01, info->guid);

  if (monsterUnit == nullptr)
    return;

  const auto mx = d2_common::get_unit_x(monsterUnit->path);
  const auto my = d2_common::get_unit_y(monsterUnit->path);

  const auto label = g_label_pool.get();

  label->color = damage_type_to_color(info->damage_type);

  memset(&gfxdata, 0x00, sizeof gfxdata);

  int32_t index;
  if (d2_client::cache_gfx_data(
          &gfxdata, monsterUnit, nullptr, 0, 0, &index, 1, -1) &&
      d2_cmp::init_gfx_data(&gfxdata) &&
    gfxdata.cell_init) {
    label->unit_width = gfxdata.cell_init->width + gfxdata.cell_init->offset_x;
    label->unit_height = gfxdata.cell_init->height - gfxdata.cell_init->offset_y;
  }
  else {
    label->unit_width = 0;
    label->unit_height = 0;
  }

  label->screen_space = false;
  label->x = mx;
  label->y = my;
  label->damage = info->damage;
  label->currentHp = info->currentHp;  // Set currentHp value
  label->maxHp = info->maxHp;          // Set maxHp value
  label->start = GetTickCount64();
  swprintf_s(label->text, L"%i", label->damage);
  label->text_width = d2_win::get_text_pixel_width(label->text);

  // // Assuming info is already defined and contains the GUID
  // auto monster = d2_client::get_unit_by_guid(info->guid, 0x01);
  // auto monsterData = monster->monster_data;
  // 
  // bool isChampion = monsterData->is_champion;
  // bool isSuperUnique = monsterData->is_super_unique;
  // bool isUnique = monsterData->is_unique;
  // 
  // if (info->currentHp == 0) {
  //   // MessageBoxA(nullptr, "Monster is dead", "Monster is dead", MB_OK);
  //   if (isChampion) {
  //     // MessageBoxA(
  //     //     nullptr, "Monster is a Champion", "Monster is a Champion", MB_OK);
  //     auto isChampionValue =
  //         d2_common::get_stat(player, UNIT_STAT_is_champion, 0);
  //     d2_common::set_stat(
  //         player, UNIT_STAT_is_champion, isChampionValue + 1, 0);
  // 
  //   } else if (isUnique) {
  //     // MessageBoxA(nullptr, "Monster is Unique", "Monster is Unique", MB_OK);
  //     auto isUniqueValue = d2_common::get_stat(player, UNIT_STAT_is_unique, 0);
  //     d2_common::set_stat(player, UNIT_STAT_is_unique, isUniqueValue + 1, 0);
  //   } else if (isSuperUnique) {
  //     // MessageBoxA(
  //     //     nullptr, "Monster is Super Unique", "Monster is Super Unique", MB_OK);
  //     auto isSuperUniqueValue =
  //         d2_common::get_stat(player, UNIT_STAT_is_super_unique, 0);
  //     d2_common::set_stat(
  //         player, UNIT_STAT_is_super_unique, isSuperUniqueValue + 1, 0);
  //   }
  // }

  if (add_label(label))
    return;

  g_label_pool.put(label); //prevent memory leak if there's no room for another label
}

void damage_display::tick() {
}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
