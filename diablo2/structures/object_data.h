#pragma once

#include <cstdint>

namespace diablo2 {
namespace structures {
struct objects_bin;

struct ObjectData {
  objects_bin* pt_objects_bin;
  uint8_t level_id;
};
}  // namespace structures
}  // namespace diablo2
