#pragma once

#include <cstdint>

#include <fw/singleton.h>

#include <diablo2/d2win.h>
#include <string>
#include <vector>
#include <diablo2/structures/gfxdata.h>

// Define the structure to hold stat information
struct StatEntry {
	std::wstring stat_display_string;
	diablo2::ui_color_t colorStat, colorStatValue;
	int x1, y1, x2, y2, is_item_stat, item_type_id, stat = 0; // x1,y1 stat_display_string | x2,y2 statValue
};

extern std::vector<StatEntry> globalStatsVector; // Declaration of the global variable

extern diablo2::structures::gfxdata g_gfxdata; // global gfxdata

extern int randStat;
extern int randStatRangeLow;
extern int randStatRangeHigh;
extern int randStatBool;

namespace diablo2 {
	namespace structures {
		struct unit;
	}
}

namespace d2_tweaks {
	namespace common {
		struct packet_header;
		enum packet_types_cs_t;
		enum message_types_t;
	}

	namespace client {
		namespace modules {
			class client_module;
		}

		class client : public singleton<client> {
			uint8_t m_module_id_counter;
			uint8_t m_tick_handler_id_counter;
			modules::client_module* m_modules[0xFF]{ nullptr }; //max 255 modules atm.
			modules::client_module* m_tick_handlers[0xFF]{ nullptr }; //max 255 handlers
			modules::client_module* m_packet_handlers[0xFF]{ nullptr }; //max 255 handlers because of one-byte packet header
			modules::client_module* m_packet_cs_handlers[0xFF]{ nullptr }; //max 255 handlers because of one-byte packet header
		public:
			explicit client(token);

			void init();
			void register_module(modules::client_module* module);

			void register_tick_handler(modules::client_module* module);
			void register_packet_handler(common::message_types_t type, modules::client_module* module);
			void register_packet_cs_handler(common::packet_types_cs_t packet, common::message_types_t type, modules::client_module* module);
			static diablo2::structures::unit* get_client_unit(uint32_t type, uint32_t guid);

		private:
			//static void __fastcall game_loop_start();
			static void  __fastcall handle_standart_packet(common::packet_header* packet, size_t size);
			static void __fastcall handle_cs_packet(common::packet_header* packet, size_t size);
			static void __fastcall handle_packet(common::packet_header* packet, size_t size);
			static void __fastcall game_tick();
			static int32_t __stdcall draw_game_ui();
		};
	}
}