#pragma once

#include "d2tweaks/server/module_base.h"

namespace d2_tweaks {
namespace server {

class Test final : public ModuleBase {
 public:
  void init() override;
};

}  // namespace server
}  // namespace d2_tweaks
