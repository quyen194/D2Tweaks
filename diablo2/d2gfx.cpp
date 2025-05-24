#include <common/ptr_wrapper.h>
#include <diablo2/d2gfx.h>

namespace diablo2 {

char* d2_gfx::get_base() {
  static char* base = reinterpret_cast<char*>(GetModuleHandle("d2gfx.dll"));
  return base;
}

bool d2_gfx::check_perspective_mode() {
  static wrap_func_std_import<bool()> fn_check_perspective_mode(10010, get_base());
  return fn_check_perspective_mode();
}

bool d2_gfx::check_perspective_coords(int32_t x, int32_t y) {
  static wrap_func_std_import<bool(int32_t, int32_t)> fn_check_perspective_coords(
      10065, get_base());
  return fn_check_perspective_coords(x, y);
}

int32_t d2_gfx::adjust_perspective_coords(int32_t x,
                                          int32_t y,
                                          int32_t* adjustX,
                                          int32_t* adjustY) {
  static wrap_func_std_import<int32_t(
      int32_t, int32_t, int32_t, int32_t*, int32_t*)>
      fn_adjust_perspective_coords(10066, get_base());
  return fn_adjust_perspective_coords(x, y, 0, adjustX, adjustY);
}

int32_t d2_gfx::get_resolution_mode() {
  static wrap_func_std_import<int32_t()> fn_get_resolution_mode(10005, get_base());
  return fn_get_resolution_mode();
}

void d2_gfx::draw_image(structures::gfxdata* data,
                        uint32_t x,
                        uint32_t y,
                        int32_t gamma,
                        int32_t drawType,
                        void* palette) {
  static wrap_func_std_import<void(
      structures::gfxdata*, uint32_t, uint32_t, int32_t, int32_t, void*)>
      fn_draw_image(10072, get_base());
  fn_draw_image(data, x, y, gamma, drawType, palette);
}

void d2_gfx::draw_filled_rect(
    int left, int top, int right, int bottom, DWORD color, int transTbl) {
  static wrap_func_std_import<void(int, int, int, int, DWORD, int)>
      fn_draw_boxed_text(10056, get_base());
  fn_draw_boxed_text(left, top, right, bottom, color, transTbl);
}

}  // namespace diablo2
