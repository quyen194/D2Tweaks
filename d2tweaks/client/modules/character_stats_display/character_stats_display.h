#pragma once

#include "d2tweaks/client/modules/base.h"

// Inventory auto sort module client side

namespace d2_tweaks {
namespace client {
namespace modules {

class character_stats_display final : public Base {
 public:
  void init() override;
  void init_early() override;
};

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
