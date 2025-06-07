#pragma once

#include <cstdint>

namespace diablo2 {
namespace structures {
struct ObjectsBin;

struct ObjectData {
  ObjectsBin* pt_objects_bin;
  uint8_t level_id;
};
}  // namespace structures
}  // namespace diablo2
