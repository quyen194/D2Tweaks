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
#include <map>

#include <iomanip> // For std::setw
#include <sstream>
#include <stdexcept> // For std::invalid_argument

#include <iostream>
#include <vector>
#include <string>
#include <CommCtrl.h> // Include for edit control

#include <d2tweaks/client/client.h>

#include <chrono> // Include chrono for time-based operations

// Define a static variable to keep track of the last time the stash window was toggled
static std::chrono::steady_clock::time_point lastToggleTime;


#pragma pack(push, 1)

using namespace std;

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

// Declare a variable to hold the handle to the edit box control
HWND g_hEditBox = nullptr;
// Declare a variable to store the input text
std::vector<char> g_inputText(256, '\0'); // Adjust the size according to your needs

// Function to read the first line from the file and extract hex bytes after ':'
std::string readFirstLineAndExtractHex(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << fileName << std::endl;
		return "";
	}

	std::string line;
	std::getline(file, line); // Read the first line from the file
	file.close(); // Close the file

	// Find the position of ':' in the line
	size_t colonPos = line.find(':');
	if (colonPos == std::string::npos) {
		std::cerr << "Colon not found in the first line of file: " << fileName << std::endl;
		return "";
	}

	// Extract hex bytes after ':' and return
	std::string hexBytes = line.substr(colonPos + 1);
	return hexBytes;
}

// Function to delete the first line from the file
void deleteFirstLine(const std::string& fileName) {
	std::ifstream inFile(fileName);
	if (!inFile.is_open()) {
		std::cerr << "Error opening file: " << fileName << std::endl;
		return;
	}

	// Read the contents of the file except the first line
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(inFile, line)) {
		lines.push_back(line);
	}
	inFile.close();

	// Write the contents back to the file, excluding the first line
	std::ofstream outFile(fileName);
	if (!outFile.is_open()) {
		std::cerr << "Error opening file: " << fileName << std::endl;
		return;
	}
	for (size_t i = 1; i < lines.size(); ++i) {
		outFile << lines[i] << std::endl;
	}
	outFile.close();
}

//// Define your deserialization method for the item structure
//diablo2::structures::unit unserialize_item(const std::string& itemcode, std::ifstream& file) {
//	// Read each line from the file
//	std::string line;
//	while (std::getline(file, line)) {
//		// Split the line into item code and serialized data
//		std::istringstream iss(line);
//		std::string code;
//		std::getline(iss, code, ':');
//		if (code == itemcode) {
//			// Found matching item code, extract serialized data
//			std::string serializedData;
//			std::getline(iss, serializedData);
//			// Convert serialized data from hexadecimal string to binary
//			std::istringstream hexStream(serializedData);
//			diablo2::structures::unit item;
//			for (size_t i = 0; i < sizeof(item); ++i) {
//				int byte;
//				if (!(hexStream >> std::hex >> byte)) {
//					throw std::invalid_argument("Error reading serialized data");
//				}
//				reinterpret_cast<char*>(&item)[i] = static_cast<char>(byte);
//			}
//			return item;
//		}
//	}
//	// Item code not found
//	throw std::invalid_argument("Item code not found");
//}

#include <stdexcept>

// Unserialize the hexadecimal string to an item structure
diablo2::structures::unit unserialize_item(const std::string& hexString) {
	if (hexString.size() % 2 != 0) {
		throw std::invalid_argument("Invalid hex string length");
	}

	// Create an item object
	diablo2::structures::unit item{};

	// Convert the hexadecimal string to binary data
	for (size_t i = 0; i < hexString.size(); i += 2) {
		std::string byteStr = hexString.substr(i, 2);
		unsigned int byte;
		std::istringstream(byteStr) >> std::hex >> byte;
		reinterpret_cast<unsigned char*>(&item)[i / 2] = static_cast<unsigned char>(byte);
	}

	return item;
}

LRESULT d2_tweaks::ui::ui_manager::wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static auto& instance = singleton<ui_manager>::instance();

	bool block;

	// Send transmute packet
	if (wParam == 'X') {
		diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);
		block = true; // block the game from processing this key
	}

	if (wParam == 'V') {
		// Define a cooldown duration in milliseconds
		constexpr int cooldownDuration = 500; // Adjust this value as needed

		// Get the current time
		auto currentTime = std::chrono::steady_clock::now();

		// Calculate the time elapsed since the last stash toggle
		auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastToggleTime).count();

		// Check if enough time has elapsed since the last toggle
		if (timeElapsed >= cooldownDuration) {
			// Update the last toggle time
			lastToggleTime = currentTime;

			if (!diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH)) {
				// Code to open stash
				const auto player = diablo2::d2_client::get_local_player();
				int32_t st0Guid = 0;
				uint32_t st0X = 0;
				uint32_t st0Y = 0;
				diablo2::structures::unit* box{};
				for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
					const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
					char* st0Code = record->string_code;
					if (strncmp(st0Code, "st0", 3) == 0) {
						box = item;
						st0Guid = box->guid;
						st0X = player->path->mapx;
						st0Y = player->path->mapy;
					}
				}
				struct D2GSPacketClt20 {
					uint8_t PacketId; // 0x01
					uint32_t guid;    // 0x06
					uint32_t tx;    // 0x07
					uint32_t ty;    // 0x09
				};
				D2GSPacketClt20 packet;
				packet.PacketId = 0x20;
				packet.guid = st0Guid;
				packet.tx = st0X;
				packet.ty = st0Y;
				diablo2::d2_client::send_to_server(&packet, sizeof packet);
				block = true;

				// MessageBoxA(NULL, "Stash opened", "Stash", MB_OK);
				// spdlog::info("Stash opened");
			}
			else {
				// Code to close stash
				// Close stash
				diablo2::d2_client::set_ui_toggle(0x19, 1, FALSE);
				// send to server7 to close cube packet 0x4F
				diablo2::d2_client::send_to_server_7(0x4F, 0x17, 0, 0);
				block = true;
			}
		}

	}




	//if (wParam == 'V') {
	//	const auto player = diablo2::d2_client::get_local_player();
	//	int32_t st0Guid = 0;
	//	uint32_t st0X = 0;
	//	uint32_t st0Y = 0;
	//	diablo2::structures::unit* box{};
	//	for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
	//		const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
	//		char* st0Code = record->string_code;
	//		if (strncmp(st0Code, "st0", 3) == 0) {
	//			box = item;
	//			st0Guid = box->guid;
	//			st0X = player->path->mapx;
	//			st0Y = player->path->mapy;
	//		}
	//	}
	//	struct D2GSPacketClt20 {
	//		uint8_t PacketId; // 0x01
	//		uint32_t guid;	// 0x06
	//		uint32_t tx;	// 0x07
	//		uint32_t ty;	// 0x09
	//	};
	//	D2GSPacketClt20 packet;
	//	packet.PacketId = 0x20;
	//	packet.guid = st0Guid;
	//	packet.tx = st0X;
	//	packet.ty = st0Y;
	//	diablo2::d2_client::send_to_server(&packet, sizeof packet);
	//	m_stash_enabled = false;
	//	block = true;
	//}

	// Send item move packet + transmute packet for certain codes only for runes and gems
	if (wParam == 'G') {
		// Call the item_click function using the function pointer
		const auto g_hoverItem = (*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4));

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
	}

	/*
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
	}
	*/

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
		const auto player = diablo2::d2_client::get_local_player();
		auto pInventory = player->inventory;

		int32_t gemBagGuid = 0;
		int32_t harvesterGuid = 0;
		int32_t boxGuid;
		uint32_t boxX;
		uint32_t boxY;

		const auto g_hoverItem = *reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4);

		if (g_hoverItem != nullptr) {
			const auto record = diablo2::d2_common::get_item_record(g_hoverItem->data_record_index);
			char* normCode = record->string_code;
			// Get item record type
			const auto itemtype_record = diablo2::d2_common::get_item_type_record(record->type);
			auto itemtype_record_equiv1 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv1);
			auto itemtype_record_equiv2 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv2);

			/*
			if (strncmp(normCode, "ib1", 3) == 0) {
				// display Messagebox with normCode
				//MessageBoxA(0, normCode, "normCode", 0);

				// we need to accept user input here
				// we need to get the user input and store it in a variable
				// we need to use win32 api to get the user input using editbox control, simple inputbox, not using resources.rc

				  // Get the player name
				const auto player = diablo2::d2_client::get_local_player();
				std::string playerName = player->player_data->name;

				// Construct the file name
				std::string fileName = "./Save/" + playerName + ".boh";

				// Read the first line from the file and extract hex bytes after ':'
				std::string hexBytes = readFirstLineAndExtractHex(fileName);
				if (hexBytes.empty()) {
					// Error handling
					return 1;
				}

				// Output the extracted hex bytes using MessageBox
				MessageBoxA(0, hexBytes.c_str(), "Extracted hex bytes", 0);

				diablo2::structures::unit item = unserialize_item(hexBytes);

				// Display the item guid using MessageBox
				MessageBoxA(0, std::to_string(item.guid).c_str(), "Item GUID", 0);

				// Delete the first line from the file
				deleteFirstLine(fileName);

				const auto record1 = diablo2::d2_common::get_item_record(item.data_record_index);
				char* normCode = record1->string_code;

				MessageBoxA(0, normCode, "normCode", 0);
			}
			*/

			std::vector<diablo2::structures::unit*> items;
			diablo2::structures::unit* gemBag{};
			diablo2::structures::unit* box{};
			diablo2::structures::unit* harvester{};

			// get the gembag item
			for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
				const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
				char* normCode1 = record->string_code;
				if (record->type == 101) {
					gemBag = item;
					gemBagGuid = gemBag->guid;
				}
				if (strncmp(normCode1, "box", 3) == 0) {
					box = item;
					boxGuid = box->guid;
					boxX = player->path->mapx;
					boxY = player->path->mapy;
				}
				if (strncmp(normCode1, "ib3", 3) == 0) {
					harvester = item;
					harvesterGuid = harvester->guid;
				}
			}

			// Actual ID to use is 378 for Ruby, but actual row number is 381
			// 378 is ruby
			// 379 is amathyst
			// 380 is diamond
			// 381 is emerald
			// 382 is saphhire
			// 383 is topaz
			// 384 is skull
			// So remember, it's properties.txt row number - 3

			// Define the structure D2PropertyStrc
			struct GemType {
				int chippedCount;
				int rowID;
			};

			static std::unordered_map<std::string, GemType> gemTypes = {
				{"gcv", {1, 382}},   // Chipped Amethyst
				{"gcw", {1, 383}},   // Chipped Diamond
				{"gcg", {1, 384}},   // Chipped Emerald
				{"gcr", {1, 381}},   // Chipped Ruby
				{"gcb", {1, 385}},   // Chipped Sapphire
				{"skc", {1, 387}},   // Chipped Skull
				{"gcy", {1, 386}},   // Chipped Topaz
				{"gfv", {3, 382}},   // Flawed Amethyst
				{"gfw", {3, 383}},   // Flawed Diamond
				{"gfg", {3, 384}},   // Flawed Emerald
				{"gfr", {3, 381}},   // Flawed Ruby
				{"gfb", {3, 385}},   // Flawed Sapphire
				{"skf", {3, 387}},   // Flawed Skull
				{"gfy", {3, 386}},   // Flawed Topaz
				{"gsv", {9, 382}},   // Amethyst
				{"gsw", {9, 383}},   // Diamond
				{"gsg", {9, 384}},   // Emerald
				{"gsr", {9, 381}},   // Ruby
				{"gsb", {9, 385}},   // Sapphire
				{"sku", {9, 387}},   // Skull
				{"gsy", {9, 386}},   // Topaz
				{"gzv", {27, 382}},  // Flawless Amethyst
				{"glw", {27, 383}},  // Flawless Diamond
				{"glg", {27, 384}},  // Flawless Emerald
				{"glr", {27, 381}},  // Flawless Ruby
				{"glb", {27, 385}},  // Flawless Sapphire
				{"skl", {27, 387}},  // Flawless Skull
				{"gly", {27, 386}},  // Flawless Topaz
				{"gpv", {81, 382}},  // Perfect Amethyst
				{"gpw", {81, 383}},  // Perfect Diamond
				{"gpg", {81, 384}},  // Perfect Emerald
				{"gpr", {81, 381}},  // Perfect Ruby
				{"gpb", {81, 385}},  // Perfect Sapphire
				{"skz", {81, 387}},  // Perfect Skull
				{"gpy", {81, 386}},  // Perfect Topaz
				{"r01", {1, 388}},   // El Rune
				{"r02", {3, 388}},   // Eld Rune
				{"r03", {9, 388}},   // Tir Rune
				{"r04", {27, 388}},  // Nef Rune
				{"r05", {81, 388}},  // Eth Rune
				{"r06", {243, 388}}, // Ith Rune
				{"r07", {1, 389}},   // Tal Rune
				{"r08", {3, 389}},   // Ral Rune
				{"r09", {9, 389}},   // Ort Rune
				{"r10", {27, 389}},  // Thul Rune
				{"r11", {81, 389}},  // Amn Rune
				{"r12", {243, 389}}, // Sol Rune
				{"r13", {1, 390}},   // Shael Rune
				{"r14", {3, 390}},   // Dol Rune
				{"r15", {9, 390}},   // Hel Rune
				{"r16", {27, 390}},  // Io Rune
				{"r17", {81, 390}},  // Lum Rune
				{"r18", {243, 390}}, // Ko Rune
				{"r19", {1, 391}},   // Fal Rune
				{"r20", {3, 391}},   // Lem Rune
				{"r21", {9, 391}},   // Pul Rune
				{"r22", {27, 391}},  // Um Rune
				{"r23", {81, 3901}}, // Mal Rune
				{"r24", {243, 391}}, // Ist Rune
				{"r25", {1, 392}},   // Gul Rune
				{"r26", {3, 392}},   // Vex Rune
				{"r27", {9, 392}},   // Ohm Rune
				{"r28", {27, 392}},  // Lo Rune
				{"r29", {81, 392}},  // Sur Rune
				{"r30", {243, 392}}, // Ber Rune
				{"r31", {1, 393}},   // Jah Rune
				{"r32", {2, 393}},   // Cham Rune
				{"r33", {4, 393}}    // Zod Rune
			};

			auto currentPage = diablo2::d2_common::get_item_page(g_hoverItem);

			const char* key;

			if (currentPage == 0 || currentPage == 4) {
				if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH) || diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE) || diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY)) {
					for (const auto& gem : gemTypes) {
						// Accessing key and value
						const std::string& _key = gem.first;
						key = gem.first.c_str();

						const GemType& value = gem.second;
						if (strncmp(normCode, key, 3) == 0) {
							D2PropertyStrc itemProperty = {};
							itemProperty.nProperty = value.rowID - 3;
							itemProperty.nLayer = 0;
							itemProperty.nMin = value.chippedCount;
							itemProperty.nMax = value.chippedCount;
							diablo2::d2_common::add_property(gemBag, &itemProperty, 0);
							diablo2::d2_client::play_sound(record->drop_sound, nullptr, 0, 0, 0);

							//MessageBoxA(0, key, "key", 0);

							static d2_tweaks::common::item_move_cs packet;
							packet.item_guid = g_hoverItem->guid;
							packet.item_code = key;
							packet.bag_guid = gemBagGuid;
							packet.updateBag = 1;
							packet.prop = itemProperty.nProperty;
							packet.val = itemProperty.nMin;
							packet.target_page = 99;
							diablo2::d2_client::send_to_server(&packet, sizeof packet);
						}
					}
				}
			}

			static std::unordered_map<std::string, GemType> exTypes = {
				{"g25", {-1, 381}},   // Chipped Amethyst
				{"g24", {-1, 383}},   // Chipped Diamond
				{"ge8", {-1, 384}},   // Chipped Emerald
				{"g22", {-1, 381}},   // Chipped Ruby
				{"g21", {-1, 382}},   // Chipped Sapphire
				{"g15", {-1, 384}},   // Chipped Skull
				{"g14", {-1, 383}},   // Chipped Topaz
				{"gb4", {-3, 378}},   // Flawed Amethyst
				{"g12", {-3, 383}},   // Flawed Diamond
				{"g11", {-3, 384}},   // Flawed Emerald
				{"g30", {-3, 381}},   // Flawed Ruby
				{"g29", {-3, 382}},   // Flawed Sapphire
				{"g28", {-3, 384}},   // Flawed Skull
				{"g27", {-3, 383}},   // Flawed Topaz
				{"g10", {-9, 378}},   // Amethyst
				{"ge9", {-9, 383}},   // Diamond
				{"g23", {-9, 384}},   // Emerald
				{"ge7", {-9, 381}},   // Ruby
				{"ge6", {-9, 382}},   // Sapphire
				{"g35", {-9, 384}},   // Skull
				{"gb5", {-9, 383}},   // Topaz
				{"g18", {-27, 378}},  // Flawless Amethyst
				{"g32", {-27, 383}},  // Flawless Diamond
				{"g31", {-27, 384}},  // Flawless Emerald
				{"ge5", {-27, 381}},  // Flawless Ruby
				{"ge4", {-27, 382}},  // Flawless Sapphire
				{"g13", {-27, 384}},  // Flawless Skull
				{"ge2", {-27, 383}},  // Flawless Topaz
				{"ge1", {-81, 378}},  // Perfect Amethyst
				{"g20", {-81, 383}},  // Perfect Diamond
				{"g19", {-81, 384}},  // Perfect Emerald
				{"ge3", {-81, 381}},  // Perfect Ruby
				{"g17", {-81, 382}},  // Perfect Sapphire
				{"g16", {-81, 384}},  // Perfect Skull

				{"x01", {-1, 388}},   // El Rune
				{"x02", {-3, 388}},   // Eld Rune
				{"x03", {-9, 388}},   // Tir Rune
				{"x04", {-27, 388}},  // Nef Rune
				{"x05", {-81, 388}},  // Eth Rune
				{"x06", {-243, 388}}, // Ith Rune
				{"x07", {-1, 389}},   // Tal Rune
				{"x08", {-3, 389}},   // Ral Rune
				{"x09", {-9, 389}},   // Ort Rune
				{"x10", {-27, 389}},  // Thul Rune
				{"x11", {-81, 389}},  // Amn Rune
				{"x12", {-243, 389}}, // Sol Rune
				{"x13", {-1, 390}},   // Shael Rune
				{"x14", {-3, 390}},   // Dol Rune
				{"x15", {-9, 390}},   // Hel Rune
				{"x16", {-27, 390}},  // Io Rune
				{"x17", {-81, 390}},  // Lum Rune
				{"x18", {-243, 390}}, // Ko Rune
				{"x19", {-1, 391}},   // Fal Rune
				{"x20", {-3, 391}},   // Lem Rune
				{"x21", {-9, 391}},   // Pul Rune
				{"x22", {-27, 391}},  // Um Rune
				{"x23", {-81, 3901}}, // Mal Rune
				{"x24", {-243, 391}}, // Ist Rune
				{"x25", {-1, 392}},   // Gul Rune
				{"x26", {-3, 392}},   // Vex Rune
				{"x27", {-9, 392}},   // Ohm Rune
				{"x28", {-27, 392}},  // Lo Rune
				{"x29", {-81, 392}},  // Sur Rune
				{"x30", {-243, 392}}, // Ber Rune
				{"x31", {-1, 393}},   // Jah Rune
				{"x32", {-2, 393}},   // Cham Rune
				{"x33", {-4, 393}}    // Zod Rune
			};

			/*
			if (currentPage == 0 || currentPage == 3 || currentPage == 4) {
				if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH) || diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE) || diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY)) {
					for (const auto& gem : exTypes) {
						// Accessing key and value
						const std::string& _key = gem.first;
						key = gem.first.c_str();

						const GemType& value = gem.second;
						if (strncmp(normCode, key, 3) == 0) {
							D2PropertyStrc itemProperty = {};
							itemProperty.nProperty = value.rowID - 3;
							itemProperty.nLayer = 0;
							itemProperty.nMin = value.chippedCount;
							itemProperty.nMax = value.chippedCount;
							diablo2::d2_common::add_property(gemBag, &itemProperty, 0);
							diablo2::d2_client::play_sound(record->drop_sound, nullptr, 0, 0, 0);

							//MessageBoxA(0, key, "key", 0);

							static d2_tweaks::common::item_move_cs packet;
							packet.item_guid = g_hoverItem->guid;
							packet.item_code = key;
							packet.bag_guid = gemBagGuid;
							packet.updateBag = 1;
							packet.removeFromBag = 1;
							packet.prop = itemProperty.nProperty;
							packet.val = itemProperty.nMin;
							packet.target_page = 99;
							diablo2::d2_client::send_to_server(&packet, sizeof packet);
						}
					}
				}
			}
			*/

			// If Cube is open, then if we right click certain item codes, they should be moved to the cube, send transmute packet, and then move the item back to the inventory

			if (currentPage == 0 || currentPage == 3 || currentPage == 4) {
				if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH) || diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE) || diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY)) {
					for (const auto& gem : exTypes) {
						// Accessing key and value
						const std::string& _key = gem.first;
						key = gem.first.c_str();

						const GemType& value = gem.second;
						if (strncmp(normCode, key, 3) == 0) {
							char currentPage = diablo2::d2_common::get_item_page(g_hoverItem);

							// Create the packet to send hover item to cube
							static d2_tweaks::common::item_move_cs packet;
							packet.item_guid = g_hoverItem->guid;
							packet.target_page = 3;
							diablo2::d2_client::send_to_server(&packet, sizeof packet);

							static d2_tweaks::common::item_move_cs packetBag;
							packetBag.item_guid = gemBag->guid;
							packetBag.target_page = 3;
							diablo2::d2_client::send_to_server(&packetBag, sizeof packetBag);

							// send transmute button packet
							diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);

							(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;

							// Create the packet to send bag back to inventory
							//static d2_tweaks::common::item_move_cs packetItemBack;
							//packetItemBack.item_guid = gemBag->guid;
							//packetItemBack.target_page = 0;
							//diablo2::d2_client::send_to_server(&packetItemBack, sizeof packetItemBack);

							//static d2_tweaks::common::item_move_cs packetBagBack;
							//packetBagBack.item_guid = gemBag->guid;
							//packetBagBack.target_page = 0;
							//diablo2::d2_client::send_to_server(&packetBagBack, sizeof packetBagBack);

							/*

							// Create the packet to send bag to cube
							packet.item_guid = gemBag->guid;
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

							// send transmute button packet
							diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);

							// after transmute, move item and bag back to inventory
							packet.item_guid = gemBag->guid;
							packet.target_page = 0;
							diablo2::d2_client::send_to_server(&packet, sizeof packet);

							packet.item_guid = g_hoverItem->guid;
							packet.target_page = 0;
							diablo2::d2_client::send_to_server(&packet, sizeof packet);
							*/
						}
					}
				}
			}

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
				strncmp(normCode, "r33", 3) == 0
				) {
				// Move items from cube to inventory
				const auto player = diablo2::d2_client::get_local_player();
				for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
					currentPage = diablo2::d2_common::get_item_page(item);

					// display current page in a messagebox
					// MessageBoxA(0, std::to_string(currentPage).c_str(), "Current Page", 0);

					if (currentPage == 3) { // Item is in the cube
						// get item record
						const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
						char* normCode = record->string_code;

						// if normCode strncmp is equal to "ib1", don't send the item back to inventory
						if (strncmp(normCode, "ib1", 3) == 0) {
							continue;
						}
						else {
							// display item guid in a messagebox
							//MessageBoxA(0, std::to_string(item->guid).c_str(), "Item GUID", 0);

							static d2_tweaks::common::item_move_cs movePacket;
							movePacket.item_guid = item->guid;
							movePacket.target_page = 0; // Move to inventory
							diablo2::d2_client::send_to_server(&movePacket, sizeof movePacket);
						}
					}
				}
			}

			if (strncmp(normCode, "ib1", 3) == 0 ||
				strncmp(normCode, "ib3", 3) == 0 ||
				strncmp(normCode, "mez", 3) == 0 ||
				strncmp(normCode, "me0", 3) == 0 ||
				strncmp(normCode, "cap", 3) == 0 ||
				strncmp(normCode, "skp", 3) == 0 ||
				strncmp(normCode, "hlm", 3) == 0 ||
				strncmp(normCode, "fhl", 3) == 0 ||
				strncmp(normCode, "ghm", 3) == 0 ||
				strncmp(normCode, "crn", 3) == 0 ||
				strncmp(normCode, "msk", 3) == 0 ||
				strncmp(normCode, "qui", 3) == 0 ||
				strncmp(normCode, "lea", 3) == 0 ||
				strncmp(normCode, "hla", 3) == 0 ||
				strncmp(normCode, "stu", 3) == 0 ||
				strncmp(normCode, "rng", 3) == 0 ||
				strncmp(normCode, "scl", 3) == 0 ||
				strncmp(normCode, "chn", 3) == 0 ||
				strncmp(normCode, "brs", 3) == 0 ||
				strncmp(normCode, "spl", 3) == 0 ||
				strncmp(normCode, "plt", 3) == 0 ||
				strncmp(normCode, "fld", 3) == 0 ||
				strncmp(normCode, "gth", 3) == 0 ||
				strncmp(normCode, "ful", 3) == 0 ||
				strncmp(normCode, "aar", 3) == 0 ||
				strncmp(normCode, "ltp", 3) == 0 ||
				strncmp(normCode, "buc", 3) == 0 ||
				strncmp(normCode, "sml", 3) == 0 ||
				strncmp(normCode, "lrg", 3) == 0 ||
				strncmp(normCode, "kit", 3) == 0 ||
				strncmp(normCode, "tow", 3) == 0 ||
				strncmp(normCode, "gts", 3) == 0 ||
				strncmp(normCode, "lgl", 3) == 0 ||
				strncmp(normCode, "vgl", 3) == 0 ||
				strncmp(normCode, "mgl", 3) == 0 ||
				strncmp(normCode, "tgl", 3) == 0 ||
				strncmp(normCode, "hgl", 3) == 0 ||
				strncmp(normCode, "lbt", 3) == 0 ||
				strncmp(normCode, "vbt", 3) == 0 ||
				strncmp(normCode, "mbt", 3) == 0 ||
				strncmp(normCode, "tbt", 3) == 0 ||
				strncmp(normCode, "hbt", 3) == 0 ||
				strncmp(normCode, "lbl", 3) == 0 ||
				strncmp(normCode, "vbl", 3) == 0 ||
				strncmp(normCode, "mbl", 3) == 0 ||
				strncmp(normCode, "tbl", 3) == 0 ||
				strncmp(normCode, "hbl", 3) == 0 ||
				strncmp(normCode, "bhm", 3) == 0 ||
				strncmp(normCode, "bsh", 3) == 0 ||
				strncmp(normCode, "spk", 3) == 0 ||
				strncmp(normCode, "xap", 3) == 0 ||
				strncmp(normCode, "xkp", 3) == 0 ||
				strncmp(normCode, "xlm", 3) == 0 ||
				strncmp(normCode, "xhl", 3) == 0 ||
				strncmp(normCode, "xhm", 3) == 0 ||
				strncmp(normCode, "xrn", 3) == 0 ||
				strncmp(normCode, "xsk", 3) == 0 ||
				strncmp(normCode, "xui", 3) == 0 ||
				strncmp(normCode, "xea", 3) == 0 ||
				strncmp(normCode, "xla", 3) == 0 ||
				strncmp(normCode, "xtu", 3) == 0 ||
				strncmp(normCode, "xng", 3) == 0 ||
				strncmp(normCode, "xcl", 3) == 0 ||
				strncmp(normCode, "xhn", 3) == 0 ||
				strncmp(normCode, "xrs", 3) == 0 ||
				strncmp(normCode, "xpl", 3) == 0 ||
				strncmp(normCode, "xlt", 3) == 0 ||
				strncmp(normCode, "xld", 3) == 0 ||
				strncmp(normCode, "xth", 3) == 0 ||
				strncmp(normCode, "xul", 3) == 0 ||
				strncmp(normCode, "xar", 3) == 0 ||
				strncmp(normCode, "xtp", 3) == 0 ||
				strncmp(normCode, "xuc", 3) == 0 ||
				strncmp(normCode, "xml", 3) == 0 ||
				strncmp(normCode, "xrg", 3) == 0 ||
				strncmp(normCode, "xit", 3) == 0 ||
				strncmp(normCode, "xow", 3) == 0 ||
				strncmp(normCode, "xts", 3) == 0 ||
				strncmp(normCode, "xlg", 3) == 0 ||
				strncmp(normCode, "xvg", 3) == 0 ||
				strncmp(normCode, "xmg", 3) == 0 ||
				strncmp(normCode, "xtg", 3) == 0 ||
				strncmp(normCode, "xhg", 3) == 0 ||
				strncmp(normCode, "xlb", 3) == 0 ||
				strncmp(normCode, "xvb", 3) == 0 ||
				strncmp(normCode, "xmb", 3) == 0 ||
				strncmp(normCode, "xtb", 3) == 0 ||
				strncmp(normCode, "xhb", 3) == 0 ||
				strncmp(normCode, "zlb", 3) == 0 ||
				strncmp(normCode, "zvb", 3) == 0 ||
				strncmp(normCode, "zmb", 3) == 0 ||
				strncmp(normCode, "ztb", 3) == 0 ||
				strncmp(normCode, "zhb", 3) == 0 ||
				strncmp(normCode, "xh9", 3) == 0 ||
				strncmp(normCode, "xsh", 3) == 0 ||
				strncmp(normCode, "xpk", 3) == 0 ||
				strncmp(normCode, "dr1", 3) == 0 ||
				strncmp(normCode, "dr2", 3) == 0 ||
				strncmp(normCode, "dr3", 3) == 0 ||
				strncmp(normCode, "dr4", 3) == 0 ||
				strncmp(normCode, "dr5", 3) == 0 ||
				strncmp(normCode, "ba1", 3) == 0 ||
				strncmp(normCode, "ba2", 3) == 0 ||
				strncmp(normCode, "ba3", 3) == 0 ||
				strncmp(normCode, "ba4", 3) == 0 ||
				strncmp(normCode, "ba5", 3) == 0 ||
				strncmp(normCode, "pa1", 3) == 0 ||
				strncmp(normCode, "pa2", 3) == 0 ||
				strncmp(normCode, "pa3", 3) == 0 ||
				strncmp(normCode, "pa4", 3) == 0 ||
				strncmp(normCode, "pa5", 3) == 0 ||
				strncmp(normCode, "ne1", 3) == 0 ||
				strncmp(normCode, "ne2", 3) == 0 ||
				strncmp(normCode, "ne3", 3) == 0 ||
				strncmp(normCode, "ne4", 3) == 0 ||
				strncmp(normCode, "ne5", 3) == 0 ||
				strncmp(normCode, "ci0", 3) == 0 ||
				strncmp(normCode, "ci1", 3) == 0 ||
				strncmp(normCode, "ci2", 3) == 0 ||
				strncmp(normCode, "ci3", 3) == 0 ||
				strncmp(normCode, "uap", 3) == 0 ||
				strncmp(normCode, "ukp", 3) == 0 ||
				strncmp(normCode, "ulm", 3) == 0 ||
				strncmp(normCode, "uhl", 3) == 0 ||
				strncmp(normCode, "uhm", 3) == 0 ||
				strncmp(normCode, "urn", 3) == 0 ||
				strncmp(normCode, "usk", 3) == 0 ||
				strncmp(normCode, "uui", 3) == 0 ||
				strncmp(normCode, "uea", 3) == 0 ||
				strncmp(normCode, "ula", 3) == 0 ||
				strncmp(normCode, "utu", 3) == 0 ||
				strncmp(normCode, "ung", 3) == 0 ||
				strncmp(normCode, "ucl", 3) == 0 ||
				strncmp(normCode, "uhn", 3) == 0 ||
				strncmp(normCode, "urs", 3) == 0 ||
				strncmp(normCode, "upl", 3) == 0 ||
				strncmp(normCode, "ult", 3) == 0 ||
				strncmp(normCode, "uld", 3) == 0 ||
				strncmp(normCode, "uth", 3) == 0 ||
				strncmp(normCode, "uul", 3) == 0 ||
				strncmp(normCode, "uar", 3) == 0 ||
				strncmp(normCode, "utp", 3) == 0 ||
				strncmp(normCode, "uuc", 3) == 0 ||
				strncmp(normCode, "uml", 3) == 0 ||
				strncmp(normCode, "urg", 3) == 0 ||
				strncmp(normCode, "uit", 3) == 0 ||
				strncmp(normCode, "uow", 3) == 0 ||
				strncmp(normCode, "uts", 3) == 0 ||
				strncmp(normCode, "ulg", 3) == 0 ||
				strncmp(normCode, "uvg", 3) == 0 ||
				strncmp(normCode, "umg", 3) == 0 ||
				strncmp(normCode, "utg", 3) == 0 ||
				strncmp(normCode, "uhg", 3) == 0 ||
				strncmp(normCode, "ulb", 3) == 0 ||
				strncmp(normCode, "uvb", 3) == 0 ||
				strncmp(normCode, "umb", 3) == 0 ||
				strncmp(normCode, "utb", 3) == 0 ||
				strncmp(normCode, "uhb", 3) == 0 ||
				strncmp(normCode, "ulc", 3) == 0 ||
				strncmp(normCode, "uvc", 3) == 0 ||
				strncmp(normCode, "umc", 3) == 0 ||
				strncmp(normCode, "utc", 3) == 0 ||
				strncmp(normCode, "uhc", 3) == 0 ||
				strncmp(normCode, "uh9", 3) == 0 ||
				strncmp(normCode, "ush", 3) == 0 ||
				strncmp(normCode, "upk", 3) == 0 ||
				strncmp(normCode, "dr6", 3) == 0 ||
				strncmp(normCode, "dr7", 3) == 0 ||
				strncmp(normCode, "dr8", 3) == 0 ||
				strncmp(normCode, "dr9", 3) == 0 ||
				strncmp(normCode, "dra", 3) == 0 ||
				strncmp(normCode, "ba6", 3) == 0 ||
				strncmp(normCode, "ba7", 3) == 0 ||
				strncmp(normCode, "ba8", 3) == 0 ||
				strncmp(normCode, "ba9", 3) == 0 ||
				strncmp(normCode, "baa", 3) == 0 ||
				strncmp(normCode, "pa6", 3) == 0 ||
				strncmp(normCode, "pa7", 3) == 0 ||
				strncmp(normCode, "pa8", 3) == 0 ||
				strncmp(normCode, "pa9", 3) == 0 ||
				strncmp(normCode, "paa", 3) == 0 ||
				strncmp(normCode, "ne6", 3) == 0 ||
				strncmp(normCode, "ne7", 3) == 0 ||
				strncmp(normCode, "ne8", 3) == 0 ||
				strncmp(normCode, "ne9", 3) == 0 ||
				strncmp(normCode, "nea", 3) == 0 ||
				strncmp(normCode, "drb", 3) == 0 ||
				strncmp(normCode, "drc", 3) == 0 ||
				strncmp(normCode, "drd", 3) == 0 ||
				strncmp(normCode, "dre", 3) == 0 ||
				strncmp(normCode, "drf", 3) == 0 ||
				strncmp(normCode, "bab", 3) == 0 ||
				strncmp(normCode, "bac", 3) == 0 ||
				strncmp(normCode, "bad", 3) == 0 ||
				strncmp(normCode, "bae", 3) == 0 ||
				strncmp(normCode, "baf", 3) == 0 ||
				strncmp(normCode, "pab", 3) == 0 ||
				strncmp(normCode, "pac", 3) == 0 ||
				strncmp(normCode, "pad", 3) == 0 ||
				strncmp(normCode, "pae", 3) == 0 ||
				strncmp(normCode, "paf", 3) == 0 ||
				strncmp(normCode, "neb", 3) == 0 ||
				strncmp(normCode, "neg", 3) == 0 ||
				strncmp(normCode, "ned", 3) == 0 ||
				strncmp(normCode, "nee", 3) == 0 ||
				strncmp(normCode, "nef", 3) == 0 ||
				strncmp(normCode, "tor", 3) == 0 ||
				strncmp(normCode, "hax", 3) == 0 ||
				strncmp(normCode, "axe", 3) == 0 ||
				strncmp(normCode, "2ax", 3) == 0 ||
				strncmp(normCode, "mpi", 3) == 0 ||
				strncmp(normCode, "wax", 3) == 0 ||
				strncmp(normCode, "lax", 3) == 0 ||
				strncmp(normCode, "bax", 3) == 0 ||
				strncmp(normCode, "btx", 3) == 0 ||
				strncmp(normCode, "gax", 3) == 0 ||
				strncmp(normCode, "gix", 3) == 0 ||
				strncmp(normCode, "wnd", 3) == 0 ||
				strncmp(normCode, "ywn", 3) == 0 ||
				strncmp(normCode, "bwn", 3) == 0 ||
				strncmp(normCode, "gwn", 3) == 0 ||
				strncmp(normCode, "clb", 3) == 0 ||
				strncmp(normCode, "scp", 3) == 0 ||
				strncmp(normCode, "gsc", 3) == 0 ||
				strncmp(normCode, "wsp", 3) == 0 ||
				strncmp(normCode, "spc", 3) == 0 ||
				strncmp(normCode, "mac", 3) == 0 ||
				strncmp(normCode, "mst", 3) == 0 ||
				strncmp(normCode, "fla", 3) == 0 ||
				strncmp(normCode, "whm", 3) == 0 ||
				strncmp(normCode, "mau", 3) == 0 ||
				strncmp(normCode, "gma", 3) == 0 ||
				strncmp(normCode, "ssd", 3) == 0 ||
				strncmp(normCode, "scm", 3) == 0 ||
				strncmp(normCode, "sbr", 3) == 0 ||
				strncmp(normCode, "flc", 3) == 0 ||
				strncmp(normCode, "crs", 3) == 0 ||
				strncmp(normCode, "bsd", 3) == 0 ||
				strncmp(normCode, "lsd", 3) == 0 ||
				strncmp(normCode, "wsd", 3) == 0 ||
				strncmp(normCode, "2hs", 3) == 0 ||
				strncmp(normCode, "clm", 3) == 0 ||
				strncmp(normCode, "gis", 3) == 0 ||
				strncmp(normCode, "bsw", 3) == 0 ||
				strncmp(normCode, "flb", 3) == 0 ||
				strncmp(normCode, "gsd", 3) == 0 ||
				strncmp(normCode, "dgr", 3) == 0 ||
				strncmp(normCode, "dir", 3) == 0 ||
				strncmp(normCode, "kri", 3) == 0 ||
				strncmp(normCode, "bld", 3) == 0 ||
				strncmp(normCode, "tkf", 3) == 0 ||
				strncmp(normCode, "tax", 3) == 0 ||
				strncmp(normCode, "bkf", 3) == 0 ||
				strncmp(normCode, "bal", 3) == 0 ||
				strncmp(normCode, "jav", 3) == 0 ||
				strncmp(normCode, "pil", 3) == 0 ||
				strncmp(normCode, "ssp", 3) == 0 ||
				strncmp(normCode, "glv", 3) == 0 ||
				strncmp(normCode, "tsp", 3) == 0 ||
				strncmp(normCode, "spr", 3) == 0 ||
				strncmp(normCode, "tri", 3) == 0 ||
				strncmp(normCode, "brn", 3) == 0 ||
				strncmp(normCode, "spt", 3) == 0 ||
				strncmp(normCode, "pik", 3) == 0 ||
				strncmp(normCode, "bar", 3) == 0 ||
				strncmp(normCode, "vou", 3) == 0 ||
				strncmp(normCode, "scy", 3) == 0 ||
				strncmp(normCode, "pax", 3) == 0 ||
				strncmp(normCode, "hal", 3) == 0 ||
				strncmp(normCode, "wsc", 3) == 0 ||
				strncmp(normCode, "sst", 3) == 0 ||
				strncmp(normCode, "lst", 3) == 0 ||
				strncmp(normCode, "cst", 3) == 0 ||
				strncmp(normCode, "bst", 3) == 0 ||
				strncmp(normCode, "wst", 3) == 0 ||
				strncmp(normCode, "sbw", 3) == 0 ||
				strncmp(normCode, "hbw", 3) == 0 ||
				strncmp(normCode, "lbw", 3) == 0 ||
				strncmp(normCode, "cbw", 3) == 0 ||
				strncmp(normCode, "sbb", 3) == 0 ||
				strncmp(normCode, "lbb", 3) == 0 ||
				strncmp(normCode, "swb", 3) == 0 ||
				strncmp(normCode, "lwb", 3) == 0 ||
				strncmp(normCode, "lxb", 3) == 0 ||
				strncmp(normCode, "mxb", 3) == 0 ||
				strncmp(normCode, "hxb", 3) == 0 ||
				strncmp(normCode, "rxb", 3) == 0 ||
				strncmp(normCode, "gps", 3) == 0 ||
				strncmp(normCode, "ops", 3) == 0 ||
				strncmp(normCode, "gpm", 3) == 0 ||
				strncmp(normCode, "opm", 3) == 0 ||
				strncmp(normCode, "gpl", 3) == 0 ||
				strncmp(normCode, "opl", 3) == 0 ||
				strncmp(normCode, "d33", 3) == 0 ||
				strncmp(normCode, "g33", 3) == 0 ||
				strncmp(normCode, "leg", 3) == 0 ||
				strncmp(normCode, "hdm", 3) == 0 ||
				strncmp(normCode, "hfh", 3) == 0 ||
				strncmp(normCode, "hst", 3) == 0 ||
				strncmp(normCode, "msf", 3) == 0 ||
				strncmp(normCode, "9ha", 3) == 0 ||
				strncmp(normCode, "9ax", 3) == 0 ||
				strncmp(normCode, "92a", 3) == 0 ||
				strncmp(normCode, "9mp", 3) == 0 ||
				strncmp(normCode, "9wa", 3) == 0 ||
				strncmp(normCode, "9la", 3) == 0 ||
				strncmp(normCode, "9ba", 3) == 0 ||
				strncmp(normCode, "9bt", 3) == 0 ||
				strncmp(normCode, "9ga", 3) == 0 ||
				strncmp(normCode, "9gi", 3) == 0 ||
				strncmp(normCode, "9wn", 3) == 0 ||
				strncmp(normCode, "9yw", 3) == 0 ||
				strncmp(normCode, "9bw", 3) == 0 ||
				strncmp(normCode, "9gw", 3) == 0 ||
				strncmp(normCode, "9cl", 3) == 0 ||
				strncmp(normCode, "9sc", 3) == 0 ||
				strncmp(normCode, "9qs", 3) == 0 ||
				strncmp(normCode, "9ws", 3) == 0 ||
				strncmp(normCode, "9sp", 3) == 0 ||
				strncmp(normCode, "9ma", 3) == 0 ||
				strncmp(normCode, "9mt", 3) == 0 ||
				strncmp(normCode, "9fl", 3) == 0 ||
				strncmp(normCode, "9wh", 3) == 0 ||
				strncmp(normCode, "9m9", 3) == 0 ||
				strncmp(normCode, "9gm", 3) == 0 ||
				strncmp(normCode, "9ss", 3) == 0 ||
				strncmp(normCode, "9sm", 3) == 0 ||
				strncmp(normCode, "9sb", 3) == 0 ||
				strncmp(normCode, "9fc", 3) == 0 ||
				strncmp(normCode, "9cr", 3) == 0 ||
				strncmp(normCode, "9bs", 3) == 0 ||
				strncmp(normCode, "9ls", 3) == 0 ||
				strncmp(normCode, "9wd", 3) == 0 ||
				strncmp(normCode, "92h", 3) == 0 ||
				strncmp(normCode, "9cm", 3) == 0 ||
				strncmp(normCode, "9gs", 3) == 0 ||
				strncmp(normCode, "9b9", 3) == 0 ||
				strncmp(normCode, "9fb", 3) == 0 ||
				strncmp(normCode, "9gd", 3) == 0 ||
				strncmp(normCode, "9dg", 3) == 0 ||
				strncmp(normCode, "9di", 3) == 0 ||
				strncmp(normCode, "9kr", 3) == 0 ||
				strncmp(normCode, "9bl", 3) == 0 ||
				strncmp(normCode, "9tk", 3) == 0 ||
				strncmp(normCode, "9ta", 3) == 0 ||
				strncmp(normCode, "9bk", 3) == 0 ||
				strncmp(normCode, "9b8", 3) == 0 ||
				strncmp(normCode, "9ja", 3) == 0 ||
				strncmp(normCode, "9pi", 3) == 0 ||
				strncmp(normCode, "9s9", 3) == 0 ||
				strncmp(normCode, "9gl", 3) == 0 ||
				strncmp(normCode, "9ts", 3) == 0 ||
				strncmp(normCode, "9sr", 3) == 0 ||
				strncmp(normCode, "9tr", 3) == 0 ||
				strncmp(normCode, "9br", 3) == 0 ||
				strncmp(normCode, "9st", 3) == 0 ||
				strncmp(normCode, "9p9", 3) == 0 ||
				strncmp(normCode, "9b7", 3) == 0 ||
				strncmp(normCode, "9vo", 3) == 0 ||
				strncmp(normCode, "9s8", 3) == 0 ||
				strncmp(normCode, "9pa", 3) == 0 ||
				strncmp(normCode, "9h9", 3) == 0 ||
				strncmp(normCode, "9wc", 3) == 0 ||
				strncmp(normCode, "8ss", 3) == 0 ||
				strncmp(normCode, "8ls", 3) == 0 ||
				strncmp(normCode, "8cs", 3) == 0 ||
				strncmp(normCode, "8bs", 3) == 0 ||
				strncmp(normCode, "8ws", 3) == 0 ||
				strncmp(normCode, "8sb", 3) == 0 ||
				strncmp(normCode, "8hb", 3) == 0 ||
				strncmp(normCode, "8lb", 3) == 0 ||
				strncmp(normCode, "8cb", 3) == 0 ||
				strncmp(normCode, "8s8", 3) == 0 ||
				strncmp(normCode, "8l8", 3) == 0 ||
				strncmp(normCode, "8sw", 3) == 0 ||
				strncmp(normCode, "8lw", 3) == 0 ||
				strncmp(normCode, "8lx", 3) == 0 ||
				strncmp(normCode, "8mx", 3) == 0 ||
				strncmp(normCode, "8hx", 3) == 0 ||
				strncmp(normCode, "8rx", 3) == 0 ||
				strncmp(normCode, "qf1", 3) == 0 ||
				strncmp(normCode, "qf2", 3) == 0 ||
				strncmp(normCode, "ktr", 3) == 0 ||
				strncmp(normCode, "wrb", 3) == 0 ||
				strncmp(normCode, "axf", 3) == 0 ||
				strncmp(normCode, "ces", 3) == 0 ||
				strncmp(normCode, "clw", 3) == 0 ||
				strncmp(normCode, "btl", 3) == 0 ||
				strncmp(normCode, "skr", 3) == 0 ||
				strncmp(normCode, "9ar", 3) == 0 ||
				strncmp(normCode, "9wb", 3) == 0 ||
				strncmp(normCode, "9xf", 3) == 0 ||
				strncmp(normCode, "9cs", 3) == 0 ||
				strncmp(normCode, "9lw", 3) == 0 ||
				strncmp(normCode, "9tw", 3) == 0 ||
				strncmp(normCode, "9qr", 3) == 0 ||
				strncmp(normCode, "7ar", 3) == 0 ||
				strncmp(normCode, "7wb", 3) == 0 ||
				strncmp(normCode, "7xf", 3) == 0 ||
				strncmp(normCode, "7cs", 3) == 0 ||
				strncmp(normCode, "7lw", 3) == 0 ||
				strncmp(normCode, "7tw", 3) == 0 ||
				strncmp(normCode, "7qr", 3) == 0 ||
				strncmp(normCode, "7ha", 3) == 0 ||
				strncmp(normCode, "7ax", 3) == 0 ||
				strncmp(normCode, "72a", 3) == 0 ||
				strncmp(normCode, "7mp", 3) == 0 ||
				strncmp(normCode, "7wa", 3) == 0 ||
				strncmp(normCode, "7la", 3) == 0 ||
				strncmp(normCode, "7ba", 3) == 0 ||
				strncmp(normCode, "7bt", 3) == 0 ||
				strncmp(normCode, "7ga", 3) == 0 ||
				strncmp(normCode, "7gi", 3) == 0 ||
				strncmp(normCode, "7wn", 3) == 0 ||
				strncmp(normCode, "7yw", 3) == 0 ||
				strncmp(normCode, "7bw", 3) == 0 ||
				strncmp(normCode, "7gw", 3) == 0 ||
				strncmp(normCode, "7cl", 3) == 0 ||
				strncmp(normCode, "7sc", 3) == 0 ||
				strncmp(normCode, "7qs", 3) == 0 ||
				strncmp(normCode, "7ws", 3) == 0 ||
				strncmp(normCode, "7sp", 3) == 0 ||
				strncmp(normCode, "7ma", 3) == 0 ||
				strncmp(normCode, "7mt", 3) == 0 ||
				strncmp(normCode, "7fl", 3) == 0 ||
				strncmp(normCode, "7wh", 3) == 0 ||
				strncmp(normCode, "7m7", 3) == 0 ||
				strncmp(normCode, "7gm", 3) == 0 ||
				strncmp(normCode, "7ss", 3) == 0 ||
				strncmp(normCode, "7sm", 3) == 0 ||
				strncmp(normCode, "7sb", 3) == 0 ||
				strncmp(normCode, "7fc", 3) == 0 ||
				strncmp(normCode, "7cr", 3) == 0 ||
				strncmp(normCode, "7bs", 3) == 0 ||
				strncmp(normCode, "7ls", 3) == 0 ||
				strncmp(normCode, "7wd", 3) == 0 ||
				strncmp(normCode, "72h", 3) == 0 ||
				strncmp(normCode, "7cm", 3) == 0 ||
				strncmp(normCode, "7gs", 3) == 0 ||
				strncmp(normCode, "7b7", 3) == 0 ||
				strncmp(normCode, "7fb", 3) == 0 ||
				strncmp(normCode, "7gd", 3) == 0 ||
				strncmp(normCode, "7dg", 3) == 0 ||
				strncmp(normCode, "7di", 3) == 0 ||
				strncmp(normCode, "7kr", 3) == 0 ||
				strncmp(normCode, "7bl", 3) == 0 ||
				strncmp(normCode, "7tk", 3) == 0 ||
				strncmp(normCode, "7ta", 3) == 0 ||
				strncmp(normCode, "7bk", 3) == 0 ||
				strncmp(normCode, "7b8", 3) == 0 ||
				strncmp(normCode, "7ja", 3) == 0 ||
				strncmp(normCode, "7pi", 3) == 0 ||
				strncmp(normCode, "7s7", 3) == 0 ||
				strncmp(normCode, "7gl", 3) == 0 ||
				strncmp(normCode, "7ts", 3) == 0 ||
				strncmp(normCode, "7sr", 3) == 0 ||
				strncmp(normCode, "7tr", 3) == 0 ||
				strncmp(normCode, "7br", 3) == 0 ||
				strncmp(normCode, "7st", 3) == 0 ||
				strncmp(normCode, "7p7", 3) == 0 ||
				strncmp(normCode, "7o7", 3) == 0 ||
				strncmp(normCode, "7vo", 3) == 0 ||
				strncmp(normCode, "7s8", 3) == 0 ||
				strncmp(normCode, "7pa", 3) == 0 ||
				strncmp(normCode, "7h7", 3) == 0 ||
				strncmp(normCode, "7wc", 3) == 0 ||
				strncmp(normCode, "6ss", 3) == 0 ||
				strncmp(normCode, "6ls", 3) == 0 ||
				strncmp(normCode, "6cs", 3) == 0 ||
				strncmp(normCode, "6bs", 3) == 0 ||
				strncmp(normCode, "6ws", 3) == 0 ||
				strncmp(normCode, "6sb", 3) == 0 ||
				strncmp(normCode, "6hb", 3) == 0 ||
				strncmp(normCode, "6lb", 3) == 0 ||
				strncmp(normCode, "6cb", 3) == 0 ||
				strncmp(normCode, "6s7", 3) == 0 ||
				strncmp(normCode, "6l7", 3) == 0 ||
				strncmp(normCode, "6sw", 3) == 0 ||
				strncmp(normCode, "6lw", 3) == 0 ||
				strncmp(normCode, "6lx", 3) == 0 ||
				strncmp(normCode, "6mx", 3) == 0 ||
				strncmp(normCode, "6hx", 3) == 0 ||
				strncmp(normCode, "6rx", 3) == 0 ||
				strncmp(normCode, "ob1", 3) == 0 ||
				strncmp(normCode, "ob2", 3) == 0 ||
				strncmp(normCode, "ob3", 3) == 0 ||
				strncmp(normCode, "ob4", 3) == 0 ||
				strncmp(normCode, "ob5", 3) == 0 ||
				strncmp(normCode, "am1", 3) == 0 ||
				strncmp(normCode, "am2", 3) == 0 ||
				strncmp(normCode, "am3", 3) == 0 ||
				strncmp(normCode, "am4", 3) == 0 ||
				strncmp(normCode, "am5", 3) == 0 ||
				strncmp(normCode, "ob6", 3) == 0 ||
				strncmp(normCode, "ob7", 3) == 0 ||
				strncmp(normCode, "ob8", 3) == 0 ||
				strncmp(normCode, "ob9", 3) == 0 ||
				strncmp(normCode, "oba", 3) == 0 ||
				strncmp(normCode, "am6", 3) == 0 ||
				strncmp(normCode, "am7", 3) == 0 ||
				strncmp(normCode, "am8", 3) == 0 ||
				strncmp(normCode, "am9", 3) == 0 ||
				strncmp(normCode, "ama", 3) == 0 ||
				strncmp(normCode, "obb", 3) == 0 ||
				strncmp(normCode, "obc", 3) == 0 ||
				strncmp(normCode, "obd", 3) == 0 ||
				strncmp(normCode, "obe", 3) == 0 ||
				strncmp(normCode, "obf", 3) == 0 ||
				strncmp(normCode, "amb", 3) == 0 ||
				strncmp(normCode, "amc", 3) == 0 ||
				strncmp(normCode, "amd", 3) == 0 ||
				strncmp(normCode, "ame", 3) == 0 ||
				strncmp(normCode, "amf", 3) == 0 ||
				strncmp(normCode, "ooc", 3) == 0 ||
				strncmp(normCode, "eaq", 3) == 0 ||
				strncmp(normCode, "ebq", 3) == 0 ||

				record->type == 109
				|| record->type == 111
				|| record->type == 112
				|| record->type == 113
				|| record->type == 120
				|| record->type == 113
				|| record->type == 122
				|| record->type == 123
				|| record->type == 125

				|| record->type == 4 - 3
				|| record->type == 5 - 3
				|| record->type == 17 - 3
				|| record->type == 18 - 3
				|| record->type == 21 - 3
				|| record->type == 26 - 3
				|| record->type == 27 - 3
				|| record->type == 28 - 3
				|| record->type == 29 - 3
				|| record->type == 30 - 3
				|| record->type == 31 - 3
				|| record->type == 32 - 3
				|| record->type == 33 - 3
				|| record->type == 34 - 3
				|| record->type == 35 - 3
				|| record->type == 36 - 3
				|| record->type == 37 - 3
				|| record->type == 38 - 3
				|| record->type == 39 - 3
				|| record->type == 40 - 3
				|| record->type == 41 - 3
				|| record->type == 47 - 3
				|| record->type == 48 - 3
				|| record->type == 49 - 3
				|| record->type == 50 - 3

				|| record->type == 51 - 3
				|| record->type == 52 - 3
				|| record->type == 53 - 3

				) {
				// open the cube
				struct D2GSPacketClt20 {
					uint8_t PacketId; // 0x01
					uint32_t guid;	// 0x06
					uint32_t tx;	// 0x07
					uint32_t ty;	// 0x09
				};
				D2GSPacketClt20 D2GSPacketClt20;
				D2GSPacketClt20.PacketId = 0x20;
				D2GSPacketClt20.guid = boxGuid;
				D2GSPacketClt20.tx = player->path->mapx;
				D2GSPacketClt20.ty = player->path->mapy;
				diablo2::d2_client::send_to_server(&D2GSPacketClt20, sizeof D2GSPacketClt20);

				Sleep(100);

				// now move the harvester guid to the cube
				// Create the packet
				static d2_tweaks::common::item_move_cs hpacket;
				hpacket.item_guid = harvesterGuid;
				if (currentPage == 0) { //item is in inventory
					if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH))
						hpacket.target_page = 4;

					if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE))
						hpacket.target_page = 3;
				}
				else {
					hpacket.target_page = 0;
				}
				diablo2::d2_client::send_to_server(&hpacket, sizeof hpacket);

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
				diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);

				// now move the harvester back to the inv
				//static d2_tweaks::common::item_move_cs h1packet;
				//h1packet.item_guid = harvesterGuid;
				//h1packet.target_page = 0;
				//diablo2::d2_client::send_to_server(&h1packet, sizeof h1packet);

				(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
			}
		}

		block = instance.process_right_mouse(false);
		break;
	}

	case WM_RBUTTONUP:
	{
		diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);
		block = instance.process_right_mouse(true);
		break;
	}

	case WM_MOUSEWHEEL:
	{
		short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta > 0) {
			diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);
			block = instance.process_mouse_wheel(true);
		}
		else if (zDelta < 0) {
			diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);
			block = instance.process_mouse_wheel(false);
		}
		break;
	}

	case WM_MBUTTONUP:
	{
		block = instance.process_middle_mouse(true);
		break;
	}

	case WM_MBUTTONDOWN:
	{
		diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);
		block = instance.process_middle_mouse(false);
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
	}
	else if (m_was_down_before_left) {
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
	}
	else if (m_was_down_before_right) {
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

bool d2_tweaks::ui::ui_manager::process_middle_mouse(bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->middle_mouse(up);
	}

	return block;
}

bool d2_tweaks::ui::ui_manager::process_mouse_wheel(bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->mouse_wheel(up);
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

#pragma pack(pop)