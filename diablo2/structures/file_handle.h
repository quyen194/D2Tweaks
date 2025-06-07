#pragma once

#include <cstdint>

namespace diablo2 {
namespace structures {
struct FileHandle {
  char pad_0000[0x134];
  void* file;
  int32_t position;
  int32_t size;
  char pad_0001[0x18];
};
static_assert(sizeof(FileHandle) == 0x158);
}  // namespace structures
}  // namespace diablo2
