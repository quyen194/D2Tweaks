#pragma once

#include <fw/singleton.h>

#include <d2tweaks/ui/menu.h>

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



				void(__fastcall* m_draw_dropped_items_names_original)(void*, void*);
				void(__fastcall* m_handle_dropped_items_original)(void*, void*);
			public:
				explicit loot_filter_settings_menu(token);

				void reload_settings();

				void draw() override;
			private:
				void register_misc_checkboxes();
				void register_quality_checkboxes();

				void update_alt_only(bool value);
				void extract_r01(bool value);
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
