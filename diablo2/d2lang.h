#pragma once

#include <cstdint>
#include <functional>

#include "diablo2/structures/unit.h"

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
