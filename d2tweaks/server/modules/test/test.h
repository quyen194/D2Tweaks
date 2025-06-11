#pragma once

#include "d2tweaks/server/modules/server_module.h"

namespace d2_tweaks {
namespace server {
namespace modules {

class test final : public server_module {
 public:
  void init() override;
};

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
