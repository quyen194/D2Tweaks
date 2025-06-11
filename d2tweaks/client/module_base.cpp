
#include <Windows.h>

#include "d2tweaks/client/client.h"
#include "d2tweaks/client/module_base.h"

namespace d2_tweaks {
namespace client {

ModuleBase::ModuleBase() {
  Client::instance().register_module(this);
}

void ModuleBase::draw_ui() {}

void ModuleBase::tick() {}

void ModuleBase::handle_packet(common::packet_header* packet) {}

void ModuleBase::handle_cs_packet(common::packet_header* packet) {}

}  // namespace client
}  // namespace d2_tweaks
