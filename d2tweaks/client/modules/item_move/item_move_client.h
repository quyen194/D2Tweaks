#pragma once

#include "d2tweaks/client/modules/base.h"

// Item moving between inventory pages (cube, inventory and stash) by ctrl+click
// client side

namespace d2_tweaks {
namespace client {
namespace modules {

class item_move final : public Base {
 public:
  void init() override;
  void init_early() override;
  void handle_packet(common::packet_header* packet) override;
};

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
