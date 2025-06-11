#pragma once

#include "d2tweaks/client/module_base.h"

// Inventory auto sort module client side

namespace d2_tweaks {
namespace client {

class autosort final : public ModuleBase {
 public:
  void init() override;
  void init_early() override;
  void handle_packet(common::packet_header* packet) override;
};

}  // namespace client
}  // namespace d2_tweaks
