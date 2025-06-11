#pragma once

#include <cstdint>
#include <functional>

#include "fw/singleton.h"

#include "d2tweaks/common/protocol.h"

#include "d2tweaks/server/module_base.h"

namespace diablo2 {
namespace structures {
enum class unit_type_t;
struct Game;
struct Inventory;
struct Unit;
struct NetClient;
}  // namespace structures
}  // namespace diablo2

using namespace diablo2::structures;

namespace d2_tweaks {
namespace common {
struct packet_header;
}

class Server : public singleton<Server> {
  uint8_t m_module_id_counter;
  uint8_t m_tick_handler_id_counter;
  server::ModuleBase* m_modules[0xFF]{nullptr};        // max 255 modules atm.
  server::ModuleBase* m_tick_handlers[0xFF]{nullptr};  // max 255 modules atm.
  // max 255 handlers because of one-byte packet header
  server::ModuleBase* m_packet_handlers[0xFF]{nullptr};

 public:
  explicit Server(token);

  void init();

  void send_packet(NetClient* client,
                   common::packet_header* packet,
                   size_t size);
  bool handle_packet(Game* game, Unit* player, common::packet_header* packet);

  void register_module(server::ModuleBase* module);

  void register_tick_handler(server::ModuleBase* module);
  void register_packet_handler(common::message_types_t type, server::ModuleBase* module);

  Unit* get_server_unit(Game* game, uint32_t guid, unit_type_t type);
  void iterate_server_units(Game* game,
                            unit_type_t type,
                            const std::function<bool(Unit*)>& cb);

 private:
  static int32_t __fastcall net_tick(Game* game,
                                     Unit* unit,
                                     int32_t a3,
                                     int32_t a4);
};

}  // namespace d2_tweaks
