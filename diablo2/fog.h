#pragma once

#include <Windows.h>

#include <cstdint>

namespace diablo2 {
namespace structures {
struct FileHandle;
}

class fog {
 public:
  static char* get_base();

  static void get_save_path(char* buffer, size_t bufferSize);

  static bool mpq_open_file(char* path, structures::FileHandle** outHandle);
  static bool mpq_close_file(structures::FileHandle* handle);
  static bool mpq_read_file(structures::FileHandle* handle,
                            void* buffer,
                            size_t size,
                            size_t* bytesToRead);
  static size_t mpq_get_file_size(structures::FileHandle* handle,
                                  size_t* compressedSize);
};

}  // namespace diablo2
