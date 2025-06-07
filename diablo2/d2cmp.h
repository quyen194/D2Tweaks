#pragma once

namespace diablo2 {
namespace structures {
struct GfxData;

class d2_cmp {
 public:
  static char* get_base();

  static bool init_gfx_data(GfxData* gfxdata);
};

}  // namespace structures
}  // namespace diablo2
