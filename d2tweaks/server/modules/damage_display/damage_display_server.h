#pragma once

#include <d2tweaks/server/modules/server_module.h>

#include <cstdint>

// Display damage server side

namespace diablo2 {
namespace structures {
struct inventory;
struct game;
struct unit;
}  // namespace structures
}  // namespace diablo2

namespace d2_tweaks {
namespace server {
class server;

namespace modules {
class damage_display final : public server_module {
 public:
  void init() override;

  bool handle_packet(game* game,
                     unit* player,
                     common::packet_header* packet) override;
  void tick(game* game, unit* unit) override;
};
}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
