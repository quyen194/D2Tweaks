#pragma once

#include <diablo2/structures/unit.h>

#include <cstdint>
#include <functional>

namespace diablo2 {

namespace structures {
struct Unit;
}

class d2_lang {
 public:
  static char* get_base();
  static wchar_t* get_string_from_index(short);
};

}  // namespace diablo2
