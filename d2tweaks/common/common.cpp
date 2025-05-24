#include <d2tweaks/common/common.h>

#include <spdlog/spdlog.h>

#include <d2tweaks/common/protocol.h>
#include <d2tweaks/common/asset_manager.h>

#include <diablo2/d2net.h>
#include <common/hooking.h>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace common {

static int32_t(__fastcall* g_get_packet_size_client_to_server)(
    packet_header* data, size_t size, size_t* sizeOut);
static char(__fastcall* g_get_packet_size_server_to_client)(packet_header* data,
                                                            size_t size,
                                                            size_t* outSize);

static int32_t(__stdcall* g_net_send_to_server)(int32_t queue,
                                                packet_header* packet,
                                                size_t size);
static int32_t(__stdcall* g_net_send_to_client)(int32_t queue,
                                                int32_t clientId,
                                                packet_header* packet,
                                                size_t size);

static int32_t g_ebp_send_to_server;
static int32_t g_ebp_send_to_client;
static int32_t __stdcall net_send_to_server(int32_t queue,
                                            packet_header* packet,
                                            size_t size) {
  __asm {
    push[ebp + 4]
    pop[g_ebp_send_to_server]
  }

  spdlog::debug("[d2net SEND C >>> S] Queue {}  Packet {}  Size {} CallFrom {}",
                queue,
                (void*) packet->d2_packet_type,
                size,
                (void*) g_ebp_send_to_server);
  //do something
  return g_net_send_to_server(queue, packet, size);
}

static int32_t __stdcall net_send_to_client(int32_t queue,
                                            int32_t clientId,
                                            packet_header* packet,
                                            size_t size) {
  __asm {
    push[ebp + 4]
    pop[g_ebp_send_to_client]
  }

  spdlog::error(
      "[d2net SEND S >>> C] Queue {}  Packet {}  Size {} ClientID {} CallFrom {}",
      queue,
      (void*) packet->d2_packet_type,
      size,
      clientId,
      (void*) g_ebp_send_to_client);
  //do something
  return g_net_send_to_client(queue, clientId, packet, size);
}

static int32_t __fastcall get_packet_size_client_to_server(packet_header* data,
                                                           size_t size,
                                                           size_t* sizeOut) {
  static packet_header dummy;
  static auto& instance = singleton<common>::instance();

  if (data->d2_packet_type == dummy.d2_packet_type) {
    size_t packetSize;

    if (!instance.get_packet_size_cs(data, packetSize))
      return g_get_packet_size_client_to_server(data, size, sizeOut);

    *sizeOut = packetSize;
    return packetSize;
  }

  return g_get_packet_size_client_to_server(data, size, sizeOut);
}

static char __fastcall get_packet_size_server_to_client(packet_header* data,
                                                        size_t size,
                                                        size_t* sizeOut) {
  static packet_header dummy;
  static auto& instance = singleton<common>::instance();

  if (data->d2_packet_type == dummy.d2_packet_type) {
    size_t packetSize;

    if (!instance.get_packet_size_sc(data, packetSize))
      return g_get_packet_size_server_to_client(data, size, sizeOut);

    *sizeOut = packetSize;
    return static_cast<char>(packetSize);
  }

  return g_get_packet_size_server_to_client(data, size, sizeOut);
}

common::common(token) {}

void common::init() {
  singleton<asset_manager>::instance().init();

  hooking::hook(reinterpret_cast<void*>(d2_net::get_base() + 0x1B60),
                get_packet_size_server_to_client,
                reinterpret_cast<void**>(&g_get_packet_size_server_to_client));
  hooking::hook(reinterpret_cast<void*>(d2_net::get_base() + 0x1E60),
                get_packet_size_client_to_server,
                reinterpret_cast<void**>(&g_get_packet_size_client_to_server));

#ifndef NDEBUG
  // hooking::hook(reinterpret_cast<void*>(d2_net::get_base() + 0x1760),
  //               net_send_to_server,
  //               reinterpret_cast<void**>(&g_net_send_to_server));
  // hooking::hook(reinterpret_cast<void*>(d2_net::get_base() + 0x22B0),
  //               net_send_to_client,
  //               reinterpret_cast<void**>(&g_net_send_to_client));
#endif
}

bool common::get_packet_size_cs(packet_header* packet, size_t& size) {
  switch (packet->message_type) {
    case MESSAGE_TYPE_ITEM_MOVE: {
      size = sizeof item_move_cs;
      return true;
    }
    case MESSAGE_TYPE_INVENTORY_SORT: {
      size = sizeof inventory_sort_cs;
      return true;
    }
    case MESSAGE_TYPE_DAMAGE_INFO: {
      size = sizeof damage_info_cs;
      return true;
    }
    case MESSAGE_TYPE_GOLD_PICKUP_INFO: {
      size = sizeof gold_pickup_info_cs;
      return true;
    }
    case MESSAGE_TYPE_ITEM_PICKUP_INFO: {
      size = sizeof item_pickup_info_cs;
      return true;
    }
    case MESSAGE_TYPE_ITEM_DROPPED_INFO: {
      size = sizeof item_dropped_info_cs;
      return true;
    }
    case MESSAGE_TYPE_TRANSMUTE: {
      size = sizeof transmute_info_cs;
      return true;
    }
    case MESSAGE_TYPE_TRADER_UPDATE: {
      size = sizeof trader_update_cs;
      return true;
    }
    default:
      return false;
  }
}

bool common::get_packet_size_sc(packet_header* packet, size_t& size) {
  switch (packet->message_type) {
    case MESSAGE_TYPE_ITEM_MOVE: {
      size = sizeof item_move_sc;
      return true;
    }
    case MESSAGE_TYPE_INVENTORY_SORT: {
      size = sizeof inventory_sort_sc;
      return true;
    }
    case MESSAGE_TYPE_DAMAGE_INFO: {
      size = sizeof damage_info_sc;
      return true;
    }
    case MESSAGE_TYPE_GOLD_PICKUP_INFO: {
      size = sizeof gold_pickup_info_sc;
      return true;
    }
    case MESSAGE_TYPE_ITEM_PICKUP_INFO: {
      size = sizeof item_pickup_info_sc;
      return true;
    }
    case MESSAGE_TYPE_ITEM_DROPPED_INFO: {
      size = sizeof item_dropped_info_sc;
      return true;
    }
    case MESSAGE_TYPE_TRANSMUTE: {
      size = sizeof transmute_info_sc;
      return true;
    }
    case MESSAGE_TYPE_TRADER_UPDATE: {
      size = sizeof trader_update_sc;
      return true;
    }
    default:
      return false;
  }
}

}  // namespace common
}  // namespace d2_tweaks
