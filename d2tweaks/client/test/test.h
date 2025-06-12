#pragma once

#include "d2tweaks/client/module_base.h"

// Test client side module

namespace d2_tweaks {
namespace client {

class Test final : public ModuleBase {
 public:
  void init() override;
  void init_early() override;
  void handle_packet(packet_header* packet) override;
};

}  // namespace client
}  // namespace d2_tweaks
