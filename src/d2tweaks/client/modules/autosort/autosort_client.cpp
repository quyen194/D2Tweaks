#include <d2tweaks/client/modules/autosort/autosort_client.h>

#include <d2tweaks/client/client.h>

#include <spdlog/spdlog.h>


#include <d2tweaks/common/common.h>
#include <d2tweaks/common/protocol.h>
#include <d2tweaks/common/asset_manager.h>

#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>
#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/controls/button.h>

#include <diablo2/d2common.h>
#include <diablo2/d2client.h>
#include <diablo2/d2win.h>
#include <diablo2/d2gfx.h>
#include <diablo2/d2cmp.h>


#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/player_data.h>

#include <diablo2/structures/path.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <time.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <map>


#include <DllNotify.h>
#include <D2Template.h>

MODULE_INIT(autosort)

enum ColorEnum {
	RED = diablo2::ui_color_t::UI_COLOR_RED,
	LIGHT_GREEN = diablo2::ui_color_t::UI_COLOR_LIGHT_GREEN,
	BLUE = diablo2::ui_color_t::UI_COLOR_BLUE,
	DARK_GOLD = diablo2::ui_color_t::UI_COLOR_DARK_GOLD,
	GREY = diablo2::ui_color_t::UI_COLOR_GREY,
	BLACK = diablo2::ui_color_t::UI_COLOR_BLACK,
	GOLD = diablo2::ui_color_t::UI_COLOR_GOLD,
	ORANGE = diablo2::ui_color_t::UI_COLOR_ORANGE,
	YELLOW = diablo2::ui_color_t::UI_COLOR_YELLOW,
	DARK_GREEN = diablo2::ui_color_t::UI_COLOR_DARK_GREEN,
	PURPLE = diablo2::ui_color_t::UI_COLOR_PURPLE,
	GREEN = diablo2::ui_color_t::UI_COLOR_GREEN,
	WHITE = diablo2::ui_color_t::UI_COLOR_WHITE,
	BLACK2 = diablo2::ui_color_t::UI_COLOR_BLACK2,
	DARK_WHITE = diablo2::ui_color_t::UI_COLOR_DARK_WHITE,
	LIGHT_GREY = diablo2::ui_color_t::UI_COLOR_LIGHT_GREY
};

// Mapping from int to ui_font_t
std::map<int, diablo2::ui_font_t> fontMap = {
	{0, diablo2::ui_font_t::UI_FONT_8},
	{1, diablo2::ui_font_t::UI_FONT_16},
	{2, diablo2::ui_font_t::UI_FONT_30},
	{3, diablo2::ui_font_t::UI_FONT_42},
	{4, diablo2::ui_font_t::UI_FONT_FORMAL10},
	{5, diablo2::ui_font_t::UI_FONT_FORMAL12},
	{6, diablo2::ui_font_t::UI_FONT_6},
	{7, diablo2::ui_font_t::UI_FONT_24},
	{8, diablo2::ui_font_t::UI_FONT_FORMAL11},
	{9, diablo2::ui_font_t::UI_FONT_EXOCET10},
	{10, diablo2::ui_font_t::UI_FONT_RIDICULOUS},
	{11, diablo2::ui_font_t::UI_FONT_EXOCET8},
	{12, diablo2::ui_font_t::UI_FONT_REALLYTHELASTSUCKER},
	{13, diablo2::ui_font_t::UI_FONT_INGAMECHAT}
};


class inventory_sort_menu : public d2_tweaks::ui::menu {
	d2_tweaks::common::asset* m_buttons_img;

	d2_tweaks::ui::controls::button* m_sort_inventory_btn;
	d2_tweaks::ui::controls::button* m_sort_stash_btn;
	d2_tweaks::ui::controls::button* m_sort_cube_btn;
public:
	inventory_sort_menu() {
		menu::set_enabled(true);
		menu::set_visible(true);

		//load_xml("d2tweaks\\interfaces\\autosort.xml");
		if (DLLBASE_D2EXPRES != 0)
			load_xml("d2tweaks\\interface_d2expres\\autosort.xml");
		if (DLLBASE_SGD2FREERES != 0)
			load_xml("d2tweaks\\interface_sgd2freeres\\autosort.xml");
		if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
			load_xml("d2tweaks\\interface_vanilla\\autosort.xml");

		m_buttons_img = singleton<d2_tweaks::common::asset_manager>::instance().get_mpq_file("d2tweaks\\assets\\buttons", d2_tweaks::common::MPQ_FILE_TYPE_DC6);

		m_sort_inventory_btn = get_button("m_sort_inventory_btn", std::bind(&inventory_sort_menu::sort_inventory_click, this));
		m_sort_stash_btn = get_button("m_sort_stash_btn", std::bind(&inventory_sort_menu::sort_stash_click, this));
		m_sort_cube_btn = get_button("m_sort_cube_btn", std::bind(&inventory_sort_menu::sort_cube_click, this));
	}



	// Function to find the Diablo II window handle
	HWND FindDiabloIIWindow() {
		return FindWindow(NULL, "Diablo II");
	}

	// Function to draw a filled rectangle using GDI
	void DrawFilledRect(HWND hwnd, int left, int top, int right, int bottom, COLORREF color) {
		HDC hdc = GetDC(hwnd);
		HBRUSH hBrush = CreateSolidBrush(color);
		RECT rect = { left, top, right, bottom };
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);
		ReleaseDC(hwnd, hdc);
	}




	int statsFont = GetPrivateProfileIntA("Options", "statsFont", 0, "./d2tweaks.ini");



	void draw() override {
		auto stats = globalStatsVector;
		int textOffset = 40; // Initial offset for the first line

		const auto player = diablo2::d2_client::get_local_player();

		// Add all items to vector
		std::vector<diablo2::structures::unit*> items;
		for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
			const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
			if (record->type == 104 || record->type == 123) {
				items.push_back(item);
			}
		}

		// Initialize statValue
		int32_t statValue = 0;

		if (m_stats_enabled) {
			for (const auto& stat : stats) {

				double param = 6;

				int32_t spirits = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(185), NULL);
				int32_t soulscaptured = statValue = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(184), NULL);

				switch (stat.stat) {
					// 2. (statValue <- this is probably op stat1 ? * baseValue <- this is probably op base ) / 2 ^ param

					// (op stat1 value *  base stat value) / (2 ^ param) 
					// let's try this fucking thing

				case 190: {
					// str/spirits
					statValue = static_cast<int32_t>((1 * spirits) / pow(2, param)); // what is the value of opStat_str
					break;
				}
				case 191: {
					// dex/spirits
					statValue = static_cast<int32_t>((1 * spirits) / pow(2, param)); // what is the value of opStat_str
					break;
				}
				case 192: {
					// vit/spirits
					statValue = static_cast<int32_t>((1 * spirits) / pow(2, param)); // what is the value of opStat_str
					break;
				}
				case 193: {
					// enr/spirits
					statValue = static_cast<int32_t>((1 * spirits) / pow(2, param)); // what is the value of opStat_str
					break;
				}
				case 200: {
					// skills/souls
					param = 8;
					statValue = static_cast<int32_t>((1 * soulscaptured) / pow(2, param)); // what is the value of opStat_str
					break;
				}

				case 301: {
					for (auto item : items) {
						const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
						if (record->type == 104) {
							statValue = diablo2::d2_common::get_stat(item, static_cast<diablo2::unit_stats_t>(stat.stat), NULL);
						}
					}
					break;
				}

				case 304: {
					for (auto item : items) {
						const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
						if (record->type == 104) {
							statValue = diablo2::d2_common::get_stat(item, static_cast<diablo2::unit_stats_t>(stat.stat), NULL);
						}
					}
					break;
				}

				default: {
					// By default, get player stats
					statValue = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(stat.stat), NULL);
					break;

				}
				}

				/*
				int32_t diablo2::d2_common::set_stat(structures::unit* unit, unit_stats_t stat, uint32_t value, int16_t param) {
						static wrap_func_std_import<int32_t(structures::unit*, int32_t, int32_t, int32_t)> set_stat(10517, get_base());
						return set_stat(unit, stat, value, param);
					}

				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(randStatRangeLow, randStatRangeHigh);
				unsigned int randomNumber = dis(gen);

				std::random_device rdb;
				std::mt19937 genb(rdb());
				std::uniform_int_distribution<> randBool(1, 2);
				unsigned int randomBool = randBool(genb) - 1;

				if (stat.is_item_stat == 1) {
					for (auto item : items) {
						const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
						int RandStatValue = diablo2::d2_common::get_stat(item, static_cast<diablo2::unit_stats_t>(randStat), NULL);

						if (record->type == stat.item_type_id && RandStatValue != 0) {
							// set randStat value to random number 1 and 2^(32) = 4294967296
							diablo2::d2_common::set_stat(item, static_cast<diablo2::unit_stats_t>(randStat), randomNumber, 0);
							diablo2::d2_common::set_stat(item, static_cast<diablo2::unit_stats_t>(randStatBool), randomBool, 0);
						}
					}
				}
				else {
					// set randStat value to random number 1 and 2^(32) = 4294967296
					//diablo2::d2_common::set_stat(player, static_cast<diablo2::unit_stats_t>(randStat), randomNumber, 0);
					//diablo2::d2_common::set_stat(player, static_cast<diablo2::unit_stats_t>(randStatBool), randomBool, 0);

					int statValue1 = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(randStat), NULL);
					int statValue2 = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(randStatBool), NULL);

					if (statValue1 > 0 ) {
						diablo2::d2_common::set_stat(player, static_cast<diablo2::unit_stats_t>(randStat), 0, 0);
						diablo2::d2_common::set_stat(player, static_cast<diablo2::unit_stats_t>(randStatBool), 0, 0);
					}

				}
				*/
				auto statValueStr = std::to_wstring(statValue);
				std::wstring statText = std::wstring(stat.stat_display_string);// .append(L" " + statValueStr);

				if (!diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CHARACTER)
					// && !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY)
					// && !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_SKILL)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CHAT)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_NPCMENU)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MAINMENU)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CONFIG)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_NPCSHOP)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_ANVIL)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_QUEST)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_QUESTLOG)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STATUSAREA)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_WPMENU)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_PARTY)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_TRADE)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MSGS)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_BELT)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_HELP)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MERC)
					&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_SCROLL)) {

					// Draw stats
					diablo2::d2_win::set_current_font(fontMap[statsFont]); // Set font to FONT16
					diablo2::d2_win::draw_text(const_cast<wchar_t*>(statText.c_str()), stat.x1, stat.y1 + textOffset, stat.colorStat, 0);

					diablo2::d2_win::set_current_font(fontMap[statsFont]); // Set font to FONT16
					diablo2::d2_win::draw_text(const_cast<wchar_t*>(statValueStr.c_str()), stat.x2, stat.y2 + textOffset, stat.colorStatValue, 0);


					//diablo2::d2_win::draw_boxed_text(const_cast<wchar_t*>(statText.c_str()), stat.x1, stat.y1 + textOffset, 1, 0, stat.colorStat);
					//diablo2::d2_win::draw_boxed_text(const_cast<wchar_t*>(statValueStr.c_str()), stat.x2, stat.y2 + textOffset, 1, 4, stat.colorStatValue);

					// diablo2::d2_win::set_current_font(diablo2::UI_FONT_16); // Set font to FONT16


					//diablo2::structures::d2_cmp::init_gfx_data(&g_gfxdata);

					//diablo2::d2_gfx::draw_image(&g_gfxdata, 200, 200, 1, 5, 0);

					// instead try to load direct jpg with gdi and insetad ofloading jpg file, specify it bb64 encoded and decode it.

					diablo2::ui_color_t::UI_COLOR_WHITE;


					// print player health, mana, and stamina bars, lastexp, nextexp, and level
					// Get current HP, Mana, and Stamina along with their maximum values
					int statHP = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(6), NULL) / 256;
					int statMaxHP = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(7), NULL) / 256;
					int statMana = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(8), NULL) / 256;
					int statMaxMana = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(9), NULL) / 256;
					int statStamina = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(10), NULL) / 256;
					int statMaxStamina = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(11), NULL) / 256;

					// Convert the integer values to wide character strings
					std::wstring strHP = std::to_wstring(statHP);
					std::wstring strMaxHP = std::to_wstring(statMaxHP);
					std::wstring strMana = std::to_wstring(statMana);
					std::wstring strMaxMana = std::to_wstring(statMaxMana);
					std::wstring strStamina = std::to_wstring(statStamina);
					std::wstring strMaxStamina = std::to_wstring(statMaxStamina);

					diablo2::d2_win::set_current_font(diablo2::UI_FONT_16); // Set font to FONT16

					// Calculate the percentages of current HP, Mana, and Stamina
					float healthPercentage = static_cast<float>(statHP) / static_cast<float>(statMaxHP);
					float manaPercentage = static_cast<float>(statMana) / static_cast<float>(statMaxMana);
					float staminaPercentage = static_cast<float>(statStamina) / static_cast<float>(statMaxStamina);


					int sHeight = 768;
					int sWidth = 1280;

					int sHCenter = sHeight / 2;
					int sWCenter = sWidth / 2;

					// Define the dimensions for the bars
					int barWidth = 200; // Width of the bars
					int barHeight = 16; // Height of the bars

					// Define the coordinates for the bars
					int barX = sWCenter - 100; // Left coordinate of the bars
					int barY_HP = 654; // Top coordinate of the HP bar
					int barY_Mana = barY_HP + barHeight + 4; // Top coordinate of the Mana bar with separator
					int barY_Stamina = barY_Mana + barHeight + 4; // Top coordinate of the Stamina bar with separator

					std::wstring life = L"L: " + strHP + L" / " + strMaxHP;
					std::wstring mana = L"M: " + strMana + L" / " + strMaxMana;
					std::wstring stamina = L"" + strStamina + L" / " + strMaxStamina;

					int lifeWidth = diablo2::d2_win::get_text_pixel_width(const_cast<wchar_t*>(life.c_str()));
					int manaWidth = diablo2::d2_win::get_text_pixel_width(const_cast<wchar_t*>(mana.c_str()));
					int staminaWidth = diablo2::d2_win::get_text_pixel_width(const_cast<wchar_t*>(stamina.c_str()));


					// Calculate the filled widths of the bars
					int filledHPWidth = static_cast<int>(healthPercentage * barWidth);
					int filledManaWidth = static_cast<int>(manaPercentage * barWidth);
					int filledStaminaWidth = static_cast<int>(staminaPercentage * barWidth);

					HWND diabloIIWnd = FindDiabloIIWindow();

					// Draw the filled HP bar
					diablo2::d2_gfx::draw_filled_rect(barX, barY_HP, barX + filledHPWidth, barY_HP + barHeight, 10, 255);
					//DrawFilledRect(diabloIIWnd, barX, barY_HP, barX + filledHPWidth, barY_HP + barHeight, RGB(255, 0, 0)); // Red color for HP
					diablo2::d2_win::draw_text(const_cast<wchar_t*>(life.c_str()), sWCenter - (lifeWidth/2), barY_HP + 15, diablo2::ui_color_t::UI_COLOR_GOLD, 0);
					
					// Draw the filled Mana bar
					diablo2::d2_gfx::draw_filled_rect(barX, barY_Mana, barX + filledManaWidth, barY_Mana + barHeight, 156, 255);
					//DrawFilledRect(diabloIIWnd, barX, barY_Mana, barX + filledManaWidth, barY_Mana + barHeight, RGB(100, 100, 255)); // Blue color for Mana
					diablo2::d2_win::draw_text(const_cast<wchar_t*>(mana.c_str()), sWCenter - (lifeWidth / 2), barY_Mana + 15, diablo2::ui_color_t::UI_COLOR_DARK_WHITE, 0);

					/*
					// Define the number of separators
					int numColors = 256;
					int numColumns = 4;
					int colorsPerColumn = numColors / numColumns;
					int separatorHeight = 1; // Height of each separator
					int columnOffset = 200; // Offset for each column

					// Iterate over each column
					for (int column = 0; column < numColumns; ++column) {
						// Calculate the starting X coordinate for this column
						int columnX = barX + (column * columnOffset);

						// Draw the filled Mana bars for this column
						for (int i = 0; i < colorsPerColumn; ++i) {
							int colorIndex = column * colorsPerColumn + i;
							int separatorY = barY_Mana + barHeight + i * (separatorHeight + barHeight);

							// Draw the filled Mana bar
							diablo2::d2_gfx::draw_filled_rect(columnX, separatorY, columnX + filledManaWidth, separatorY + barHeight, colorIndex, 255);

							// Draw the index number at the same coordinates as the bar
							std::wstring indexStr = std::to_wstring(colorIndex);
							diablo2::d2_win::draw_text(const_cast<wchar_t*>(indexStr.c_str()), columnX - 20, separatorY, diablo2::UI_COLOR_DARK_GOLD, 0);
						}
					}
					*/





					// Draw the filled Stamina bar
					// diablo2::d2_gfx::draw_filled_rect(barX, barY_Stamina, barX + filledStaminaWidth, barY_Stamina + barHeight, 12, 255);
					//DrawFilledRect(diabloIIWnd, barX, barY_Stamina, barX + filledStaminaWidth, barY_Stamina + barHeight, RGB(255, 255, 0)); // Green color for Stamina
					
					int statLevel = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(12), NULL);
					int statLastExp = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(29), NULL);
					int statNextExp = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(30), NULL);

					diablo2::d2_win::set_current_font(diablo2::UI_FONT_16); // Set font to FONT16


				}
			}
		}
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

		m_sort_stash_btn->set_enabled(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH));
		m_sort_stash_btn->set_visible(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH));

		m_sort_cube_btn->set_enabled(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE));
		m_sort_cube_btn->set_visible(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE));

		menu::draw();
	}

private:
	static bool should_draw() {
		return diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY) ||
			diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH) ||
			diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE) ||
			diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_NPCSHOP);
	}

	d2_tweaks::ui::controls::button* get_button(const std::string& name, const std::function<void()>& onClick) {
		auto result = static_cast<d2_tweaks::ui::controls::button*>(get_control(name));

		result->set_on_click(onClick);

		result->set_enabled(false);
		result->set_visible(false);

		return result;
	}

	void sort_inventory_click() {
		request_sort(0x00);
	}

	void sort_stash_click() {
		request_sort(0x04);
	}

	void sort_cube_click() {
		request_sort(0x03);
	}

	void request_sort(const uint8_t page) {
		static d2_tweaks::common::inventory_sort_cs packet;

		const auto player = diablo2::d2_client::get_local_player();

		std::vector<diablo2::structures::unit*> items;

		for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
			if (item->item_data->page == page)
				items.push_back(item);
		}

		for (auto item : items)
			diablo2::d2_common::inv_remove_item(player->inventory, item);

		packet.page = page;
		diablo2::d2_client::send_to_server(&packet, sizeof packet);
	}
};


void d2_tweaks::client::modules::autosort::init_early() {

}


void d2_tweaks::client::modules::autosort::init() {
	char acPathToIni[MAX_PATH] = { 0 };
	const char* pcIniFile = "\\d2tweaks.ini";

	GetCurrentDirectory(MAX_PATH, acPathToIni);
	lstrcat(acPathToIni, pcIniFile);

	if (GetPrivateProfileInt("modules", "Autosort", 1, acPathToIni) != FALSE) {
		singleton<ui::ui_manager>::instance().add_menu(new inventory_sort_menu());
		singleton<client>::instance().register_packet_handler(common::MESSAGE_TYPE_INVENTORY_SORT, this);
	}
}

void d2_tweaks::client::modules::autosort::handle_packet(common::packet_header* packet) {
	static auto& instance = singleton<client>::instance();
	const auto inventorySort = static_cast<common::inventory_sort_sc*>(packet);

	const auto item = instance.get_client_unit(0x04, inventorySort->guid); //0x03 -> 0x04 - item

	if (item == nullptr)
		return;;

	const auto player = diablo2::d2_client::get_local_player();

	//Last parameter is some boolean
	const auto inventoryIndex = diablo2::d2_common::get_inventory_index(player, inventorySort->page, diablo2::d2_client::is_lod());

	item->item_data->page = inventorySort->page;

	diablo2::d2_common::inv_add_item(player->inventory, item, inventorySort->tx, inventorySort->ty, inventoryIndex, true, item->item_data->page);
	diablo2::d2_common::inv_update_item(player->inventory, item, true);
}

