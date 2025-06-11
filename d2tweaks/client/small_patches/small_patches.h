#pragma once

#include "d2tweaks/client/module_base.h"

// Client side patches that are too small to implement as separate modules

namespace d2_tweaks {
namespace client {

class small_patches final : public ModuleBase {
 public:
  void init() override;
  void init_early() override;
};

}  // namespace client
}  // namespace d2_tweaks
