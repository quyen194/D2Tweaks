#pragma once

#include "d2tweaks/client/modules/base.h"

namespace d2_tweaks {
namespace client {
namespace modules {

class auto_item_pickup final : public Base {
 public:
  void init() override;
  void init_early() override;
  void handle_packet(common::packet_header* packet) override;
  void tick() override;
};

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
