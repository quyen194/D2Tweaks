#pragma once

#include <d2tweaks/server/modules/server_module.h>

#include <cstdint>

// Item moving between inventory pages (cube, inventory and stash) by ctrl+click
// server side

namespace diablo2 {
namespace structures {
struct inventory;
struct Game;
struct unit;
}  // namespace structures
}  // namespace diablo2

namespace d2_tweaks {
namespace server {
class server;

namespace modules {
class item_move final : public server_module {
 public:
  void init() override;

  bool handle_packet(Game* game,
                     unit* player,
                     common::packet_header* packet) override;

 private:
  bool find_free_space(inventory* inv,
                       unit* item,
                       int32_t inventoryIndex,
                       char page,
                       uint32_t& x,
                       uint32_t& y);
};
}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
