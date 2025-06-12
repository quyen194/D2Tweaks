#pragma once

#include <cstdint>

#include "d2tweaks/client/module_base.h"

namespace d2_tweaks {
namespace client {

class Transmute final : public ModuleBase {
 public:
  void init() override;
  void init_early() override;
  void handle_packet(common::packet_header* packet) override;
  void tick() override;
};

}  // namespace client
}  // namespace d2_tweaks
