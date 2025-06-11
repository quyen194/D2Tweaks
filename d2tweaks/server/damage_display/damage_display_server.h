#pragma once

#include <cstdint>

#include "d2tweaks/server/module_base.h"

// Display damage server side

namespace diablo2 {
namespace structures {
struct Inventory;
struct Game;
struct Unit;
}  // namespace structures
}  // namespace diablo2

namespace d2_tweaks {
namespace server {

class damage_display final : public ModuleBase {
 public:
  void init() override;

  bool handle_packet(Game* game,
                     Unit* player,
                     common::packet_header* packet) override;
  void tick(Game* game, Unit* unit) override;
};

}  // namespace server
}  // namespace d2_tweaks
