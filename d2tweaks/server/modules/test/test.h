#pragma once

#include "d2tweaks/server/modules/base.h"

namespace d2_tweaks {
namespace server {
namespace modules {

class test final : public Base {
 public:
  void init() override;
};

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
