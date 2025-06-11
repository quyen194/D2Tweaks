
#include <Windows.h>

#include "d2tweaks/client/client.h"
#include "d2tweaks/client/modules/client_module.h"

namespace d2_tweaks {
namespace client {
namespace modules {

client_module::client_module() {
  Client::instance().register_module(this);
}

void client_module::draw_ui() {}

void client_module::tick() {}

void client_module::handle_packet(common::packet_header* packet) {}

void client_module::handle_cs_packet(common::packet_header* packet) {}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
