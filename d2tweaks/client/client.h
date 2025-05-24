#pragma once

#include <d2tweaks/client/modules/client_module.h>
#include <d2tweaks/common/protocol.h>

#include <diablo2/d2win.h>
#include <diablo2/structures/gfxdata.h>
#include <fw/singleton.h>

#include <cstdint>
#include <string>
#include <vector>

using namespace diablo2;
using namespace diablo2::structures;

// Define the structure to hold stat information
struct StatEntry {
  std::wstring stat_display_string;
  ui_color_t colorStat, colorStatValue;
  int x1, y1, x2, y2, is_item_stat, item_type_id, stat, op,
      param = 0;  // x1,y1 stat_display_string | x2,y2 statValue
};

// Declaration of the global variable
extern std::vector<StatEntry> globalStatsVector;

extern gfxdata g_gfxdata;  // global gfxdata

extern bool m_stats_enabled;
extern bool m_help_enabled;
extern bool m_cube_enabled;
extern bool m_stash_enabled;

namespace d2_tweaks {
namespace client {

using namespace modules;

class client : public singleton<client> {
  uint8_t m_module_id_counter;
  uint8_t m_tick_handler_id_counter;
  client_module* m_modules[0xFF]{nullptr};  // max 255 modules atm.
  client_module* m_tick_handlers[0xFF]{nullptr};  // max 255 handlers
  // max 255 handlers because of one-byte packet header
  client_module* m_packet_handlers[0xFF]{nullptr};
  // max 255 handlers because of one-byte packet header
  client_module* m_packet_cs_handlers[0xFF]{nullptr};
 public:
  explicit client(token);

  void init();
  void register_module(client_module* module);

  void register_tick_handler(client_module* module);
  void register_packet_handler(common::message_types_t type,
                               client_module* module);
  void register_packet_cs_handler(common::packet_types_cs_t packet,
                                  common::message_types_t type,
                                  client_module* module);
  static unit* get_client_unit(uint32_t type, uint32_t guid);

 private:
  // static void __fastcall game_loop_start();
  static void __fastcall handle_standart_packet(common::packet_header* packet,
                                                size_t size);
  static void __fastcall handle_cs_packet(common::packet_header* packet,
                                          size_t size);
  static void __fastcall handle_packet(common::packet_header* packet,
                                       size_t size);
  static void __fastcall game_tick();
  static int32_t __stdcall draw_game_ui();
};

}  // namespace client
}  // namespace d2_tweaks
