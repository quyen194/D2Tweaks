#pragma once

#include "d2tweaks/client/modules/base.h"

namespace d2_tweaks {
namespace client {
namespace modules {

class loot_filter final : public Base {
 public:
  void init() override;
  void init_early() override;
};

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
