#include <d2tweaks/ui/ui_manager.h>

#include <Windows.h>
#include <algorithm>

#include <common/hooking.h>


#include <d2tweaks/common/protocol.h>


#include <d2tweaks/ui/menu.h>

#include <diablo2/d2win.h>
#include <diablo2/d2client.h>
#include <diablo2/d2common.h>
#include <diablo2/d2game.h>
#include <spdlog/spdlog.h>

#include <Windows.h>

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

diablo2::structures::unit* g_item1;

static LRESULT(__stdcall* g_wnd_proc_original)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

d2_tweaks::ui::ui_manager::ui_manager(token) {
	hooking::hook(diablo2::d2_win::get_base() + 0xD9B0, wnd_proc, reinterpret_cast<void**>(&g_wnd_proc_original));
}

void d2_tweaks::ui::ui_manager::add_menu(menu* m) {
	if (m == nullptr)
		return;

	const auto it = std::find(m_menus.begin(), m_menus.end(), m);

	if (it != m_menus.end())
		return;

	m_menus.push_back(m);
}

d2_tweaks::ui::menu* d2_tweaks::ui::ui_manager::get_menu(const std::string& name) {
	if (name.empty())
		return nullptr;

	//TODO: optimize somehow
	for (auto menu : m_menus) {
		if (menu->get_name() == name)
			return menu;
	}

	return nullptr;
}

void d2_tweaks::ui::ui_manager::remove_menu(menu* m) {
	if (m == nullptr)
		return;

	m_menus.erase(std::remove(m_menus.begin(), m_menus.end(), m), m_menus.end());
}

void d2_tweaks::ui::ui_manager::draw() {
	//process_inputs();

	for (auto menu : m_menus) {
		if (!menu->get_visible())
			continue;

		menu->draw();
	}
}

struct D2InventoryGridInfoStrc
{
	BYTE nGridX;                            //0x00
	BYTE nGridY;                            //0x01
	WORD pad0x02;                            //0x02
	int nGridLeft;                            //0x04
	int nGridRight;                            //0x08
	int nGridTop;                            //0x0C
	int nGridBottom;                        //0x10
	BYTE nGridBoxWidth;                        //0x14
	BYTE nGridBoxHeight;                    //0x15
	WORD pad0x16;                            //0x16
};



LRESULT d2_tweaks::ui::ui_manager::wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static auto& instance = singleton<ui_manager>::instance();

	bool block;

	// Send transmute packet
	if (wParam == 'X') {
		diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);
		block = true; // block the game from processing this key
	}

	// Send item move packet + transmute packet for certain codes
	if (wParam == 'Z') {
		// Call the item_click function using the function pointer
		const auto g_hoverItem = (*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4));

		if (g_hoverItem != 0) {
			char currentPage = diablo2::d2_common::get_item_page(g_hoverItem);

			// Create the packet
			static d2_tweaks::common::item_move_cs packet;
			packet.item_guid = g_hoverItem->guid;

			if (currentPage == 0) { //item is in inventory
				if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH))
					packet.target_page = 4;

				if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE))
					packet.target_page = 3;
			}
			else {
				packet.target_page = 0;
			}

			diablo2::d2_client::send_to_server(&packet, sizeof packet);
			(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
		}


		if (g_hoverItem != 0) {
			const auto record = diablo2::d2_common::get_item_record(g_hoverItem->data_record_index);
			char* normCode = record->string_code;
			if (strncmp(normCode, "gcv", 3) == 0 ||
				strncmp(normCode, "gcw", 3) == 0 ||
				strncmp(normCode, "gcg", 3) == 0 ||
				strncmp(normCode, "gcr", 3) == 0 ||
				strncmp(normCode, "gcb", 3) == 0 ||
				strncmp(normCode, "skc", 3) == 0 ||
				strncmp(normCode, "gcy", 3) == 0 ||
				strncmp(normCode, "gfv", 3) == 0 ||
				strncmp(normCode, "gfw", 3) == 0 ||
				strncmp(normCode, "gfg", 3) == 0 ||
				strncmp(normCode, "gfr", 3) == 0 ||
				strncmp(normCode, "gfb", 3) == 0 ||
				strncmp(normCode, "skf", 3) == 0 ||
				strncmp(normCode, "gfy", 3) == 0 ||
				strncmp(normCode, "gsv", 3) == 0 ||
				strncmp(normCode, "gsw", 3) == 0 ||
				strncmp(normCode, "gsg", 3) == 0 ||
				strncmp(normCode, "gsr", 3) == 0 ||
				strncmp(normCode, "gsb", 3) == 0 ||
				strncmp(normCode, "sku", 3) == 0 ||
				strncmp(normCode, "gsy", 3) == 0 ||
				strncmp(normCode, "gzv", 3) == 0 ||
				strncmp(normCode, "glw", 3) == 0 ||
				strncmp(normCode, "glg", 3) == 0 ||
				strncmp(normCode, "glr", 3) == 0 ||
				strncmp(normCode, "glb", 3) == 0 ||
				strncmp(normCode, "skl", 3) == 0 ||
				strncmp(normCode, "gly", 3) == 0 ||
				strncmp(normCode, "gpv", 3) == 0 ||
				strncmp(normCode, "gpw", 3) == 0 ||
				strncmp(normCode, "gpg", 3) == 0 ||
				strncmp(normCode, "gpr", 3) == 0 ||
				strncmp(normCode, "gpb", 3) == 0 ||
				strncmp(normCode, "skz", 3) == 0 ||
				strncmp(normCode, "gpy", 3) == 0 ||
				strncmp(normCode, "ib1", 3) == 0 ||
				// Runes
				strncmp(normCode, "r01", 3) == 0 ||
				strncmp(normCode, "r02", 3) == 0 ||
				strncmp(normCode, "r03", 3) == 0 ||
				strncmp(normCode, "r04", 3) == 0 ||
				strncmp(normCode, "r05", 3) == 0 ||
				strncmp(normCode, "r06", 3) == 0 ||
				strncmp(normCode, "r07", 3) == 0 ||
				strncmp(normCode, "r08", 3) == 0 ||
				strncmp(normCode, "r09", 3) == 0 ||
				strncmp(normCode, "r10", 3) == 0 ||
				strncmp(normCode, "r11", 3) == 0 ||
				strncmp(normCode, "r12", 3) == 0 ||
				strncmp(normCode, "r13", 3) == 0 ||
				strncmp(normCode, "r14", 3) == 0 ||
				strncmp(normCode, "r15", 3) == 0 ||
				strncmp(normCode, "r16", 3) == 0 ||
				strncmp(normCode, "r17", 3) == 0 ||
				strncmp(normCode, "r18", 3) == 0 ||
				strncmp(normCode, "r19", 3) == 0 ||
				strncmp(normCode, "r20", 3) == 0 ||
				strncmp(normCode, "r21", 3) == 0 ||
				strncmp(normCode, "r22", 3) == 0 ||
				strncmp(normCode, "r23", 3) == 0 ||
				strncmp(normCode, "r24", 3) == 0 ||
				strncmp(normCode, "r25", 3) == 0 ||
				strncmp(normCode, "r26", 3) == 0 ||
				strncmp(normCode, "r27", 3) == 0 ||
				strncmp(normCode, "r28", 3) == 0 ||
				strncmp(normCode, "r29", 3) == 0 ||
				strncmp(normCode, "r30", 3) == 0 ||
				strncmp(normCode, "r31", 3) == 0 ||
				strncmp(normCode, "r32", 3) == 0 ||
				strncmp(normCode, "r33", 3) == 0 ||
				strncmp(normCode, "ib2", 3) == 0
				
				) {

				char currentPage = diablo2::d2_common::get_item_page(g_hoverItem);

				// Create the packet
				static d2_tweaks::common::item_move_cs packet;
				packet.item_guid = g_hoverItem->guid;

				if (currentPage == 0) { //item is in inventory
					if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH))
						packet.target_page = 4;

					if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE))
						packet.target_page = 3;
				}
				else {
					packet.target_page = 0;
				}

				diablo2::d2_client::send_to_server(&packet, sizeof packet);
				(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
				diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);
			}
		}

		block = true; // block the game from processing this key
	}

	if (wParam == 'G') {
		const auto g_hoverItem = *reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4);
		if (g_hoverItem != nullptr) {
			const auto record = diablo2::d2_common::get_item_record(g_hoverItem->data_record_index);
			char* normCode = record->string_code;			

			const auto player = diablo2::d2_client::get_local_player();

			std::vector<diablo2::structures::unit*> items;
			diablo2::structures::unit* gemBag{};

			// get the gembag item
			for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
				const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
				if (record->type == 101) {
					gemBag = item;
				}
			}

			// 378 is ruby
			// 379 is amathyst
			// 380 is diamond
			// 381 is emerald
			// 382 is saphhire
			// 383 is topaz
			// 384 is skull
				
			// 385 is El Runes
			// 386 is Sol Runes
			// 387 is Mal Runes

			// properties.txt row number - 3

			if (strncmp(normCode, "gcv", 3) == 0) { // Use strncmp for string comparison
				int32_t stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);				
				MessageBoxA(0, std::to_string(stat).c_str(), "stat", 0);

				stat++;


				D2PropertyStrc itemProperty = {};

				itemProperty.nProperty = 382 - 3;
				itemProperty.nLayer = 0;
				itemProperty.nMin = 1;
				itemProperty.nMax = 1;


				//diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, stat, 0);
				diablo2::d2_common::add_property(gemBag, &itemProperty, 0);



				
				int32_t stat1 = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);
				MessageBoxA(0, std::to_string(stat1).c_str(), "stat", 0);
			}




			// check to see if gembag is actually right
			//const auto bagrecord = diablo2::d2_common::get_item_record(gemBag->data_record_index);
			//char* bagcode = bagrecord->string_code;
			//MessageBoxA(0, bagcode, "bag code", 0);


			//if (normCode == "gcv") {
			//	int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);
			//	diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, stat + 1, 0);
			//	MessageBoxA(0, normCode, "normCode", 0);
			//}




		}




		/*
		for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
			const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

			if (record->type == 101) {
				gemBag = item;
				MessageBoxA(0, "Gem Bag Found", "Gem Bag Found", 0);

				const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

				char* normCode = record->string_code;

				MessageBoxA(0, normCode, "normCode", 0);

				if (normCode == "gcv") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, stat + 1, 0);
					MessageBoxA(0, normCode, "normCode", 0);
				}
				else if (normCode == "gcy") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Topaz, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Topaz, stat + 1, 0);
				}
				else if (normCode == "gcb") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Sapphire, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Sapphire, stat + 1, 0);
				}
				else if (normCode == "gcr") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Ruby, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Ruby, stat + 1, 0);
				}
				else if (normCode == "gcg") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Emerald, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Emerald, stat + 1, 0);
				}
				else if (normCode == "gcw") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Diamond, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Diamond, stat + 1, 0);
				}
				else if (normCode == "skc") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Skull, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Skull, stat + 1, 0);
				}



				if (normCode == "gfv") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, stat + 3, 0);
				}
				else if (normCode == "gfy") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Topaz, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Topaz, stat + 3, 0);
				}
				else if (normCode == "gfb") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Sapphire, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Sapphire, stat + 3, 0);
				}
				else if (normCode == "gfr") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Ruby, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Ruby, stat + 3, 0);
				}
				else if (normCode == "gfg") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Emerald, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Emerald, stat + 3, 0);
				}
				else if (normCode == "gfw") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Diamond, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Diamond, stat + 3, 0);
				}
				else if (normCode == "skf") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Skull, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Skull, stat + 3, 0);
				}


				if (normCode == "gsv") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, stat + 9, 0);
				}
				else if (normCode == "gsy") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Topaz, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Topaz, stat + 9, 0);
				}
				else if (normCode == "gsb") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Sapphire, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Sapphire, stat + 9, 0);
				}
				else if (normCode == "gsr") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Ruby, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Ruby, stat + 9, 0);
				}
				else if (normCode == "gsg") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Emerald, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Emerald, stat + 9, 0);
				}
				else if (normCode == "gsw") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Diamond, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Diamond, stat + 9, 0);
				}
				else if (normCode == "sku") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Skull, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Skull, stat + 9, 0);
				}


				if (normCode == "gzv") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, stat + 27, 0);
				}
				else if (normCode == "gly") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Topaz, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Topaz, stat + 27, 0);
				}
				else if (normCode == "glb") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Sapphire, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Sapphire, stat + 27, 0);
				}
				else if (normCode == "glr") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Ruby, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Ruby, stat + 27, 0);
				}
				else if (normCode == "glg") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Emerald, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Emerald, stat + 27, 0);
				}
				else if (normCode == "glw") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Diamond, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Diamond, stat + 27, 0);
				}
				else if (normCode == "skl") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Skull, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Skull, stat + 27, 0);
				}


				if (normCode == "gpv") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, stat + 81, 0);
				}
				else if (normCode == "gpy") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Topaz, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Topaz, stat + 81, 0);
				}
				else if (normCode == "gpb") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Sapphire, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Sapphire, stat + 81, 0);
				}
				else if (normCode == "gpr") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Ruby, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Ruby, stat + 81, 0);
				}
				else if (normCode == "gpg") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Emerald, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Emerald, stat + 81, 0);
				}
				else if (normCode == "gpw") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Diamond, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Diamond, stat + 81, 0);
				}
				else if (normCode == "skz") {
					int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Skull, NULL);
					diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Skull, stat + 81, 0);
				}

			}

		}
		*/
	}



	switch (msg) {
		case WM_LBUTTONDOWN:
		{
			block = instance.process_left_mouse(false);
			break;
		}

		case WM_LBUTTONUP:
		{
			block = instance.process_left_mouse(true);
			break;
		}

		case WM_RBUTTONDOWN:
		{

			const auto g_hoverItem = *reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4);
			if (g_hoverItem != nullptr) {
				const auto record = diablo2::d2_common::get_item_record(g_hoverItem->data_record_index);
				char* normCode = record->string_code;

				const auto player = diablo2::d2_client::get_local_player();


				const auto pInventory = player->inventory;

				std::vector<diablo2::structures::unit*> items;
				diablo2::structures::unit* gemBag{};

				// get the gembag item
				for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
					const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
					if (record->type == 101) {
						gemBag = item;
					}
				}

				// 378 is ruby
				// 379 is amathyst
				// 380 is diamond
				// 381 is emerald
				// 382 is saphhire
				// 383 is topaz
				// 384 is skull

				// 385 is El Runes
				// 386 is Sol Runes
				// 387 is Mal Runes

				// properties.txt row number - 3

				if (strncmp(normCode, "gcv", 3) == 0) { // Use strncmp for string comparison
					int32_t stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);
					//MessageBoxA(0, std::to_string(stat).c_str(), "stat", 0);

					stat++;


					D2PropertyStrc itemProperty = {};

					itemProperty.nProperty = 382 - 3;
					itemProperty.nLayer = 0;
					itemProperty.nMin = 1;
					itemProperty.nMax = 1;


					//diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, stat, 0);
					diablo2::d2_common::add_property(gemBag, &itemProperty, 0);


					diablo2::d2_common::inv_remove_item(pInventory, g_hoverItem);



					int32_t stat1 = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);
					//MessageBoxA(0, std::to_string(stat1).c_str(), "stat", 0);
				}




				// check to see if gembag is actually right
				//const auto bagrecord = diablo2::d2_common::get_item_record(gemBag->data_record_index);
				//char* bagcode = bagrecord->string_code;
				//MessageBoxA(0, bagcode, "bag code", 0);


				//if (normCode == "gcv") {
				//	int stat = diablo2::d2_common::get_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, NULL);
				//	diablo2::d2_common::set_stat(gemBag, diablo2::unit_stats_t::UNIT_STAT_gembag_Amethyst, stat + 1, 0);
				//	MessageBoxA(0, normCode, "normCode", 0);
				//}




			}



			block = instance.process_right_mouse(false);
			break;
		}

		case WM_RBUTTONUP:
		{
			block = instance.process_right_mouse(true);
			break;
		}

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			block = instance.process_key_event(wParam, false);
			break;
		}

		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			block = instance.process_key_event(wParam, true);
			break;
		}

		default: return g_wnd_proc_original(hWnd, msg, wParam, lParam);
	}

	if (block)
		return 0;

	return g_wnd_proc_original(hWnd, msg, wParam, lParam);
}

void d2_tweaks::ui::ui_manager::process_inputs() {
	if (GetAsyncKeyState(VK_LBUTTON)) {
		if (!m_was_down_before_left) {
			m_was_down_before_left = true;
			m_mouse_state_left = true;

			process_left_mouse(false);
		}
	} else if (m_was_down_before_left) {
		m_was_down_before_left = false;
		m_mouse_state_left = false;

		process_left_mouse(true);
	}

	if (GetAsyncKeyState(VK_RBUTTON)) {
		if (!m_was_down_before_right) {
			m_was_down_before_right = true;
			m_mouse_state_right = true;

			process_right_mouse(false);
		}
	} else if (m_was_down_before_right) {
		m_was_down_before_right = false;
		m_mouse_state_right = false;

		process_right_mouse(true);
	}
}

bool d2_tweaks::ui::ui_manager::process_left_mouse(bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->left_mouse(up);
	}

	return block;
}

bool d2_tweaks::ui::ui_manager::process_right_mouse(bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->right_mouse(up);
	}

	return block;
}

bool d2_tweaks::ui::ui_manager::process_key_event(uint32_t key, bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->key_event(key, up);
	}

	return block;
}
