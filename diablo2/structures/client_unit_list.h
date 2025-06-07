#pragma once

namespace diablo2 {
namespace structures {
struct Unit;

struct ClientUnitList {
  Unit* unit_list[5][128];  // 0x1120
};
}  // namespace structures
}  // namespace diablo2
