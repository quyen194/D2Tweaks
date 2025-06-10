
#include <Windows.h>

#include "diablo2/storm.h"

namespace diablo2 {

char* storm::get_base() {
  static auto base = reinterpret_cast<char*>(GetModuleHandle("storm.dll"));
  return base;
}

}  // namespace diablo2
