#pragma once

#include "d2tweaks/client/module_base.h"

// Inventory auto sort module client side

namespace d2_tweaks {
namespace client {

class character_stats_display final : public ModuleBase {
 public:
  void init() override;
  void init_early() override;
};

}  // namespace client
}  // namespace d2_tweaks
