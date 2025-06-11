#pragma once

#include <cstdint>

namespace diablo2 {
namespace structures {
struct Game;
struct Unit;
}  // namespace structures
}  // namespace diablo2

namespace d2_tweaks {
namespace common {
struct packet_header;
}

namespace server {

using namespace diablo2::structures;

#define MODULE_INIT(module_name) static module_name g_instance;

class ModuleBase {
 public:
  virtual ~ModuleBase() = default;
  ModuleBase();

  virtual void init() = 0;

  /**
   * \brief
   * \param game
   * \param player
   * \param packet
   * \return true - block further packet processing, false - pass packet to game
   */
  virtual bool handle_packet(Game* game,
                             Unit* player,
                             common::packet_header* packet);
  virtual void tick(Game* game,
                    Unit* unit);
};

}  // namespace server
}  // namespace d2_tweaks
