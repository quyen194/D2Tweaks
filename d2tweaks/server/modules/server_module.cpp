
#include <Windows.h>

#include "d2tweaks/server/server.h"
#include "d2tweaks/server/modules/server_module.h"

namespace d2_tweaks {
namespace server {
namespace modules {

server_module::server_module() {
  Server::instance().register_module(this);
}

bool server_module::handle_packet(Game* game,
                                  Unit* player,
                                  common::packet_header* packet) {
  return false;
}

void server_module::tick(Game* game, Unit* unit) {}

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
