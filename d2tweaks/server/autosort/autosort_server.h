#pragma once

#include <cstdint>

#include "diablo2/structures/inventory.h"

#include "d2tweaks/server/module_base.h"

// Inventory auto sort module server side

namespace d2_tweaks {
namespace server {

using namespace diablo2::structures;

class autosort final : public ModuleBase {
 public:
  void init() override;

  bool handle_packet(Game* game,
                     Unit* player,
                     common::packet_header* packet) override;

 private:
  bool sort(Game* game, Unit* player, uint8_t page);
  bool find_free_space(Inventory* inv,
                       Unit* item,
                       int32_t inventoryIndex,
                       char page,
                       uint32_t& x,
                       uint32_t& y,
                       bool isCharmZone);
};

}  // namespace server
}  // namespace d2_tweaks
