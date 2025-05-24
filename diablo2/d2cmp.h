#pragma once

namespace diablo2 {
namespace structures {
struct gfxdata;

class d2_cmp {
 public:
  static char* get_base();

  static bool init_gfx_data(gfxdata* gfxdata);
};

}  // namespace structures
}  // namespace diablo2
