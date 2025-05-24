#pragma once

#include <cstdint>

namespace d2_tweaks {

namespace common {
struct packet_header;
}

namespace client {
namespace modules {

#define MODULE_INIT(module_name) static module_name g_instance;

class client_module {
 public:
  virtual ~client_module() = default;
  client_module();

  virtual void init() = 0;
  virtual void init_early() = 0;
  virtual void draw_ui();
  virtual void tick();
  virtual void handle_packet(common::packet_header* packet);
  virtual void handle_cs_packet(common::packet_header* packet);
};

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
