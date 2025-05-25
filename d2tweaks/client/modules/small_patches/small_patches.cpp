#include <d2tweaks/client/modules/small_patches/small_patches.h>

#include <Windows.h>

#include <diablo2/d2client.h>
#include <diablo2/d2win.h>
#include <common/hooking.h>
#include <diablo2/d2gfx.h>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace client {
namespace modules {

MODULE_INIT(small_patches)

static int(__cdecl* g_is_iconic_original)();
static int __cdecl is_iconic() {
  return 0;
}

void small_patches::init_early() {
}

void small_patches::init() {
  // // Ingame FPS unlock
  // DWORD oldProtect;
  // const char* config_path = common::get_config_path();
  // 
  // if (GetPrivateProfileInt("modules", "SmallPatches", 1, config_path)) {
  //   if (VirtualProtect(d2_client::get_base() + 0xA2C9,
  //                      0x04,
  //                      PAGE_EXECUTE_READWRITE,
  //                      &oldProtect))
  //     *reinterpret_cast<uint32_t*>(d2_client::get_base() + 0xA2C9) =
  //         0x90909090;
  // 
  //   // Main menu FPS unlock
  //   if (VirtualProtect(d2_win::get_base() + 0xD029,
  //                      0x03,
  //                      PAGE_EXECUTE_READWRITE,
  //                      &oldProtect))
  //     memset(d2_win::get_base() + 0xD029, 0x90, 0x03);
  // 
  //   return;
  // 
  //   // Window auto-hiding on focus loss
  //   hooking::hook<10026>(
  //       d2_gfx::get_base(), is_iconic, &g_is_iconic_original);
  // }
}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
