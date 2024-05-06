#pragma once

#include <fw/singleton.h>
#include <d2tweaks/ui/menu.h>

namespace d2_tweaks {
	namespace ui {
		namespace controls {
			class button;
		}
	}
}

namespace d2_tweaks {
	namespace client {
		namespace modules {
			class loot_filter_settings_toggle_menu final : public ui::menu, singleton<loot_filter_settings_toggle_menu> {
				ui::controls::button* m_toggle_filter_settings_btn;
				ui::controls::button* m_btn_toggle_stats;
				ui::controls::button* m_btn_toggle_help;
				ui::controls::button* m_btn_toggle_cube;
				ui::controls::button* m_btn_toggle_stash;
				ui::controls::button* m_btn_toggle_bag;

				menu* m_filter_settings_menu;
				menu* m_menu;
				bool m_show;
				bool m_show_bag;
			public:
				explicit loot_filter_settings_toggle_menu(token);

				void toggle_filter_settings_click();
				void toggle_stats_settings_click();
				void toggle_help_click();
				void toggle_cube_click();
				void toggle_stash_click();
				void toggle_bag_click();
				void draw() override;

				bool key_event(uint32_t key, bool up) override;
			};
		}
	}
}
