#pragma once

#include <cstdint>

#include "room.h"

namespace diablo2 {
namespace structures {

#pragma pack(push, 1)

struct skills;
struct Game;
struct Inventory;
struct stats;
struct Path;
struct PlayerData;
struct ItemData;
struct ObjectData;
struct MonsterData;
struct drlg_act;
struct skill_sequence;
struct anim_data;
struct unit_gfxdata;
struct statslistex;
struct quest_record;
struct NpcRecord;

enum D2C_ItemQualities {
  ITEMQUAL_INFERIOR = 0x01,  // 0x01 Inferior
  ITEMQUAL_NORMAL = 0x02,    // 0x02 Normal
  ITEMQUAL_SUPERIOR = 0x03,  // 0x03 Superior
  ITEMQUAL_MAGIC = 0x04,     // 0x04 Magic
  ITEMQUAL_SET = 0x05,       // 0x05 Set
  ITEMQUAL_RARE = 0x06,      // 0x06 Rare
  ITEMQUAL_UNIQUE = 0x07,    // 0x07 Unique
  ITEMQUAL_CRAFT = 0x08,     // 0x08 Crafted
  ITEMQUAL_TEMPERED = 0x09   // 0x09 Tempered
};

enum class unit_type_t : int32_t {
  UNIT_TYPE_PLAYER = 0,
  UNIT_TYPE_MONSTER = 1,
  UNIT_TYPE_OBJECT = 2,
  UNIT_TYPE_MISSILE = 3,
  UNIT_TYPE_ITEM = 4,
  UNIT_TYPE_VIS_TILE = 5  // unused?
};

enum player_class_t : int32_t {
  PLAYER_CLASS_AMAZON = 0,
  PLAYER_CLASS_SORCERESS = 1,
  PLAYER_CLASS_NECROMANCER = 2,
  PLAYER_CLASS_PALADIN = 3,
  PLAYER_CLASS_BARBARIAN = 4,
  PLAYER_CLASS_DRUID = 5,
  PLAYER_CLASS_ASSASSIN = 6
};

enum itemflags_t : int32_t {
  ITEMFLAG_NEWITEM = 0x00000001,
  ITEMFLAG_TARGET = 0x00000002,
  ITEMFLAG_TARGETING = 0x00000004,
  ITEMFLAG_DELETED = 0x00000008,
  ITEMFLAG_IDENTIFIED = 0x00000010,
  ITEMFLAG_QUANTITY = 0x00000020,
  ITEMFLAG_WEAPONSET_IN = 0x00000040,
  ITEMFLAG_WEAPONSET_OUT = 0x00000080,
  ITEMFLAG_BROKEN = 0x00000100,
  ITEMFLAG_REPAIRED = 0x00000200,
  ITEMFLAG_UNKNOWN2 = 0x00000400,
  ITEMFLAG_SOCKETED = 0x00000800,
  ITEMFLAG_NON_SELLABLE = 0x00001000,
  ITEMFLAG_INSTORE = 0x00002000,
  ITEMFLAG_NOEQUIP = 0x00004000,
  ITEMFLAG_NAMED = 0x00008000,
  ITEMFLAG_ORGAN = 0x00010000,
  ITEMFLAG_SELLCHEAP = 0x00020000,
  ITEMFLAG_UNK5 = 0x00040000,
  ITEMFLAG_INIT = 0x00080000,
  ITEMFLAG_UNK6 = 0x00100000,
  ITEMFLAG_COMPACTSAVE = 0x00200000,
  ITEMFLAG_ETHEREAL = 0x00400000,
  ITEMFLAG_JUSTSAVED = 0x00800000,
  ITEMFLAG_PERSONALIZED = 0x01000000,
  ITEMFLAG_LOWQUALITY = 0x02000000,
  ITEMFLAG_RUNEWORD = 0x04000000,
  ITEMFLAG_SHOPITEM = 0x06000000,
  ITEMFLAG_ITEM = 0x08000000
};

struct D2SeedStrc {
  union {
    struct {
      uint32_t nLowSeed;   // 0x00
      uint32_t nHighSeed;  // 0x04
    };
    uint64_t lSeed;  // 0x00
  };
};

struct D2CoordStrc  // sizeof 0x08
{
  int nX;  // 0x00
  int nY;  // 0x04
};

struct D2ItemDropStrc {
  Unit* pUnit;              // 0x00
  D2SeedStrc* pSeed;        // 0x04
  Game* pGame;              // 0x08
  int32_t nItemLvl;         // 0x0C
  uint32_t unk0x10;         // 0x10
  int32_t nId;              // 0x14
  int32_t nSpawnType;       // 0x18 [3 for ground spawn, 4 for inv spawn]
  int32_t nX;               // 0x1C
  int32_t nY;               // 0x20
  Room* pRoom;              // 0x24
  uint16_t wUnitInitFlags;  // 0x28
  uint16_t wItemFormat;     // 0x2A [ptGame0x0x78]
  BOOL bForce;              // 0x2C
  int32_t nQuality;         // 0x30
  int32_t nQuantity;        // 0x34
  int32_t nMinDur;          // 0x38
  int32_t nMaxDur;          // 0x3C
  int32_t nItemIndex;       // 0x40
  uint32_t dwFlags1;        // 0x44 - itemflag override (used when force is true)
  uint32_t dwSeed;          // 0x48 - overrides the seed, used when force is true
  uint32_t dwItemSeed;      // 0x4C - overrides the item seed when force is true
  int32_t eEarLvl;          // 0x50
  int32_t nQtyOverride;     // 0x54
  char szName[16];          // 0x58
  int32_t nPrefix[3];       // 0x68
  int32_t nSuffix[3];       // 0x74
  uint32_t dwFlags2;        // 0x80
};

struct Unit {
  unit_type_t type;

  union {
    player_class_t player_class;
    int32_t data_record_index;
  };

  void* memory_pool;
  uint32_t guid;
  uint32_t mode;

  union {
    PlayerData* player_data;
    MonsterData* monster_data;
    ObjectData* object_data;
    // missile_data* missile_data;
    ItemData* item_data;
  };

  int8_t act;
  int8_t act_padding[0x03];
  drlg_act* drlg_act;

  struct {
    uint32_t low_seed;
    uint32_t high_seed;
  } seed;
  uint32_t init_seed;

  Path* path;

  skill_sequence* skill_sequence;
  uint32_t skill_sequence_frame_count;
  uint32_t skill_sequence_frame;

  uint32_t anim_speed;
  uint32_t skill_sequence_mode;

  uint32_t current_frame;
  uint32_t frame_count;
  uint16_t anim_speed_w;

  uint8_t action_frame;
  uint8_t pad1;

  anim_data* anim_data;

  unit_gfxdata* gfxdata;
  unit_gfxdata* gfxdata_copy;

  statslistex* statslistex;
  Inventory* inventory;

  union {
    struct {
      uint32_t interact_guid;
      uint32_t interact_type;
      uint8_t interacting;
    } interaction;

    struct {
      void* light_map;
      uint32_t start_light_radius;
      uint16_t p12_shift_index;
    } lightning;
  };

  uint16_t update_type;
  Unit* update_unit;

  quest_record* quest_record;
  uint32_t sparky_chest;
  void* timer_args;

  union {
    Game* game;
    uint32_t sound_sync;
  };

  char pad2[0x0C];

  void* event;

  unit_type_t owner_type;
  uint32_t owner_guid;

  char pad3[8];

  char* hover_text;

  void* skills;

  void* combat;
  uint32_t hit_class;

  char pad4[4];

  uint32_t drop_code;

  char pad5[8];

  union {
    struct {
      uint32_t unit_flags;
      uint32_t unit_flags_ex;
    } flags;
    uint64_t flags64;
  };

  char pad6[4];

  uint32_t node_index;
  uint32_t get_tick_count;

  union {
    uint32_t get_tick_count2;
    void* particle_stream;
  };

  void* timer;

  Unit* change_next_unit;  //?
  Unit* prev_unit;
  Unit* prev_unit_in_room;

  void* msg_first;
  void* msg_last;

  bool is_hireling() const {
    if (type != unit_type_t::UNIT_TYPE_MONSTER)
      return false;

    return (flags.unit_flags & 0x00000200) == 0x00000200;
  }

  bool is_pet() const {
    if (type != unit_type_t::UNIT_TYPE_MONSTER)
      return false;

    return (flags.unit_flags & 0x80000000) == 0x80000000;
  }
};

#pragma pack(pop)

}  // namespace structures
}  // namespace diablo2
