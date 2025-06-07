#include <diablo2/fog.h>
#include <common/ptr_wrapper.h>

namespace diablo2 {

char* fog::get_base() {
  static auto base = reinterpret_cast<char*>(GetModuleHandle("fog.dll"));
  return base;
}

void fog::get_save_path(char* buffer, size_t bufferSize) {
  static wrap_func_fast_import<void(char*, size_t)> get_save_path(10115,
                                                                  get_base());
  return get_save_path(buffer, bufferSize);
}

bool fog::mpq_open_file(char* path, structures::FileHandle** outHandle) {
  static wrap_func_fast_import<BOOL(char*, structures::FileHandle**)>
      mpq_open_file(10102, get_base());
  return mpq_open_file(path, outHandle);
}

bool fog::mpq_close_file(structures::FileHandle* handle) {
  static wrap_func_fast_import<BOOL(structures::FileHandle*)> mpq_close_file(
      10103, get_base());
  return mpq_close_file(handle);
}

bool fog::mpq_read_file(structures::FileHandle* handle,
                        void* buffer,
                        size_t size,
                        size_t* bytesToRead) {
  static wrap_func_fast_import<BOOL(structures::FileHandle*,
                                    void*,
                                    size_t,
                                    size_t*,
                                    int32_t,
                                    int32_t,
                                    int32_t)>
      mpq_read_fiile(10104, get_base());
  return mpq_read_fiile(handle, buffer, size, bytesToRead, 0, 0, 0);
}

size_t fog::mpq_get_file_size(structures::FileHandle* handle, size_t* compressedSize) {
  static wrap_func_fast_import<size_t(structures::FileHandle*, size_t*)>
      mpq_get_file_size(10105, get_base());
  return mpq_get_file_size(handle, compressedSize);
}

}  // namespace diablo2
