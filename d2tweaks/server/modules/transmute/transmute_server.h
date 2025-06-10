#pragma once

#include <cstdint>

#include "d2tweaks/server/modules/server_module.h"

namespace diablo2 {
namespace structures {
struct Inventory;
struct Game;
struct Unit;
}  // namespace structures
}  // namespace diablo2

namespace d2_tweaks {
namespace server {
class server;

namespace modules {
class transmute final : public server_module {
 public:
  void init() override;
  void tick(Game* game, Unit* unit) override;
  bool handle_packet(Game* game,
                     Unit* player,
                     common::packet_header* packet) override;

 private:
  bool find_free_space(Inventory* inv,
                       Unit* item,
                       int32_t inventoryIndex,
                       char page,
                       uint32_t& x,
                       uint32_t& y);
  bool send_to_cube(Game* game, Unit* player, Unit* item);
  bool move_item_to(Game* game, Unit* player, common::packet_header* packet);
};
}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
