#pragma once

#include "d2tweaks/server/module_base.h"

namespace d2_tweaks {
namespace server {

class auto_item_pickup final : public ModuleBase {
 public:
  void init() override;
  void tick(Game* game, Unit* unit) override;
  bool handle_packet(Game* game,
                     Unit* player,
                     common::packet_header* packet) override;
  bool au_pickup_item(Game* game, Unit* unit, uint32_t guid);
};

}  // namespace server
}  // namespace d2_tweaks
