
#include <Windows.h>

#include "d2tweaks/server/server.h"
#include "d2tweaks/server/module_base.h"

namespace d2_tweaks {
namespace server {

ModuleBase::ModuleBase() {
  Server::instance().register_module(this);
}

bool ModuleBase::handle_packet(Game* game,
                         Unit* player,
                         common::packet_header* packet) {
  return false;
}

void ModuleBase::tick(Game* game, Unit* unit) {}

}  // namespace server
}  // namespace d2_tweaks
