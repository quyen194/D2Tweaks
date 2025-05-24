#pragma once

#include <cstdint>
#include <functional>
#include <diablo2/structures/unit.h>
#include "d2common.h"

namespace diablo2 {
	namespace structures {
		struct game_server;
		struct net_client;
		struct game;
		struct unit;

		struct D2MonPropTxt
		{
			uint32_t dwId;							//0x00
			diablo2::structures::D2PropertyStrc props[3][6];				//0x04
			uint8_t nChance[3][6];					//0x124
			uint16_t pad0x136;						//0x136
		};

		#pragma pack(1)


		enum D2C_AiSpecialState : uint32_t
		{
			AISPECIALSTATE_NONE = 0,
			AISPECIALSTATE_NO_MON_STATS = 1,					// Set when classid > nMonStatsRecord
			AISPECIALSTATE_2 = 2,								// Unused
			AISPECIALSTATE_3 = 3,								// Unused
			AISPECIALSTATE_4 = 4,								// Unused
			AISPECIALSTATE_ROGUE_HIREABLE_NON_PLAYER_OWNER = 5,
			AISPECIALSTATE_HIREABLE_NON_PLAYER_OWNER = 6,
			AISPECIALSTATE_REVIVED = 7,							// as necromancer pet
			AISPECIALSTATE_8 = 8,								// Unused
			AISPECIALSTATE_9 = 9,								// Unused
			// Curses
			AISPECIALSTATE_DIMVISION = 10,
			AISPECIALSTATE_TERROR = 11,
			AISPECIALSTATE_TAUNT = 12,

			AISPECIALSTATE_COUNTESS = 13,
			AISPECIALSTATE_14 = 14,				// Unused
			AISPECIALSTATE_WHIPPED = 15,		// When whipped by overseer, for suicide minions
			AISPECIALSTATE_MOUNTING_UNIT = 16,	// When a unit is mounting another (tower / siege beast)
			AISPECIALSTATE_17 = 17,				// Unused

			AISPECIALSTATE_TABLE_COUNT = 18,
			AISPECIALSTATE_INVALID = 19,		// Could be for objects ?
		};


		struct D2SkillsTxt
		{
			int16_t nSkillId; 						//0x00
			uint16_t unk0x02;						//0x02
			uint32_t dwFlags[2];					//0x04 D2C_SkillsTxtFlags
			int8_t nCharClass;						//0x0C
			uint8_t unk0x0D[3];						//0x0D
			uint8_t nAnim;							//0x10
			uint8_t nMonAnim;						//0x11
			uint8_t nSeqTrans;						//0x12
			uint8_t nSeqNum;						//0x13
			uint8_t nRange;							//0x14
			uint8_t nSelectProc;					//0x15
			uint8_t nSeqInput;						//0x16
			uint8_t pad0x17;						//0x17
			int16_t nITypeA[3];						//0x18
			int16_t nITypeB[3];						//0x1E
			int16_t nETypeA[2];						//0x24
			int16_t nETypeB[2];						//0x28
			int16_t wSrvStartFunc;					//0x2C
			uint16_t wSrvDoFunc;					//0x2E
			uint16_t wSrvPrgFunc[3];				//0x30
			uint16_t pad0x36;						//0x36
			uint32_t dwPrgCalc[3];					//0x38
			uint8_t nPrgDamage;						//0x44
			uint8_t pad0x45;						//0x45
			int16_t wSrvMissile;					//0x46
			int16_t wSrvMissileA;					//0x48
			int16_t wSrvMissileB;					//0x4A
			int16_t wSrvMissileC;					//0x4C
			int16_t wSrvOverlay;					//0x4E
			uint32_t dwAuraFilter;					//0x50
			int16_t wAuraStat[6];					//0x54
			uint32_t dwAuraLenCalc;					//0x60
			uint32_t dwAuraRangeCalc;				//0x64
			int32_t dwAuraStatCalc[6];				//0x68
			int16_t nAuraState;						//0x80
			int16_t wAuraTargetState;				//0x82
			int16_t wAuraEvent[3];					//0x84
			int16_t wAuraEventFunc[3];				//0x8A
			uint16_t wAuraTgtEvent;					//0x90
			uint16_t wAuraTgtEventFunc;				//0x92
			int16_t nPassiveState;					//0x94
			int16_t nPassiveIType;					//0x96
			int16_t nPassiveStat[5];				//0x98
			uint16_t pad0xA2;						//0xA2
			uint32_t dwPassiveCalc[5];				//0xA4
			uint16_t wPassiveEvent;					//0xB8
			uint16_t wPassiveEventFunc;				//0xBA
			uint16_t wSummon;						//0xBC
			int8_t nPetType;						//0xBE
			int8_t nSumMode;						//0xBF
			uint32_t dwPetMax;						//0xC0
			uint16_t wSumSkill[5];					//0xC4
			uint16_t pad0xCE;						//0xCE
			uint32_t dwSumSkCalc[5];				//0xD0
			int16_t wSumUMod;						//0xE4
			int16_t wSumOverlay;					//0xE6
			uint16_t wCltMissile;					//0xE8
			uint16_t wCltMissileA;					//0xEA
			uint16_t wCltMissileB;					//0xEC
			uint16_t wCltMissileC;					//0xEE
			uint16_t wCltMissileD;					//0xF0
			uint16_t wCltStFunc;					//0xF2
			uint16_t wCltDoFunc;					//0xF4
			uint16_t wCltPrgFunc[3];				//0xF6
			uint16_t wStSound;						//0xFC
			uint16_t nStSoundClass;					//0x0FE
			uint16_t wDoSound;						//0x100
			uint16_t wDoSoundA;						//0x102
			uint16_t wDoSoundB;						//0x104
			uint16_t wCastOverlay;					//0x106
			uint16_t wTgtOverlay;					//0x108
			uint16_t wTgtSound;						//0x10A
			uint16_t wPrgOverlay;					//0x10C
			uint16_t wPrgSound;						//0x10E
			uint16_t wCltOverlayA;					//0x110
			uint16_t wCltOverlayB;					//0x112
			int32_t dwCltCalc[3];					//0x114
			uint8_t nItemTarget;					//0x120 D2C_SkillsTxtItemTarget
			uint8_t pad0x121;						//0x121
			uint16_t wItemCastSound;				//0x122
			uint16_t wItemCastOverlay;				//0x124
			uint16_t pad0x126;						//0x126
			uint32_t dwPerDelay;					//0x128
			uint16_t wMaxLvl;						//0x12C
			uint16_t wResultFlags;					//0x12E
			uint32_t dwHitFlags;					//0x130
			uint32_t dwHitClass;					//0x134
			uint32_t dwCalc[4];						//0x138
			int32_t dwParam[8];						//0x148
			uint8_t nWeapSel;						//0x168
			uint8_t pad0x169;						//0x169
			uint16_t wItemEffect;					//0x16A
			uint16_t wItemCltEffect;				//0x16C
			uint16_t pad0x16E;						//0x16E
			uint32_t dwSkPoints;					//0x170
			uint16_t wReqLevel;						//0x174
			uint16_t wReqStr;						//0x176
			uint16_t wReqDex;						//0x178
			uint16_t wReqInt;						//0x17A
			uint16_t wReqVit;						//0x17C
			int16_t nReqSkill[3];					//0x17E
			int16_t wStartMana;						//0x184
			uint16_t wMinMana;						//0x186
			uint8_t nManaShift;						//0x188
			uint8_t pad0x189;						//0x189
			int16_t wMana;							//0x18A
			int16_t wLevelMana;						//0x18C
			uint8_t nAttackRank;					//0x18E
			uint8_t nLineOfSight;					//0x18F D2C_SkillsTxtLineOfSight
			uint32_t dwDelay;						//0x190
			uint16_t wSkillDesc;					//0x194
			uint16_t pad0x196;						//0x196
			uint32_t dwToHit;						//0x198
			uint32_t dwLevToHit;					//0x19C
			uint32_t dwToHitCalc;					//0x1A0
			uint8_t nToHitShift;					//0x1A4
			uint8_t nSrcDam;						//0x1A5
			uint16_t pad0x1A6;						//0x1A6
			uint32_t dwMinDam;						//0x1A8
			uint32_t dwMaxDam;						//0x1AC
			uint32_t dwMinLvlDam[5];				//0x1B0
			uint32_t dwMaxLvlDam[5];				//0x1C4
			uint32_t dwDmgSymPerCalc;				//0x1D8
			uint8_t nEType;							//0x1DC
			uint8_t pad0x1DD[3];					//0x1DD
			uint32_t dwEMin;						//0x1E0
			uint32_t dwEMax;						//0x1E4
			uint32_t dwEMinLev[5];					//0x1E8
			uint32_t dwEMaxLev[5];					//0x1FC
			uint32_t dwEDmgSymPerCalc;				//0x210
			uint32_t dwELen;						//0x214
			uint32_t dwELevLen[3];					//0x218
			uint32_t dwELenSymPerCalc;				//0x224
			uint8_t nRestrict;						//0x228
			uint8_t pad0x229;						//0x229
			int16_t nState[3];						//0x22A
			uint8_t nAiType;						//0x230
			uint8_t pad0x231;						//0x231
			int16_t wAiBonus;						//0x232
			int32_t nCostMult;						//0x234
			int32_t nCostAdd;						//0x238
		};

		#pragma pack()

		struct D2SummonArgStrc
		{
			uint32_t dwFlags;						//0x00
			diablo2::structures::unit* pOwner;		//0x04
			int32_t nHcIdx;							//0x08
			D2C_AiSpecialState nAiSpecialState;		//0x0C
			int32_t nMonMode;						//0x10
			D2CoordStrc pPosition;					//0x14
			int32_t nPetType;						//0x1C
			int32_t nPetMax;						//0x20
		};

	}

	class d2_game {
	public:
		static char* get_base();

		static void enqueue_packet(structures::net_client* client, void* packet, size_t size);

		static uint32_t* get_game_id_array_begin();
		static uint32_t* get_game_id_array_end();

		static structures::game_server* get_game_server();
		static structures::game* get_game(structures::game_server* gs, uint32_t gameId);

		static structures::game* get_game_from_client_id(int32_t id);
		static structures::net_client* get_net_client_from_id(structures::game* game, int32_t id);
		static structures::net_client* diablo2::d2_game::get_net_client_from_id_2(structures::game* game, int32_t id);

		static structures::unit* get_player_pet(structures::game* game, structures::unit* unit, uint32_t type, uint32_t index);

		static structures::unit* get_server_unit(structures::game* game, diablo2::structures::unit_type_t type, uint32_t uniqueid);
		static structures::npc_record* diablo2::d2_game::get_npc_record(structures::game* game, structures::unit* npc, structures::unit** ptnpc);
		static void diablo2::d2_game::free_gamble(structures::game* game, structures::unit* player, structures::unit* npc, structures::npc_record* npcrecord);
		static void diablo2::d2_game::fill_gamble(structures::game* game, structures::unit* player, structures::unit* npc, structures::npc_record* npcrecord);
		static void diablo2::d2_game::create_vendor_cache1(structures::game* game, structures::unit* player, structures::unit* npc, uint32_t param, bool bGamble);
		static void diablo2::d2_game::create_vendor_cache2(structures::game* game, structures::unit* player, structures::unit* npc, uint32_t param, bool bGamble);

		static int32_t identify_item(structures::game* game, structures::unit* player, structures::unit* item);
		static int32_t pickup_gold_pile(structures::game* game, structures::unit* unit, structures::unit* item);
		//static int32_t pickup_item(structures::game* game, structures::unit* unit, structures::unit* item);
		static bool __fastcall pickup_item(structures::game* game, structures::unit* player, uint32_t guid, uint32_t* ptrItemCarried);
		static structures::unit* get_unit_owner(structures::game* game, structures::unit* unit);
		static void* iterate_unit_pets(structures::game* game, structures::unit* unit,
			const std::function<void(structures::game*, structures::unit*, structures::unit*)>& cb);

		static void update_inventory_items(structures::game* game, structures::unit* player);
		static uint32_t __fastcall diablo2::d2_game::transmogrify(diablo2::structures::game* game, diablo2::structures::unit* player);

		//D2Game.0x6FC4ED80
		static structures::unit* __fastcall D2GAME_CreateItemEx_6FC4ED80(structures::game* pGame, structures::D2ItemDropStrc* pItemDrop, int32_t a3);

		//D2Game.0x6FC4A660
		static int32_t __fastcall diablo2::d2_game::D2GAME_Transmogrify_6FC4A660(structures::game* pGame, structures::unit* pPlayer, structures::unit* pItem);

		static diablo2::structures::unit* __fastcall diablo2::d2_game::QUESTS_CreateItem(diablo2::structures::game* pGame, diablo2::structures::unit* pPlayer, uint32_t dwCode, int32_t nLevel, uint8_t nQuality, int32_t bDroppable);

		// Add wrapper for D2Game.0x6FC603D0
		// D2MonPropTxt* __fastcall MONSTER_GetMonPropTxtRecord(int32_t nId) 
		static diablo2::structures::D2MonPropTxt* __fastcall diablo2::d2_game::MONSTER_GetMonPropTxtRecord(int32_t nId);

		// add wrapper for //D2Game.0x6FCBC900
		// D2UnitStrc* __stdcall SUNIT_GetTargetUnit(D2GameStrc* pGame, D2UnitStrc* pUnit)
		static diablo2::structures::unit* __stdcall diablo2::d2_game::SUNIT_GetTargetUnit(structures::game* pGame, structures::unit* pUnit);

		//D2Game.0x6FCF5B90
		//D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId)
		static diablo2::structures::D2SkillsTxt* __fastcall diablo2::d2_game::SKILLS_GetSkillsTxtRecord(int32_t nSkillId);

		//D2Game.0x6FD15580
		//int32_t __fastcall D2GAME_GetSummonIdFromSkill_6FD15580(D2UnitStrc* pUnit, int32_t bFromMonster, int32_t nSkillId, int32_t nSkillLevel, int32_t* pSpawnMode, int32_t* pX, int32_t* pY)
		static int32_t __fastcall diablo2::d2_game::D2GAME_GetSummonIdFromSkill_6FD15580(structures::unit* pUnit, int32_t bFromMonster, int32_t nSkillId, int32_t nSkillLevel, int32_t* pSpawnMode, int32_t* pX, int32_t* pY);

		//D2Game.0x6FD14430
		//D2UnitStrc* __fastcall D2GAME_SummonPet_6FD14430(D2GameStrc* pGame, D2SummonArgStrc* pSummonArg)
		static diablo2::structures::unit* __fastcall diablo2::d2_game::D2GAME_SummonPet_6FD14430(structures::game* pGame, structures::D2SummonArgStrc* pSummonArg);

		//D2Game.0x6FD0CB10
		//int32_t __fastcall D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nPetLevelArg, int32_t nSkillLevel)
		static int32_t __fastcall diablo2::d2_game::D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(structures::game* pGame, structures::unit* pUnit, structures::unit* pPet, int32_t nPetLevelArg, int32_t nSkillLevel);

		//D2Game.0x6FD0C530
		//int32_t __fastcall D2GAME_SetSummonPassiveStats_6FD0C530(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nSkillId, int32_t nSkillLevel, int32_t nItemLevel)
		static int32_t __fastcall diablo2::d2_game::D2GAME_SetSummonPassiveStats_6FD0C530(structures::game* pGame, structures::unit* pUnit, structures::unit* pPet, int32_t nSkillId, int32_t nSkillLevel, int32_t nItemLevel);

		//D2Game.0x6FD0C2E0
		//void __fastcall D2GAME_SetSummonResistance_6FD0C2E0(D2UnitStrc* pUnit, D2UnitStrc* pPet)
		static void __fastcall diablo2::d2_game::D2GAME_SetSummonResistance_6FD0C2E0(structures::unit* pUnit, structures::unit* pPet);

		//D2Game.0x6FC3E200
		//void __fastcall sub_6FC3E200(D2ClientStrc* pClient, D2UnitStrc* pUnit)
		static void __fastcall diablo2::d2_game::sub_6FC3E200(structures::net_client* pClient, structures::unit* pUnit);

		//D2Game.0x6FC69F10
		//D2UnitStrc* __fastcall D2GAME_SpawnMonster_6FC69F10(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nAnimMode, int32_t a7, int16_t nFlags)
		static diablo2::structures::unit* __fastcall diablo2::d2_game::D2GAME_SpawnMonster_6FC69F10(structures::game* pGame, structures::room* pRoom, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nAnimMode, int32_t a7, int16_t nFlags);
	};
}
