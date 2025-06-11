#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "fw/singleton.h"

#include "diablo2/structures/gfxdata.h"
#include "diablo2/d2win.h"

#include "d2tweaks/common/protocol.h"

#include "d2tweaks/client/module_base.h"

using namespace diablo2;
using namespace diablo2::structures;

extern GfxData g_gfxdata;  // global gfxdata

extern bool m_stats_enabled;
extern bool m_help_enabled;
extern bool m_cube_enabled;
extern bool m_stash_enabled;

namespace d2_tweaks {

class Client : public singleton<Client> {
  uint8_t m_module_id_counter;
  uint8_t m_tick_handler_id_counter;
  client::ModuleBase* m_modules[0xFF]{nullptr};  // max 255 modules atm.
  client::ModuleBase* m_tick_handlers[0xFF]{nullptr};  // max 255 handlers
  // max 255 handlers because of one-byte packet header
  client::ModuleBase* m_packet_handlers[0xFF]{nullptr};
  // max 255 handlers because of one-byte packet header
  client::ModuleBase* m_packet_cs_handlers[0xFF]{nullptr};

 public:
  explicit Client(token);

  void init();
  void register_module(client::ModuleBase* module);

  void register_tick_handler(client::ModuleBase* module);
  void register_packet_handler(common::message_types_t type,
                               client::ModuleBase* module);
  void register_packet_cs_handler(common::packet_types_cs_t packet,
                                  common::message_types_t type,
                                  client::ModuleBase* module);
  static Unit* get_client_unit(uint32_t type, uint32_t guid);

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

}  // namespace d2_tweaks
