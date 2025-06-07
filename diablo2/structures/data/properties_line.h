#pragma once

#include <cstdint>

namespace diablo2 {
namespace structures {
struct PropertiesLine {
  uint16_t prop;
  uint8_t set[8];
  uint16_t val[7];
  uint8_t func[8];
  uint16_t stat[7];
};
}  // namespace structures
}  // namespace diablo2
