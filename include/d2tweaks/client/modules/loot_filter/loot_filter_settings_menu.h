#pragma once

#include <fw/singleton.h>

#include <d2tweaks/ui/menu.h>

#include <chrono>  // Add this line
#include <diablo2/d2common.h>

namespace diablo2 {
	namespace structures {
		struct unit;
		enum class item_quality_t : unsigned;
	}
}

namespace d2_tweaks {
	namespace ui {
		namespace controls {
			class checkbox;
		}
	}

	namespace client {
		namespace modules {
			class loot_filter_settings_menu final : public ui::menu, singleton<loot_filter_settings_menu> {
				ui::controls::checkbox* m_altonly;
				ui::controls::checkbox* m_show_gold;
				ui::controls::checkbox* m_show_runes;
				ui::controls::checkbox* m_show_gems;

				ui::controls::checkbox* m_show_ruby;
				ui::controls::checkbox* m_show_sapphire;
				ui::controls::checkbox* m_show_emerald;
				ui::controls::checkbox* m_show_diamond;
				ui::controls::checkbox* m_show_topaz;
				ui::controls::checkbox* m_show_amethyst;
				ui::controls::checkbox* m_show_skull;

				ui::controls::checkbox* m_show_r01;
				ui::controls::checkbox* m_show_r02;
				ui::controls::checkbox* m_show_r03;
				ui::controls::checkbox* m_show_r04;
				ui::controls::checkbox* m_show_r05;
				ui::controls::checkbox* m_show_r06;
				ui::controls::checkbox* m_show_r07;
				ui::controls::checkbox* m_show_r08;
				ui::controls::checkbox* m_show_r09;
				ui::controls::checkbox* m_show_r10;
				ui::controls::checkbox* m_show_r11;
				ui::controls::checkbox* m_show_r12;
				ui::controls::checkbox* m_show_r13;
				ui::controls::checkbox* m_show_r14;
				ui::controls::checkbox* m_show_r15;
				ui::controls::checkbox* m_show_r16;
				ui::controls::checkbox* m_show_r17;
				ui::controls::checkbox* m_show_r18;
				ui::controls::checkbox* m_show_r19;
				ui::controls::checkbox* m_show_r20;
				ui::controls::checkbox* m_show_r21;
				ui::controls::checkbox* m_show_r22;
				ui::controls::checkbox* m_show_r23;
				ui::controls::checkbox* m_show_r24;
				ui::controls::checkbox* m_show_r25;
				ui::controls::checkbox* m_show_r26;
				ui::controls::checkbox* m_show_r27;
				ui::controls::checkbox* m_show_r28;
				ui::controls::checkbox* m_show_r29;
				ui::controls::checkbox* m_show_r30;
				ui::controls::checkbox* m_show_r31;
				ui::controls::checkbox* m_show_r32;
				ui::controls::checkbox* m_show_r33;

				ui::controls::checkbox* m_show_chipped;
				ui::controls::checkbox* m_show_flawed;
				ui::controls::checkbox* m_show_normal;
				ui::controls::checkbox* m_show_flawless;
				ui::controls::checkbox* m_show_perfect;

				ui::controls::checkbox* m_show_flourite;
				ui::controls::checkbox* m_show_jade;
				ui::controls::checkbox* m_show_argonite;
				ui::controls::checkbox* m_show_azurite;
				ui::controls::checkbox* m_show_sulpher;
				ui::controls::checkbox* m_show_quartz;
				ui::controls::checkbox* m_show_tiger_eye;

				ui::controls::checkbox* m_show_rough;
				ui::controls::checkbox* m_show_faded;
				ui::controls::checkbox* m_show_blemished;
				ui::controls::checkbox* m_show_cleaned;
				ui::controls::checkbox* m_show_triangle;
				ui::controls::checkbox* m_show_trangle_cut;
				ui::controls::checkbox* m_show_square;
				ui::controls::checkbox* m_show_square_cut;
				ui::controls::checkbox* m_show_regular;
				ui::controls::checkbox* m_show_regular_cut;
				ui::controls::checkbox* m_show_star;
				ui::controls::checkbox* m_show_star_cut;
				ui::controls::checkbox* m_show_imperial;
				ui::controls::checkbox* m_show_imperial_cut;
				ui::controls::checkbox* m_show_royal;
				ui::controls::checkbox* m_show_royal_cut;
				ui::controls::checkbox* m_show_spectacular;
				ui::controls::checkbox* m_show_legendary;
				ui::controls::checkbox* m_show_legendary_cut;
				ui::controls::checkbox* m_show_rejuv_potion;
				ui::controls::checkbox* m_show_full_rejuv_potion;

				ui::controls::checkbox* m_show_hp1;
				ui::controls::checkbox* m_show_hp2;
				ui::controls::checkbox* m_show_hp3;
				ui::controls::checkbox* m_show_hp4;
				ui::controls::checkbox* m_show_hp5;

				ui::controls::checkbox* m_show_mp1;
				ui::controls::checkbox* m_show_mp2;
				ui::controls::checkbox* m_show_mp3;
				ui::controls::checkbox* m_show_mp4;
				ui::controls::checkbox* m_show_mp5;

				ui::controls::checkbox* m_show_frp;
				ui::controls::checkbox* m_show_mrp;
				ui::controls::checkbox* m_show_prp;
				ui::controls::checkbox* m_show_crp;
				ui::controls::checkbox* m_show_lrp;

				ui::controls::checkbox* m_show_vps;
				ui::controls::checkbox* m_show_yps;
				ui::controls::checkbox* m_show_wms;

				ui::controls::checkbox* m_close;

				void(__fastcall* m_draw_dropped_items_names_original)(void*, void*);
				void(__fastcall* m_handle_dropped_items_original)(void*, void*);
			public:
				explicit loot_filter_settings_menu(token);

				void reload_settings();

				void draw() override;

			private:
				std::string m_selected_gem;

				std::chrono::steady_clock::time_point m_last_packet_sent;

				void gem_checkbox_clicked(const std::string& gem);
				void stone_checkbox_clicked(const std::string& gem);

				void register_misc_checkboxes();
				void register_quality_checkboxes();

				void update_alt_only(bool value);


				void extract_item(bool value, int prop, uint32_t val, uint32_t rune, diablo2::unit_stats_t stat);

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

				//void extract_flourite(bool value);
				//void extract_jade(bool value);
				//void extract_argonite(bool value);
				//void extract_azurite(bool value);
				//void extract_sulpher(bool value);
				//void extract_quartz(bool value);
				//void extract_tiger_eye(bool value);

				void update_show_gold(bool value);
				void update_show_runes(bool value);
				void update_show_gems(bool value);

				void update_quality_allowance(bool value, diablo2::structures::item_quality_t quality);
				void register_quality_checkbox(const std::string& name, diablo2::structures::item_quality_t quality);

				void setup_hooks();
				void setup_alt_hook() const;

				static bool is_gold(diablo2::structures::unit* item);
				static bool is_rune(diablo2::structures::unit* item);
				static bool is_gem(diablo2::structures::unit* item);

				static bool __fastcall check_alt_item(diablo2::structures::unit* unit);

				//draw labels over dropped items
				static void __fastcall draw_dropped_items_names(diablo2::structures::unit* unit, void* edx);

				//handle hovering over item and actual click
				static void __fastcall handle_dropped_items(diablo2::structures::unit* unit, void* edx);
			};
		}
	}
}
