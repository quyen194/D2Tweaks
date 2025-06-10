#pragma once

#include <cstdint>

namespace diablo2 {

namespace structures {
struct GameServer;
}

class d2_net {
 public:
  static char* get_base();

  static int32_t send_to_server(int32_t queue, void* data, size_t size);
  static int32_t send_to_client(int32_t queue,
                                int32_t clientId,
                                void* packet,
                                size_t size);
};

}  // namespace diablo2
