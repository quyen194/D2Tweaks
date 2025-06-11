#pragma once

#include "d2tweaks/client/module_base.h"

namespace d2_tweaks {
namespace client {

class LootFilter final : public ModuleBase {
 public:
  void init() override;
  void init_early() override;
};

}  // namespace client
}  // namespace d2_tweaks
