#include <d2tweaks/client/modules/loot_filter/loot_filter_settings_toggle_menu.h>

#include <functional>

#include <d2tweaks/ui/controls/button.h>
#include <d2tweaks/client/client.h>
#include <diablo2/d2client.h>
#include <d2tweaks/ui/ui_manager.h>
#include <DllNotify.h>
#include <D2Template.h>


#include <diablo2/d2gfx.h>

#include <string>
#include <vector>
#include <sstream>

#include <string>
#include <locale>
#include <codecvt>

#include <cstdlib> // For system function

#include <shellapi.h> // For ShellExecute

using namespace d2_tweaks::client::modules;
bool m_stats_enabled = true;
bool m_help_enabled = false;

d2_tweaks::client::modules::loot_filter_settings_toggle_menu::loot_filter_settings_toggle_menu(token) {
	m_show = false;

	menu::set_enabled(true);
	menu::set_visible(true);

	//load_xml("d2tweaks\\interfaces\\loot_filter_settings_toggle_menu.xml");
	if (DLLBASE_D2EXPRES != 0)
		load_xml("d2tweaks\\interface_d2expres\\loot_filter_settings_toggle_menu.xml");
	if (DLLBASE_SGD2FREERES != 0)
		load_xml("d2tweaks\\interface_sgd2freeres\\loot_filter_settings_toggle_menu.xml");
	if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
		load_xml("d2tweaks\\interface_vanilla\\loot_filter_settings_toggle_menu.xml");

	m_toggle_filter_settings_btn = static_cast<ui::controls::button*>(get_control("m_toggle_filter_settings_btn"));
	m_toggle_filter_settings_btn->set_enabled(false);
	m_toggle_filter_settings_btn->set_visible(false);
	m_toggle_filter_settings_btn->set_on_click(std::bind(&loot_filter_settings_toggle_menu::toggle_filter_settings_click, this));

	m_filter_settings_menu = singleton<ui::ui_manager>::instance().get_menu("loot_filter_settings_menu");


	m_btn_toggle_stats = static_cast<ui::controls::button*>(get_control("m_toggle_stats"));
	m_btn_toggle_stats->set_enabled(true);
	m_btn_toggle_stats->set_visible(true);
	m_btn_toggle_stats->set_on_click(std::bind(&loot_filter_settings_toggle_menu::toggle_stats_settings_click, this));


	m_btn_toggle_help = static_cast<ui::controls::button*>(get_control("m_toggle_help"));
	m_btn_toggle_help->set_enabled(true);
	m_btn_toggle_help->set_visible(true);
	m_btn_toggle_help->set_on_click(std::bind(&loot_filter_settings_toggle_menu::toggle_help_click, this));

}

void d2_tweaks::client::modules::loot_filter_settings_toggle_menu::toggle_filter_settings_click() {
	m_show = !m_show;

	m_filter_settings_menu->set_enabled(m_show);
	m_filter_settings_menu->set_visible(m_show);

	m_btn_toggle_stats->set_enabled(true);
	m_btn_toggle_stats->set_visible(true);
}

void d2_tweaks::client::modules::loot_filter_settings_toggle_menu::toggle_stats_settings_click() {
	m_stats_enabled = !m_stats_enabled;
}

void d2_tweaks::client::modules::loot_filter_settings_toggle_menu::toggle_help_click() {
	//m_help_enabled = !m_help_enabled;
	// Open the default OS browser to the URL
	const std::string url = "https://im.stoned.io";
#ifdef _WIN32
	// For Windows
	ShellExecute(nullptr, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#elif defined __linux__ || defined __APPLE__
	// For Linux and macOS
	system(("xdg-open " + url).c_str());
#else
	// Unsupported platform
	// You can add handling for other platforms here
#endif

}

void d2_tweaks::client::modules::loot_filter_settings_toggle_menu::draw() {
	m_btn_toggle_stats->set_enabled(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INTERFACE));
	m_btn_toggle_stats->set_visible(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INTERFACE));
	m_toggle_filter_settings_btn->set_enabled(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INTERFACE));
	m_toggle_filter_settings_btn->set_visible(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INTERFACE));

	menu::draw();
}

bool d2_tweaks::client::modules::loot_filter_settings_toggle_menu::key_event(uint32_t key, bool up) {
	if (key == VK_ESCAPE && m_show) {
		m_show = false;

		m_filter_settings_menu->set_enabled(m_show);
		m_filter_settings_menu->set_visible(m_show);

		return true; //block escape key stroke
	}

	return menu::key_event(key, up);
}
