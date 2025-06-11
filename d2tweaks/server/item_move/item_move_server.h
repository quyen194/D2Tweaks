#pragma once

#include <cstdint>

#include "diablo2/structures/inventory.h"

#include "d2tweaks/server/module_base.h"

// Item moving between inventory pages (cube, inventory and stash) by ctrl+click
// server side

namespace d2_tweaks {
namespace server {

class item_move final : public ModuleBase {
 public:
  void init() override;

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
};

}  // namespace server
}  // namespace d2_tweaks
