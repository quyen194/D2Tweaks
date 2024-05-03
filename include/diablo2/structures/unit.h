#pragma once

#include <cstdint>

#pragma pack(push, 1)

namespace diablo2 {
	namespace structures {
		struct skills;
		struct game;
		struct inventory;
		struct stats;
		struct path;
		struct player_data;
		struct item_data;
		struct object_data;
		struct monster_data;
		struct drlg_act;
		struct skill_sequence;
		struct anim_data;
		struct unit_gfxdata;
		struct statslistex;
		struct quest_record;
		struct npc_record;

		enum class unit_type_t : int32_t {
			UNIT_TYPE_PLAYER = 0,
			UNIT_TYPE_MONSTER = 1,
			UNIT_TYPE_OBJECT = 2,
			UNIT_TYPE_MISSILE = 3,
			UNIT_TYPE_ITEM = 4,
			UNIT_TYPE_VIS_TILE = 5 //unused?
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

		struct unit {
			unit_type_t type;

			union {
				player_class_t player_class;
				int32_t data_record_index;
			};

			void* memory_pool;
			uint32_t guid;
			uint32_t mode;

			union {
				player_data* player_data;
				monster_data* monster_data;
				object_data* object_data;
				//missile_data* missile_data;
				item_data* item_data;
			};

			int8_t act;
			int8_t act_padding[0x03];
			drlg_act* drlg_act;

			struct {
				uint32_t low_seed;
				uint32_t high_seed;
			} seed;
			uint32_t init_seed;

			path* path;

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
			inventory* inventory;

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
			unit* update_unit;

			quest_record* quest_record;
			uint32_t sparky_chest;
			void* timer_args;

			union {
				game* game;
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

			unit* change_next_unit; //?
			unit* prev_unit;
			unit* prev_unit_in_room;

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
	}
}

#pragma pack(pop)