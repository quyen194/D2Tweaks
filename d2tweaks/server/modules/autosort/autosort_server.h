#pragma once

#include <d2tweaks/server/modules/server_module.h>

#include <cstdint>

// Inventory auto sort module server side

namespace diablo2 {
namespace structures {
struct game;
struct inventory;
struct unit;
}  // namespace structures
}  // namespace diablo2

namespace d2_tweaks {
namespace server {
class server;

namespace modules {

using namespace diablo2::structures;

class autosort final : public server_module {
 public:
  void init() override;

  bool handle_packet(game* game,
                     unit* player,
                     common::packet_header* packet) override;

 private:
  bool sort(game* game, unit* player, uint8_t page);
  bool find_free_space(inventory* inv,
                       unit* item,
                       int32_t inventoryIndex,
                       char page,
                       uint32_t& x,
                       uint32_t& y,
                       bool isCharmZone);
};

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
