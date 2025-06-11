#pragma once

#include "d2tweaks/client/modules/base.h"

// Display damage client side

namespace d2_tweaks {
namespace client {
namespace modules {

class damage_display final : public Base {
 public:
  void init() override;
  void init_early() override;
  void handle_packet(common::packet_header* packet) override;
  void tick() override;
};

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
