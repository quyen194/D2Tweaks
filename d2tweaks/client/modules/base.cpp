
#include <Windows.h>

#include "d2tweaks/client/client.h"
#include "d2tweaks/client/modules/base.h"

namespace d2_tweaks {
namespace client {
namespace modules {

Base::Base() {
  Client::instance().register_module(this);
}

void Base::draw_ui() {}

void Base::tick() {}

void Base::handle_packet(common::packet_header* packet) {}

void Base::handle_cs_packet(common::packet_header* packet) {}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
