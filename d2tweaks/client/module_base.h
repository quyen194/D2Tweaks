#pragma once

#include <cstdint>

#include "d2tweaks/common/protocol.h"

namespace d2_tweaks {
namespace client {

#define MODULE_INIT(module_name) static module_name g_instance;

class ModuleBase {
 public:
  virtual ~ModuleBase() = default;
  ModuleBase();

  virtual void init() = 0;
  virtual void init_early() = 0;
  virtual void draw_ui();
  virtual void tick();
  virtual void handle_packet(common::packet_header* packet);
  virtual void handle_cs_packet(common::packet_header* packet);
};

}  // namespace client
}  // namespace d2_tweaks
