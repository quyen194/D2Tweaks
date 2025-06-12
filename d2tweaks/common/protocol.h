#pragma once

#include "diablo2/structures/unit.h"

#include <cstdint>

using namespace diablo2::structures;

namespace d2_tweaks {
namespace common {

#pragma pack(push, 1)

#define MAX_MSG_SIZE = 0x204

namespace PacketTypeCS {
enum T {
  k00,
  k01,
  k02,
  k03,
  k04,
  k05,
  k06,
  k07,
  k08,
  k09,
  k0A,
  k0B,
  k0C,
  k0D,
  k0E,
  k0F,
  k10,
  k11,
  k12,
  k13,
  k14,
  k15,
  k16,
  k17,
  k18,
  k19,
  k1A,
  k1B,
  k1C,
  k1D,
  k1E,
  k1F,
  k20,
  k21,
  k22,
  k23,
  k24,
  k25,
  k26,
  k27,
  k28,
  k29,
  k2A,
  k2B,
  k2C,
  k2D,
  k2E,
  k2F,
  k30,
  k31,
  k32,
  k33,
  k34,
  k35,
  k36,
  k37,
  k38,
  k39,
  k3A,
  k3B,
  k3C,
  k3D,
  k3E,
  k3F,
  k40,
  k41,
  k42,
  k43,
  k44,
  k45,
  k46,
  k47,
  k48,
  k49,
  k4A,
  k4B,
  k4C,
  k4D,
  k4E,
  k4F,
  k50,
  k51,
  k52,
  k53,
  k54,
  k55,
  k56,
  k57,
  k58,
  k59,
  k5A,
  k5B,
  k5C,
  k5D,
  k5E,
  k5F,
  k60,
  k61,
  k62,
  k63,
  k64,
  k65,
  k66,
  k67,
  k68,
  k69,
  k6A,
  k6B,
  k6C,
  k6D,
  k6E,
  k6F,
  k70,
  k71,
  k72,
  k73,
  k74,
  k75,
  k76,
  k77,
  k78,
  k79,
  k7A,
  k7B,
  k7C,
  k7D,
  k7E,
  k7F,
  k80,
  k81,
  k82,
  k83,
  k84,
  k85,
  k86,
  k87,
  k88,
  k89,
  k8A,
  k8B,
  k8C,
  k8D,
  k8E,
  k8F,
  k90,
  k91,
  k92,
  k93,
  k94,
  k95,
  k96,
  k97,
  k98,
  k99,
  k9A,
  k9B,
  k9C,
  k9D,
  k9E,
  k9F,
  kA0,
  kA1,
  kA2,
  kA3,
  kA4,
  kA5,
  kA6,
  kA7,
  kA8,
  kA9,
  kAA,
  kAB,
  kAC,
  kAD,
  kAE,
  kAF,
  kB0,
  kB1,
  kB2,
  kB3,
  kB4
};
}  // namespace PacketTypeCs

namespace MessageType {
enum T {
  kItemMove = 1,
  kInventorySort,
  kDamageInfo,
  kGoldPickupInfo,
  kItemPickupInfo,
  kItemDroppedInfo,
  kTransmute,
  kTraderUpdate,
  // MessageType::kInteract = 1,

  kMaxValue,
};
}  // namespace MessageType

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
    message_type = MessageType::kItemMove;
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
    message_type = MessageType::kItemMove;
  }
};

struct inventory_sort_cs : packet_header {
  uint8_t page;
  Unit* item_to_remove;
  bool remItem;

  inventory_sort_cs() : page(0) { message_type = MessageType::kInventorySort; }
};

struct inventory_sort_sc : packet_header {
  uint8_t page;
  uint8_t tx;
  uint8_t ty;
  uint32_t guid;

  inventory_sort_sc() : page(0), tx(0), ty(0), guid(0) {
    message_type = MessageType::kInventorySort;
  }
};

struct damage_info_cs : packet_header {
  uint8_t state;  // on or off

  damage_info_cs() : state(0) { message_type = MessageType::kDamageInfo; }
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
    message_type = MessageType::kDamageInfo;
  }
};

struct gold_pickup_info_sc : packet_header {
  uint32_t gold;
  gold_pickup_info_sc() : gold(0) {
    message_type = MessageType::kGoldPickupInfo;
  }
};

struct gold_pickup_info_cs : packet_header {
  uint32_t item_guid;
  gold_pickup_info_cs() : item_guid(0) {
    message_type = MessageType::kGoldPickupInfo;
  }
};

struct item_pickup_info_cs : packet_header {
  uint32_t item_guid;
  item_pickup_info_cs() : item_guid(0) {
    message_type = MessageType::kItemPickupInfo;
  }
};

struct item_pickup_info_sc : packet_header {
  bool inventory_full;
  item_pickup_info_sc() : inventory_full(false) {
    message_type = MessageType::kItemPickupInfo;
  }
};

struct item_dropped_info_cs : packet_header {
  uint16_t item_id;
  uint8_t code[4];
  item_dropped_info_cs() : item_id(0), code{0} {
    message_type = MessageType::kItemDroppedInfo;
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
    message_type = MessageType::kItemDroppedInfo;
  }
};

struct transmute_info_sc : packet_header {
  uint32_t item_guid;
  uint32_t tx;
  uint32_t ty;
  uint8_t target_page;
  uint8_t command;

  transmute_info_sc() : item_guid(0), tx(0), ty(0), target_page(0), command(0) {
    message_type = MessageType::kTransmute;
  }
};

struct transmute_info_cs : packet_header {
  uint32_t item_guid;
  uint8_t target_page;
  uint8_t command;
  bool transmute_start_flag;

  transmute_info_cs()
      : item_guid(0), target_page(0), command(0), transmute_start_flag(0) {
    message_type = MessageType::kTransmute;
  }
};

struct trader_update_cs : packet_header {
  uint32_t npc_id;
  uint32_t client_id;
  uint8_t command;
  bool is_gamble_menu_open;
  trader_update_cs()
      : npc_id(0), client_id(0), command(0), is_gamble_menu_open(0) {
    message_type = MessageType::kTraderUpdate;
  }
};

struct trader_update_sc : packet_header {
  uint32_t npc_id;
  uint32_t client_id;
  uint8_t command;
  bool is_gamble_menu_open;
  trader_update_sc()
      : npc_id(0), client_id(0), command(0), is_gamble_menu_open(0) {
    message_type = MessageType::kTraderUpdate;
  }
};

#pragma pack(pop)

}  // namespace common
}  // namespace d2_tweaks
