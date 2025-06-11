#pragma once

#include <chrono>  // Add this line

#include "fw/singleton.h"

#include "diablo2/structures/item_data.h"

#include "diablo2/d2common.h"

#include "d2tweaks/ui/controls/checkbox.h"
#include "d2tweaks/ui/menu.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

namespace d2_tweaks {
namespace client {

class loot_filter_settings_menu final
    : public ui::menu,
      public singleton<loot_filter_settings_menu> {
  int m_iBagStatsX;
  int m_iBagStatsY;
  int m_iBagStatsZ;

  checkbox* m_altonly;
  checkbox* m_show_gold;
  checkbox* m_show_runes;
  checkbox* m_show_gems;

  checkbox* m_show_ruby;
  checkbox* m_show_sapphire;
  checkbox* m_show_emerald;
  checkbox* m_show_diamond;
  checkbox* m_show_topaz;
  checkbox* m_show_amethyst;
  checkbox* m_show_skull;

  checkbox* m_show_r01;
  checkbox* m_show_r02;
  checkbox* m_show_r03;
  checkbox* m_show_r04;
  checkbox* m_show_r05;
  checkbox* m_show_r06;
  checkbox* m_show_r07;
  checkbox* m_show_r08;
  checkbox* m_show_r09;
  checkbox* m_show_r10;
  checkbox* m_show_r11;
  checkbox* m_show_r12;
  checkbox* m_show_r13;
  checkbox* m_show_r14;
  checkbox* m_show_r15;
  checkbox* m_show_r16;
  checkbox* m_show_r17;
  checkbox* m_show_r18;
  checkbox* m_show_r19;
  checkbox* m_show_r20;
  checkbox* m_show_r21;
  checkbox* m_show_r22;
  checkbox* m_show_r23;
  checkbox* m_show_r24;
  checkbox* m_show_r25;
  checkbox* m_show_r26;
  checkbox* m_show_r27;
  checkbox* m_show_r28;
  checkbox* m_show_r29;
  checkbox* m_show_r30;
  checkbox* m_show_r31;
  checkbox* m_show_r32;
  checkbox* m_show_r33;

  checkbox* m_show_chipped;
  checkbox* m_show_flawed;
  checkbox* m_show_normal;
  checkbox* m_show_flawless;
  checkbox* m_show_perfect;

  checkbox* m_show_flourite;
  checkbox* m_show_jade;
  checkbox* m_show_argonite;
  checkbox* m_show_azurite;
  checkbox* m_show_sulpher;
  checkbox* m_show_quartz;
  checkbox* m_show_tiger_eye;

  checkbox* m_show_rough;
  checkbox* m_show_faded;
  checkbox* m_show_blemished;
  checkbox* m_show_cleaned;
  checkbox* m_show_triangle;
  checkbox* m_show_trangle_cut;
  checkbox* m_show_square;
  checkbox* m_show_square_cut;
  checkbox* m_show_regular;
  checkbox* m_show_regular_cut;
  checkbox* m_show_star;
  checkbox* m_show_star_cut;
  checkbox* m_show_imperial;
  checkbox* m_show_imperial_cut;
  checkbox* m_show_royal;
  checkbox* m_show_royal_cut;
  checkbox* m_show_spectacular;
  checkbox* m_show_legendary;
  checkbox* m_show_legendary_cut;
  checkbox* m_show_rejuv_potion;
  checkbox* m_show_full_rejuv_potion;

  checkbox* m_show_hp1;
  checkbox* m_show_hp2;
  checkbox* m_show_hp3;
  checkbox* m_show_hp4;
  checkbox* m_show_hp5;

  checkbox* m_show_mp1;
  checkbox* m_show_mp2;
  checkbox* m_show_mp3;
  checkbox* m_show_mp4;
  checkbox* m_show_mp5;

  checkbox* m_show_frp;
  checkbox* m_show_mrp;
  checkbox* m_show_prp;
  checkbox* m_show_crp;
  checkbox* m_show_lrp;

  checkbox* m_show_vps;
  checkbox* m_show_yps;
  checkbox* m_show_wms;

  checkbox* m_close;

  void(__fastcall* m_draw_dropped_items_names_original)(void*, void*);
  void(__fastcall* m_handle_dropped_items_original)(void*, void*);

 public:
  explicit loot_filter_settings_menu(token);

  void reload_settings();

  void draw() override;
  void drawStats();

 private:
  std::string m_selected_gem;

  std::chrono::steady_clock::time_point m_last_packet_sent;

  void gem_checkbox_clicked(const std::string& gem);
  void stone_checkbox_clicked(const std::string& gem);

  void register_misc_checkboxes();
  void register_quality_checkboxes();

  void update_alt_only(bool value);

  void extract_item(bool value,
                    int prop,
                    uint32_t val,
                    uint32_t rune,
                    unit_stats_t stat);

  void extract_r01(bool value);
  void extract_r02(bool value);
  void extract_r03(bool value);
  void extract_r04(bool value);
  void extract_r05(bool value);
  void extract_r06(bool value);
  void extract_r07(bool value);
  void extract_r08(bool value);
  void extract_r09(bool value);
  void extract_r10(bool value);
  void extract_r11(bool value);
  void extract_r12(bool value);
  void extract_r13(bool value);
  void extract_r14(bool value);
  void extract_r15(bool value);
  void extract_r16(bool value);
  void extract_r17(bool value);
  void extract_r18(bool value);
  void extract_r19(bool value);
  void extract_r20(bool value);
  void extract_r21(bool value);
  void extract_r22(bool value);
  void extract_r23(bool value);
  void extract_r24(bool value);
  void extract_r25(bool value);
  void extract_r26(bool value);
  void extract_r27(bool value);
  void extract_r28(bool value);
  void extract_r29(bool value);
  void extract_r30(bool value);
  void extract_r31(bool value);
  void extract_r32(bool value);
  void extract_r33(bool value);

  void extract_chipped(bool value);
  void extract_flawed(bool value);
  void extract_normal(bool value);
  void extract_flawless(bool value);
  void extract_perfect(bool value);

  void extract_rough(bool value);
  void extract_faded(bool value);
  void extract_blemished(bool value);
  void extract_cleaned(bool value);
  void extract_triangle(bool value);
  void extract_trangle_cut(bool value);
  void extract_square(bool value);
  void extract_square_cut(bool value);
  void extract_regular(bool value);
  void extract_regular_cut(bool value);
  void extract_star(bool value);
  void extract_star_cut(bool value);
  void extract_imperial(bool value);
  void extract_imperial_cut(bool value);
  void extract_royal(bool value);
  void extract_royal_cut(bool value);
  void extract_spectacular(bool value);
  void extract_legendary(bool value);
  void extract_legendary_cut(bool value);

  void extract_rejuv_potion(bool value);
  void extract_full_rejuv_potion(bool value);

  void extract_hp1(bool value);
  void extract_hp2(bool value);
  void extract_hp3(bool value);
  void extract_hp4(bool value);
  void extract_hp5(bool value);

  void extract_mp1(bool value);
  void extract_mp2(bool value);
  void extract_mp3(bool value);
  void extract_mp4(bool value);
  void extract_mp5(bool value);

  void extract_frp(bool value);
  void extract_mrp(bool value);
  void extract_prp(bool value);
  void extract_crp(bool value);
  void extract_lrp(bool value);
  void extract_vps(bool value);
  void extract_yps(bool value);
  void extract_wms(bool value);

  void close_window(bool value);

  // void extract_flourite(bool value);
  // void extract_jade(bool value);
  // void extract_argonite(bool value);
  // void extract_azurite(bool value);
  // void extract_sulpher(bool value);
  // void extract_quartz(bool value);
  // void extract_tiger_eye(bool value);

  void update_show_gold(bool value);
  void update_show_runes(bool value);
  void update_show_gems(bool value);

  void update_quality_allowance(bool value, item_quality_t quality);
  void register_quality_checkbox(const std::string& name,
                                 item_quality_t quality);

  void setup_hooks();
  void setup_alt_hook() const;

  static bool is_gold(Unit* item);
  static bool is_rune(Unit* item);
  static bool is_gem(Unit* item);

  static bool __fastcall check_alt_item(Unit* unit);

  // draw labels over dropped items
  static void __fastcall draw_dropped_items_names(Unit* unit, void* edx);

  // handle hovering over item and actual click
  static void __fastcall handle_dropped_items(Unit* unit, void* edx);
};

}  // namespace client
}  // namespace d2_tweaks
