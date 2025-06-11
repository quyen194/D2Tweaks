
#include <Windows.h>

#include "d2tweaks/server/server.h"
#include "d2tweaks/server/modules/base.h"

namespace d2_tweaks {
namespace server {
namespace modules {

Base::Base() {
  Server::instance().register_module(this);
}

bool Base::handle_packet(Game* game,
                         Unit* player,
                         common::packet_header* packet) {
  return false;
}

void Base::tick(Game* game, Unit* unit) {}

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
