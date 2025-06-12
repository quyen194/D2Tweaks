#pragma once

#include <cstdint>

#include "diablo2/structures/inventory.h"

#include "d2tweaks/server/module_base.h"

namespace d2_tweaks {
namespace server {

class Transmute final : public ModuleBase {
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

}  // namespace server
}  // namespace d2_tweaks
