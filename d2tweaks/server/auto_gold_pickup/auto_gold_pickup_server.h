#pragma once

#include "d2tweaks/server/module_base.h"

namespace d2_tweaks {
namespace server {

class AutoGoldPickup final : public ModuleBase {
 public:
  void init() override;
  void tick(Game* game, Unit* unit) override;
  bool handle_packet(Game* game, Unit* player, packet_header* packet) override;
  bool au_pickup_gold(Game* game, Unit* pUnit, Unit* item);
};

}  // namespace server
}  // namespace d2_tweaks
