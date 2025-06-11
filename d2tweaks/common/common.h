#pragma once

#include "fw/singleton.h"

#include "d2tweaks/common/protocol.h"

namespace d2_tweaks {
namespace common {

class Common : public singleton<Common> {
 public:
  explicit Common(token);

  void init();

  bool get_packet_size_cs(packet_header* packet, size_t& size);
  bool get_packet_size_sc(packet_header* packet, size_t& size);
};

}  // namespace common
}  // namespace d2_tweaks
