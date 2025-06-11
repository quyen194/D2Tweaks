#pragma once

#include "d2tweaks/client/modules/base.h"

namespace d2_tweaks {
namespace client {
namespace modules {

class item_drop_message final : public Base {
 public:
  void init() override;
  void init_early() override;
  void handle_packet(common::packet_header* packet) override;
  static void GamePacketReceivedInterceptASM();
  static void __fastcall GamePacketReceivedIntercept(uint8_t* packet,
                                                     size_t size);
};

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
