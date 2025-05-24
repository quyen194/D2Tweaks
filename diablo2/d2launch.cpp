#include <diablo2/d2launch.h>

#include <Windows.h>

namespace diablo2 {

char* d2_launch::get_base() {
  static char* base = reinterpret_cast<char*>(GetModuleHandle("d2launch.dll"));
  return base;
}

}  // namespace diablo2
