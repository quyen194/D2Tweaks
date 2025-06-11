#pragma once

#include "d2tweaks/client/module_base.h"

namespace d2_tweaks {
namespace client {

class ItemDropMessage final : public ModuleBase {
 public:
  void init() override;
  void init_early() override;
  void handle_packet(common::packet_header* packet) override;
  static void GamePacketReceivedInterceptASM();
  static void __fastcall GamePacketReceivedIntercept(uint8_t* packet,
                                                     size_t size);
};

}  // namespace client
}  // namespace d2_tweaks
