#include <diablo2/storm.h>

#include <Windows.h>

namespace diablo2 {

char* storm::get_base() {
  static auto base = reinterpret_cast<char*>(GetModuleHandle("storm.dll"));
  return base;
}

}  // namespace diablo2
