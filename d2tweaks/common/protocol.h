#pragma once

#include <diablo2/structures/unit.h>

#include <cstdint>

using namespace diablo2::structures;

namespace d2_tweaks {
namespace common {

#pragma pack(push, 1)

#define MAX_MSG_SIZE = 0x204

enum packet_types_cs_t {
  PACKET_0x00,
  PACKET_0x01,
  PACKET_0x02,
  PACKET_0x03,
  PACKET_0x04,
  PACKET_0x05,
  PACKET_0x06,
  PACKET_0x07,
  PACKET_0x08,
  PACKET_0x09,
  PACKET_0x0A,
  PACKET_0x0B,
  PACKET_0x0C,
  PACKET_0x0D,
  PACKET_0x0E,
  PACKET_0x0F,
  PACKET_0x10,
  PACKET_0x11,
  PACKET_0x12,
  PACKET_0x13,
  PACKET_0x14,
  PACKET_0x15,
  PACKET_0x16,
  PACKET_0x17,
  PACKET_0x18,
  PACKET_0x19,
  PACKET_0x1A,
  PACKET_0x1B,
  PACKET_0x1C,
  PACKET_0x1D,
  PACKET_0x1E,
  PACKET_0x1F,
  PACKET_0x20,
  PACKET_0x21,
  PACKET_0x22,
  PACKET_0x23,
  PACKET_0x24,
  PACKET_0x25,
  PACKET_0x26,
  PACKET_0x27,
  PACKET_0x28,
  PACKET_0x29,
  PACKET_0x2A,
  PACKET_0x2B,
  PACKET_0x2C,
  PACKET_0x2D,
  PACKET_0x2E,
  PACKET_0x2F,
  PACKET_0x30,
  PACKET_0x31,
  PACKET_0x32,
  PACKET_0x33,
  PACKET_0x34,
  PACKET_0x35,
  PACKET_0x36,
  PACKET_0x37,
  PACKET_0x38,
  PACKET_0x39,
  PACKET_0x3A,
  PACKET_0x3B,
  PACKET_0x3C,
  PACKET_0x3D,
  PACKET_0x3E,
  PACKET_0x3F,
  PACKET_0x40,
  PACKET_0x41,
  PACKET_0x42,
  PACKET_0x43,
  PACKET_0x44,
  PACKET_0x45,
  PACKET_0x46,
  PACKET_0x47,
  PACKET_0x48,
  PACKET_0x49,
  PACKET_0x4A,
  PACKET_0x4B,
  PACKET_0x4C,
  PACKET_0x4D,
  PACKET_0x4E,
  PACKET_0x4F,
  PACKET_0x50,
  PACKET_0x51,
  PACKET_0x52,
  PACKET_0x53,
  PACKET_0x54,
  PACKET_0x55,
  PACKET_0x56,
  PACKET_0x57,
  PACKET_0x58,
  PACKET_0x59,
  PACKET_0x5A,
  PACKET_0x5B,
  PACKET_0x5C,
  PACKET_0x5D,
  PACKET_0x5E,
  PACKET_0x5F,
  PACKET_0x60,
  PACKET_0x61,
  PACKET_0x62,
  PACKET_0x63,
  PACKET_0x64,
  PACKET_0x65,
  PACKET_0x66,
  PACKET_0x67,
  PACKET_0x68,
  PACKET_0x69,
  PACKET_0x6A,
  PACKET_0x6B,
  PACKET_0x6C,
  PACKET_0x6D,
  PACKET_0x6E,
  PACKET_0x6F,
  PACKET_0x70,
  PACKET_0x71,
  PACKET_0x72,
  PACKET_0x73,
  PACKET_0x74,
  PACKET_0x75,
  PACKET_0x76,
  PACKET_0x77,
  PACKET_0x78,
  PACKET_0x79,
  PACKET_0x7A,
  PACKET_0x7B,
  PACKET_0x7C,
  PACKET_0x7D,
  PACKET_0x7E,
  PACKET_0x7F,
  PACKET_0x80,
  PACKET_0x81,
  PACKET_0x82,
  PACKET_0x83,
  PACKET_0x84,
  PACKET_0x85,
  PACKET_0x86,
  PACKET_0x87,
  PACKET_0x88,
  PACKET_0x89,
  PACKET_0x8A,
  PACKET_0x8B,
  PACKET_0x8C,
  PACKET_0x8D,
  PACKET_0x8E,
  PACKET_0x8F,
  PACKET_0x90,
  PACKET_0x91,
  PACKET_0x92,
  PACKET_0x93,
  PACKET_0x94,
  PACKET_0x95,
  PACKET_0x96,
  PACKET_0x97,
  PACKET_0x98,
  PACKET_0x99,
  PACKET_0x9A,
  PACKET_0x9B,
  PACKET_0x9C,
  PACKET_0x9D,
  PACKET_0x9E,
  PACKET_0x9F,
  PACKET_0xA0,
  PACKET_0xA1,
  PACKET_0xA2,
  PACKET_0xA3,
  PACKET_0xA4,
  PACKET_0xA5,
  PACKET_0xA6,
  PACKET_0xA7,
  PACKET_0xA8,
  PACKET_0xA9,
  PACKET_0xAA,
  PACKET_0xAB,
  PACKET_0xAC,
  PACKET_0xAD,
  PACKET_0xAE,
  PACKET_0xAF,
  PACKET_0xB0,
  PACKET_0xB1,
  PACKET_0xB2,
  PACKET_0xB3,
  PACKET_0xB4
};

enum message_types_t {
  MESSAGE_TYPE_ITEM_MOVE = 1,
  MESSAGE_TYPE_INVENTORY_SORT,
  MESSAGE_TYPE_DAMAGE_INFO,
  MESSAGE_TYPE_GOLD_PICKUP_INFO,
  MESSAGE_TYPE_ITEM_PICKUP_INFO,
  MESSAGE_TYPE_ITEM_DROPPED_INFO,
  MESSAGE_TYPE_TRANSMUTE,
  MESSAGE_TYPE_TRADER_UPDATE,
  MESSAGE_TYPE_INTERACT = 1,

  MESSAGE_TYPE_COUNT
};

enum damage_type_t : uint8_t {
  DAMAGE_TYPE_PHYSICAL = 0,

  DAMAGE_TYPE_COLD = 1,
  DAMAGE_TYPE_FIRE = 2,
  DAMAGE_TYPE_LIGHTNING = 3,
  DAMAGE_TYPE_POISON = 4,
  DAMAGE_TYPE_MAGIC = 5,

  DAMAGE_TYPE_COUNT,
  DAMAGE_TYPE_UNKNOWN = 0xFF
};

struct packet_header {
  uint8_t d2_packet_type;
  uint8_t message_type;

  packet_header() : d2_packet_type(0xBB), message_type(0) {}
};

struct d2_entity_action_cs : packet_header {
  uint32_t action;
  uint32_t entity_id;
  uint32_t complement;

  d2_entity_action_cs() : action(0), entity_id(0), complement(0) {}
};

struct item_move_cs : packet_header {
  uint32_t item_guid;
  const char* item_code;
  uint8_t target_page;
  uint32_t bag_guid = 0;
  bool updateBag;
  bool removeFromBag;
  bool summon;
  int prop;
  int val;
  int tmog;
  int x;
  int y;
  bool extract;
  uint32_t iCode;

  item_move_cs() : item_guid(0), target_page(0) {
    message_type = MESSAGE_TYPE_ITEM_MOVE;
  }
};

struct item_move_sc : packet_header {
  uint32_t item_guid;
  uint32_t tx;
  uint32_t ty;
  uint8_t target_page;
  uint32_t bag_guid = 0;
  bool updateBag;
  int prop;
  int val;
  item_move_sc() : item_guid(0), tx(0), ty(0), target_page(0) {
    message_type = MESSAGE_TYPE_ITEM_MOVE;
  }
};

struct inventory_sort_cs : packet_header {
  uint8_t page;
  unit* item_to_remove;
  bool remItem;

  inventory_sort_cs() : page(0) { message_type = MESSAGE_TYPE_INVENTORY_SORT; }
};

struct inventory_sort_sc : packet_header {
  uint8_t page;
  uint8_t tx;
  uint8_t ty;
  uint32_t guid;

  inventory_sort_sc() : page(0), tx(0), ty(0), guid(0) {
    message_type = MESSAGE_TYPE_INVENTORY_SORT;
  }
};

struct damage_info_cs : packet_header {
  uint8_t state;  // on or off

  damage_info_cs() : state(0) { message_type = MESSAGE_TYPE_DAMAGE_INFO; }
};

struct damage_info_sc : packet_header {
  uint8_t unit_type;
  uint32_t guid;
  damage_type_t damage_type;
  uint32_t damage;

  uint32_t currentHp;  // New field for current hit points
  uint32_t maxHp;      // New field for maximum hit points

  // You can add these details to the packet if needed
  // packet.isChampion = isChampion;
  // packet.isUnique = isUnique;
  // packet.isSuperUnique = isSuperUnique

  bool isChampion;
  bool isUnique;
  bool isSuperUnique;

  damage_info_sc()
      : unit_type(0),
        guid(0),
        damage_type(DAMAGE_TYPE_UNKNOWN),
        damage(0),
        currentHp(0),
        maxHp(0),
        isChampion(0),
        isUnique(0),
        isSuperUnique(0) {
    message_type = MESSAGE_TYPE_DAMAGE_INFO;
  }
};

struct gold_pickup_info_sc : packet_header {
  uint32_t gold;
  gold_pickup_info_sc() : gold(0) {
    message_type = MESSAGE_TYPE_GOLD_PICKUP_INFO;
  }
};

struct gold_pickup_info_cs : packet_header {
  uint32_t item_guid;
  gold_pickup_info_cs() : item_guid(0) {
    message_type = MESSAGE_TYPE_GOLD_PICKUP_INFO;
  }
};

struct item_pickup_info_cs : packet_header {
  uint32_t item_guid;
  item_pickup_info_cs() : item_guid(0) {
    message_type = MESSAGE_TYPE_ITEM_PICKUP_INFO;
  }
};

struct item_pickup_info_sc : packet_header {
  bool inventory_full;
  item_pickup_info_sc() : inventory_full(false) {
    message_type = MESSAGE_TYPE_ITEM_PICKUP_INFO;
  }
};

struct item_dropped_info_cs : packet_header {
  uint16_t item_id;
  uint8_t code[4];
  item_dropped_info_cs() : item_id(0), code{0} {
    message_type = MESSAGE_TYPE_ITEM_DROPPED_INFO;
  }
};

struct item_dropped_info_sc : packet_header {
  uint32_t item;
  uint8_t code[4];
  uint8_t quality;
  uint8_t showthis;
  uint8_t namestr[130];  // 130
  uint8_t index_arr_itemtype;
  uint8_t arr_itemtype_codestr_equivstr[20][5];
  item_dropped_info_sc()
      : item(0),
        quality(0),
        showthis(0),
        index_arr_itemtype(0),
        code{0},
        arr_itemtype_codestr_equivstr{0},
        namestr{0} {
    message_type = MESSAGE_TYPE_ITEM_DROPPED_INFO;
  }
};

struct transmute_info_sc : packet_header {
  uint32_t item_guid;
  uint32_t tx;
  uint32_t ty;
  uint8_t target_page;
  uint8_t command;

  transmute_info_sc() : item_guid(0), tx(0), ty(0), target_page(0), command(0) {
    message_type = MESSAGE_TYPE_TRANSMUTE;
  }
};

struct transmute_info_cs : packet_header {
  uint32_t item_guid;
  uint8_t target_page;
  uint8_t command;
  bool transmute_start_flag;

  transmute_info_cs()
      : item_guid(0), target_page(0), command(0), transmute_start_flag(0) {
    message_type = MESSAGE_TYPE_TRANSMUTE;
  }
};

struct trader_update_cs : packet_header {
  uint32_t npc_id;
  uint32_t client_id;
  uint8_t command;
  bool is_gamble_menu_open;
  trader_update_cs()
      : npc_id(0), client_id(0), command(0), is_gamble_menu_open(0) {
    message_type = MESSAGE_TYPE_TRADER_UPDATE;
  }
};

struct trader_update_sc : packet_header {
  uint32_t npc_id;
  uint32_t client_id;
  uint8_t command;
  bool is_gamble_menu_open;
  trader_update_sc()
      : npc_id(0), client_id(0), command(0), is_gamble_menu_open(0) {
    message_type = MESSAGE_TYPE_TRADER_UPDATE;
  }
};

#pragma pack(pop)

}  // namespace common
}  // namespace d2_tweaks
