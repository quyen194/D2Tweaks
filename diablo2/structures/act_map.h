#pragma once

#include <cstdint>

namespace diablo2 {
namespace structures {
struct Room;

struct ActMap {  // ptGame+BC size=0x60
  uint32_t is_not_managed;
  uint32_t uk4;
  uint32_t uk8;  // size = 0x488
  Room* pt_first_room;
};
}  // namespace structures
}  // namespace diablo2
