
#include <Windows.h>

#include "common/ptr_wrapper.h"

#include "diablo2/d2cmp.h"

namespace diablo2 {
namespace structures {

char* d2_cmp::get_base() {
  static const auto base = reinterpret_cast<char*>(GetModuleHandle("d2cmp.dll"));
  return base;
}

bool d2_cmp::init_gfx_data(GfxData* gfx_data) {
  static wrap_func_std_import<BOOL(GfxData*, int32_t, int32_t)> init_gfx_data(
      10055, get_base());
  return init_gfx_data(gfx_data, 0, 1) > 0;
}

}  // namespace structures
}  // namespace diablo2
