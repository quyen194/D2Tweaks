#include <d2tweaks/client/client.h>

#include <common/hooking.h>

#include <spdlog/spdlog.h>

#include <d2tweaks/common/protocol.h>
#include <d2tweaks/client/modules/client_module.h>
#include <d2tweaks/ui/ui_manager.h>


#include <diablo2/structures/unit.h>
#include <diablo2/structures/client_unit_list.h>
#include <WinBase.h>
//debug junk
//#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <Windows.h>

#include <sstream>
#include <vector>
#include <map>
#include <codecvt>
#include <iostream>
#include <regex>
#include <regex>

#include <d2tweaks/common/common.h>

#include <diablo2/d2common.h>
#include <diablo2/d2client.h>
#include <diablo2/d2cmp.h>
#include <diablo2/d2win.h>
#include <diablo2/d2gfx.h>
#include <diablo2/d2game.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/player_data.h>

#define STB_IMAGE_IMPLEMENTATION

std::vector<StatEntry> globalStatsVector;
diablo2::structures::gfxdata g_gfxdata; // global gfxdata

int randStat;
int randStatRangeLow;
int randStatRangeHigh;
int randStatBool;

std::wstring ConvertCharToWString(const std::string& charString) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(charString);
}

// Function to convert std::wstring to std::string
std::string WStringToString(const std::wstring& wstr) {
	// Create a codecvt facet for UTF-8 conversion
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(wstr);
}

// Function to convert std::string to std::wstring
std::wstring StringToWString(const std::string& str) {
	// Create a codecvt facet for UTF-8 conversion
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.from_bytes(str);
}

// Define the mapColorToEnum function
diablo2::ui_color_t mapColorToEnum(const std::string& colorName) {
	static const std::unordered_map<std::string, diablo2::ui_color_t> colorMap = {
		{"RED", diablo2::ui_color_t::UI_COLOR_RED},
		{"LIGHT_GREEN", diablo2::ui_color_t::UI_COLOR_LIGHT_GREEN},
		{"BLUE", diablo2::ui_color_t::UI_COLOR_BLUE},
		{"DARK_GOLD", diablo2::ui_color_t::UI_COLOR_DARK_GOLD},
		{"GREY", diablo2::ui_color_t::UI_COLOR_GREY},
		{"BLACK", diablo2::ui_color_t::UI_COLOR_BLACK},
		{"GOLD", diablo2::ui_color_t::UI_COLOR_GOLD},
		{"ORANGE", diablo2::ui_color_t::UI_COLOR_ORANGE},
		{"YELLOW", diablo2::ui_color_t::UI_COLOR_YELLOW},
		{"DARK_GREEN", diablo2::ui_color_t::UI_COLOR_DARK_GREEN},
		{"PURPLE", diablo2::ui_color_t::UI_COLOR_PURPLE},
		{"GREEN", diablo2::ui_color_t::UI_COLOR_GREEN},
		{"WHITE", diablo2::ui_color_t::UI_COLOR_WHITE},
		{"BLACK2", diablo2::ui_color_t::UI_COLOR_BLACK2},
		{"DARK_WHITE", diablo2::ui_color_t::UI_COLOR_DARK_WHITE},
		{"LIGHT_GREY", diablo2::ui_color_t::UI_COLOR_LIGHT_GREY}
	};

	auto it = colorMap.find(colorName);
	if (it != colorMap.end()) {
		return it->second;
	}
	// Default color if not found
	return diablo2::ui_color_t::UI_COLOR_WHITE;
}



// Define a struct to hold key-value pairs within a section
struct Section {
	std::map<std::string, std::string> assignments;
};

// Function to parse an INI file and extract sections starting with "Stat"
void ParseIniFile(const std::string& iniFilePath) {
	std::ifstream inFile(iniFilePath);
	if (!inFile) {
		std::cerr << "Error opening file: " << iniFilePath << std::endl;
		return;
	}

	std::string line;
	std::string currentSection;
	StatEntry entry;

	while (std::getline(inFile, line)) {
		// Trim leading and trailing whitespace from the line
		line.erase(line.find_last_not_of(" \t\r\n") + 1);

		// Check if the line contains a section header
		if (line.size() > 2 && line.front() == '[' && line.back() == ']') {
			currentSection = line.substr(1, line.size() - 2); // Extract section name
			if (currentSection.find("Stat") == 0) {
				entry = {}; // Reset entry for a new section
			}
		}
		else if (!currentSection.empty() && line.find('=') != std::string::npos) {
			// Parse key-value pairs within the section
			std::istringstream iss(line);
			std::string key, value;
			if (std::getline(iss, key, '=') && std::getline(iss, value)) {
				if (currentSection.find("Stat") == 0) {
					// Process key-value pairs for "Stat" sections
					if (key == "stat") {
						entry.stat = std::stoi(value);
					}
					else if (key == "stat_display_string") {
						entry.stat_display_string = ConvertCharToWString(value); // a conversion function
					}
					else if (key == "colorStat") {
						entry.colorStat = mapColorToEnum(value); // a mapping function
					}
					else if (key == "colorStatValue") {
						entry.colorStatValue = mapColorToEnum(value); // a mapping function
					}
					else if (key == "x1") {
						entry.x1 = std::stoi(value);
					}
					else if (key == "y1") {
						entry.y1 = std::stoi(value);
					}
					else if (key == "x2") {
						entry.x2 = std::stoi(value);
					}
					else if (key == "y2") {
						entry.y2 = std::stoi(value);
					}
					else if (key == "is_item_stat") {
						entry.is_item_stat = std::stoi(value);
					}
					else if (key == "item_type_id") {
						entry.item_type_id = std::stoi(value);
					}
				}
			}
		}
		else if (!currentSection.empty() && currentSection.find("Stat") == 0) {
			// End of section detected, add entry to global vector
			globalStatsVector.push_back(entry);
		}
	}

	inFile.close();
}

#include <D2Template.h>

static void(__fastcall* g_handle_packet)(d2_tweaks::common::packet_header* packet, size_t size);
static void(__fastcall* g_handle_packet_standart)(d2_tweaks::common::packet_header* packet, size_t size);
static void(__fastcall* g_handle_cs_packet)(d2_tweaks::common::packet_header* packet, size_t size);
static int32_t(__stdcall* g_draw_game_ui_original)();
static int32_t(__fastcall* g_game_tick_original)(int32_t a1);
//void(__fastcall* g_game_loop_start)();

//static uint32_t g_ret;

d2_tweaks::client::client::client(token) {
	m_module_id_counter = 0;
	m_tick_handler_id_counter = 0;
}

__declspec(naked) void __stdcall game_tick_wrapper()
{
	// usercall, использует ecx = param1, esi = param2
	__asm
	{
		pushad
		pushfd
		call[d2_tweaks::client::client::game_tick]
		popfd
		popad

		push[g_game_tick_original]
		ret
	}
}

__declspec (naked) void handle_cs_packet_wrapper() {
	__asm {
		pushad;
		pushfd;
		call [d2_tweaks::client::client::handle_cs_packet]
		popfd;
		popad;
		// оригинальные инструкции
		sub esp, 0x200;
	}
	RET_TO_RVA(DLLBASE_D2CLIENT, 0xD856);
}


__declspec (naked) void handle_sc_standart_packet_wrapper() {
	__asm {
		pushad;
		pushfd;
		call[d2_tweaks::client::client::handle_standart_packet]
		popfd;
		popad;
		// оригинальные инструкции
		sub esp, 0x54;
		push ebx;
		push ebp;
	}
	RET_TO_RVA(DLLBASE_D2CLIENT, 0x150B5);
}


static const DLLPatchStrc gpt_handle_cs_packet[] =
{
	{D2DLL_D2CLIENT, 0xD850 + 0, PATCH_JMP, FALSE, 0x1},
	{D2DLL_D2CLIENT, 0xD850 + 1, (DWORD)handle_cs_packet_wrapper, TRUE, 0x0},
	{D2DLL_D2CLIENT, 0xD850 + 5, (DWORD)PATCH_NOPBLOCK, FALSE, 0x1},
	{D2DLL_INVALID}
};


static const DLLPatchStrc gpt_handle_sc_standart_packet[] =
{
	{D2DLL_D2CLIENT, 0x150B0 + 0, PATCH_JMP, FALSE, 0x1},
	{D2DLL_D2CLIENT, 0x150B0 + 1, (DWORD)handle_sc_standart_packet_wrapper, TRUE, 0x0},
	{D2DLL_INVALID}
};


void d2_tweaks::client::client::init() {
	// handle packet обрабатывает пакет перед GamePacketReceivedIntercept
	hooking::hook(diablo2::d2_client::get_base() + 0x11CB0, handle_packet, reinterpret_cast<void**>(&g_handle_packet));
	hooking::hook(diablo2::d2_client::get_base() + 0x9640, game_tick_wrapper, reinterpret_cast<void**>(&g_game_tick_original));
	hooking::hook(diablo2::d2_client::get_base() + 0x5E650, draw_game_ui, reinterpret_cast<void**>(&g_draw_game_ui_original));
	//hooking::hook(diablo2::d2_client::get_base() + 0x150B0, handle_standart_packet, reinterpret_cast<void**>(&g_handle_packet_standart));

		// Get the path to the INI file
	std::string iniFilePath = std::filesystem::current_path().generic_string() + "/d2tweaks.ini";
	LPCSTR lpIniFilePath = iniFilePath.c_str();

	for (auto& m_module : m_modules) {
		if (m_module == nullptr)
			break;

		randStat = GetPrivateProfileIntA("RandStat", "stat", 0, lpIniFilePath);
		randStatRangeLow = GetPrivateProfileIntA("RandStat", "statRangeLow", 0, lpIniFilePath);
		randStatRangeHigh = GetPrivateProfileIntA("RandStat", "statRangeHigh", 0, lpIniFilePath);
		randStatBool = GetPrivateProfileIntA("RandStat", "statBool", 0, lpIniFilePath);

		spdlog::info("randStat = {0}", randStat);
		spdlog::info("randStatRangeLow = {0}", randStatRangeLow);
		spdlog::info("randStatRangeHigh = {0}", randStatRangeHigh);



		// Load and parse the INI file
		ParseIniFile(iniFilePath);
	}



	D2TEMPLATE_ApplyPatch(gpt_handle_cs_packet);
	//D2TEMPLATE_ApplyPatch(gpt_handle_sc_standart_packet);

	for (auto& m_module : m_modules) {
		if (m_module == nullptr)
			break;

		m_module->init_early();
	}
}


static int32_t g_ebp_send_to_client;
void d2_tweaks::client::client::handle_cs_packet(common::packet_header* packet, size_t size) {
#ifndef NDEBUG
	__asm {
		push [ebp + 0x2C];
		pop  [g_ebp_send_to_client];
	}
	spdlog::warn("[d2client SEND] Packet {} Message {} Size {} CallFrom {}", (void*)packet->d2_packet_type, (void*)packet->message_type, size, (void*)g_ebp_send_to_client);
#endif

	static common::packet_header dummy;
	static auto& instance = singleton<client>::instance();

	if (size == -1)
		return;

	auto handler = instance.m_packet_cs_handlers[packet->d2_packet_type];

	if (!handler)
		return;

	handler->handle_cs_packet(packet);
}


void d2_tweaks::client::client::handle_standart_packet(common::packet_header* packet, size_t size) {
	if (size == -1)
		return;

#ifndef NDEBUG
	spdlog::info("[d2client RECV] Packet {} Message {} Size {}", (void*)packet->d2_packet_type, (void*)packet->message_type, size);
#endif

	return;
}


void d2_tweaks::client::client::handle_packet(common::packet_header* packet, size_t size) {
	static common::packet_header dummy;
	static auto& instance = singleton<client>::instance();

	if (size == -1)
		return;

#ifndef NDEBUG
	spdlog::info("[d2client RECV] Packet {} Message {} Size {}", (void*)packet->d2_packet_type, (void*)packet->message_type, size);
#endif

	if (packet->d2_packet_type != dummy.d2_packet_type) {
		g_handle_packet(packet, size);
		return;
	}

	auto handler = instance.m_packet_handlers[packet->message_type];

	if (!handler)
		return;

	handler->handle_packet(packet);
}


static bool g_is_init = false;
void d2_tweaks::client::client::game_tick() {

	static auto& instance = singleton<client>::instance();  /// конфликт с текстом на d2 gl

	if (g_is_init == false) {
		D2TEMPLATE_Init();

		for (auto& m_module : instance.m_modules) {
			if (m_module == nullptr)
				break;

			m_module->init();
		}
		g_is_init = true;
	}

	for (auto & tick_handler : instance.m_tick_handlers) {
		if (tick_handler == nullptr)
			break;

		tick_handler->tick();
	}

	return;
}


int32_t d2_tweaks::client::client::draw_game_ui() {
	static auto& ui = singleton<ui::ui_manager>::instance();

	const auto result = g_draw_game_ui_original();

	ui.draw();

	return result;
}



void d2_tweaks::client::client::register_module(modules::client_module* module) {
	m_modules[m_module_id_counter++] = module;
}

void d2_tweaks::client::client::register_tick_handler(modules::client_module* module) {
	m_tick_handlers[m_tick_handler_id_counter++] = module;
}

void d2_tweaks::client::client::register_packet_cs_handler(common::packet_types_cs_t packet, common::message_types_t type, modules::client_module* module) {
	if (m_packet_cs_handlers[packet] != nullptr) {
		spdlog::warn("Clientside packet cs handler for {0} is already registered!", type);
	}

	m_packet_cs_handlers[packet] = module;
}

void d2_tweaks::client::client::register_packet_handler(common::message_types_t type, modules::client_module* module) {
	if (m_packet_handlers[type] != nullptr) {
		spdlog::warn("Clientside packet handler for {0} is already registered!", type);
	}

	m_packet_handlers[type] = module;
}

diablo2::structures::unit* d2_tweaks::client::client::get_client_unit(uint32_t type, uint32_t guid) {
	static auto units = diablo2::d2_client::get_client_unit_list();

	const auto index = guid & 127;

	auto result = units->unit_list[type][index];

	while (result != nullptr && result->guid != guid) {
		result = result->prev_unit;
	}

	return result;
}
