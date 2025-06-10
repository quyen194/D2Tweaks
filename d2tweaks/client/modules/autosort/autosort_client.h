#pragma once

#include "d2tweaks/client/modules/client_module.h"

// Inventory auto sort module client side

namespace d2_tweaks {
namespace client {
namespace modules {

class autosort final : public client_module {
 public:
  void init() override;
  void init_early() override;
  void handle_packet(common::packet_header* packet) override;
};

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
