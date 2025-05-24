#pragma once

#include <d2tweaks/server/modules/server_module.h>

#include <cstdint>

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

class trader_update final : public server_module {
 public:
  void init() override;
  void tick(game* game, unit* unit) override;
  bool handle_packet(game* game,
                     unit* player,
                     common::packet_header* packet) override;

 // private:
 //  bool find_free_space(inventory* inv,
 //                       unit* item,
 //                       int32_t inventoryIndex,
 //                       char page,
 //                       uint32_t& x,
 //                       uint32_t& y);
 // bool send_to_cube(game* game, unit* player, unit* item);
 //  bool move_item_to(game* game,
 //                    unit* player,
 //                    common::packet_header* packet);
};

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
