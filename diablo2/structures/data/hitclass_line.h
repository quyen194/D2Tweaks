#pragma once

#include <cstdint>

namespace diablo2 {
namespace structures {
struct HitClassLine {
  union {
    uint32_t code;
    char str[4];
  };
};
}  // namespace structures
}  // namespace diablo2
