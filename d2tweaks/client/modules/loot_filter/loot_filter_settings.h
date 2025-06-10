#pragma once

#include <cstring>

#include "diablo2/structures/item_data.h"

using namespace diablo2::structures;

namespace d2_tweaks {
namespace client {
namespace modules {

struct loot_filter_settings {
  size_t size;  // struct size

  bool alt_only;
  bool show_gold;
  bool show_runes;
  bool show_gems;

  bool m_show_r01;
  bool m_show_r02;
  bool m_show_r03;
  bool m_show_r04;
  bool m_show_r05;
  bool m_show_r06;
  bool m_show_r07;
  bool m_show_r08;
  bool m_show_r09;
  bool m_show_r10;
  bool m_show_r11;
  bool m_show_r12;
  bool m_show_r13;
  bool m_show_r14;
  bool m_show_r15;
  bool m_show_r16;
  bool m_show_r17;
  bool m_show_r18;
  bool m_show_r19;
  bool m_show_r20;
  bool m_show_r21;
  bool m_show_r22;
  bool m_show_r23;
  bool m_show_r24;
  bool m_show_r25;
  bool m_show_r26;
  bool m_show_r27;
  bool m_show_r28;
  bool m_show_r29;
  bool m_show_r30;
  bool m_show_r31;
  bool m_show_r32;
  bool m_show_r33;

  bool m_show_amethyst;
  bool m_show_diamond;
  bool m_show_emerald;
  bool m_show_ruby;
  bool m_show_sapphire;
  bool m_show_skull;
  bool m_show_topaz;

  bool m_show_chipped;
  bool m_show_flawed;
  bool m_show_normal;
  bool m_show_flawless;
  bool m_show_perfect;

  bool m_show_rough;
  bool m_show_faded;
  bool m_show_blemished;
  bool m_show_cleaned;
  bool m_show_triangle;
  bool m_show_trangle_cut;
  bool m_show_square;
  bool m_show_square_cut;
  bool m_show_regular;
  bool m_show_regular_cut;
  bool m_show_star;
  bool m_show_star_cut;
  bool m_show_imperial;
  bool m_show_imperial_cut;
  bool m_show_royal;
  bool m_show_royal_cut;
  bool m_show_spectacular;
  bool m_show_legendary;
  bool m_show_legendary_cut;

  bool m_show_flourite;
  bool m_show_jade;
  bool m_show_argonite;
  bool m_show_azurite;
  bool m_show_sulpher;
  bool m_show_quartz;
  bool m_show_tiger_eye;

  bool m_show_rejuv_potion;
  bool m_show_full_rejuv_potion;

  bool m_show_hp1;
  bool m_show_hp2;
  bool m_show_hp3;
  bool m_show_hp4;
  bool m_show_hp5;

  bool m_show_mp1;
  bool m_show_mp2;
  bool m_show_mp3;
  bool m_show_mp4;
  bool m_show_mp5;

  bool m_show_frp;
  bool m_show_mrp;
  bool m_show_prp;
  bool m_show_crp;
  bool m_show_lrp;
  bool m_show_vps;
  bool m_show_yps;
  bool m_show_wms;

  bool m_show_close;

  bool quality_settings[static_cast<size_t>(
      item_quality_t::ITEM_QUALITY_COUNT)];

  char reserved[888];

  static loot_filter_settings& get();

  static void save(const char* name);
  static void load(const char* name);
  static void remove(const char* name);

 private:
  loot_filter_settings()
      : size(sizeof(loot_filter_settings)),
        alt_only(false),
        show_gold(true),
        show_runes(true),
        show_gems(true),
        reserved{} {
    memset(quality_settings, 0x1, sizeof quality_settings);
  }
};

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
