#pragma once

#include "d2tweaks/server/modules/server_module.h"

namespace d2_tweaks {
namespace server {
class server;

namespace modules {

class auto_gold_pickup final : public server_module {
 public:
  void init() override;
  void tick(Game* game, Unit* unit) override;
  bool handle_packet(Game* game,
                     Unit* player,
                     common::packet_header* packet) override;
  bool au_pickup_gold(Game* game, Unit* pUnit, Unit* item);
};

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
