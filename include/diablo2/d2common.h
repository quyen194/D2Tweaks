#pragma once

#include <Windows.h>
#include <cstdint>

//struct UniqueItemsBIN //size=0x14C (332)
//{
//	WORD	uniqueId;		//+00
//	BYTE	uk1[0x20];		//+02
//	WORD	uniqueNameId;	//+22
//	BYTE	uk2[0x08];		//+24
//	union {
//		BYTE	flag;		//+2C
//		struct {
//			BYTE ukf : 2;
//			BYTE carry1 : 1;
//			BYTE ladder : 1;
//		};
//	};
//	BYTE	uk3[0x11F];		//+2D
//};

//struct DataTables//01EE6A20 * 01FDA2D0 //second comments=1.11
//{
//	BYTE			uk1[0xA78];			//+000
//	DWORD* monStats;			//+A78 //1.11 and 1.10
//	BYTE			uk2[0x4];			//+A7C
//	DWORD			nbMonStats;			//+A80 //1.11 and 1.10
//	BYTE			uk3[0x108];			//+A84
//	DWORD* skilldesc;			//+B8C
//	BYTE			uk4[0x4];			//+B90
//	DWORD			nbSkilldesc;		//+B94
//	DWORD* skills;				//+B98
//	BYTE			uk5[0x4];			//+B9C
//	DWORD			nbSkills;			//+BA0
//	int* nbSkillsPerPlayer;	//+BA4
//	int				maxSkillsPerPlayer;	//+BA8
//	WORD* playerSkills;		//+BAC
//	BYTE			uk6[0x14];			//+BB0
//	DWORD* charStats;			//+BC4
//	DWORD			nbCharStats;		//+BC8
//	DWORD* itemStatCost;		//+BCC
//	BYTE			uk7[4];				//+BD0
//	DWORD			nbItemStatCosts;	//+BD4
//	WORD* statsDescPriority;	//+BD8
//	DWORD			nbStatsDescPriority;//+BDC
//	BYTE			uk8[0x18];			//+BE0
//	DWORD* itemTypes;			//+BF8
//	DWORD			nbItemTypes;		//+BFC
//	BYTE			uk9[0x0C];			//+C00
//	DWORD* sets;				//+C0C //1.11 and 1.10
//	DWORD			nbSets;				//+C10 //1.11 and 1.10
//	BYTE			uk9b[0x4];			//+C14
//	DWORD* setItems;			//+C18 //1.11 and 1.10
//	DWORD			nbSetItems;			//+C1C //1.11 and 1.10
//	BYTE			uk10[0x4];			//+C20
//	UniqueItemsBIN* uniqueItems;		//+C24 //1.11 and 1.10
//	DWORD			nbUniqueItems;		//+C28 //1.11 and 1.10
//	BYTE			uk11[0x2C];			//+C2C
//	DWORD* levels;				//+C58
//	DWORD			nbLevels;			//+C5C
//	BYTE			uk12[0x64];			//+C60
//	DWORD* cubemain;			//+CC4 //14C0 by 148 for 1.11
//	DWORD			nbCubemain;			//+CC8 //14C4 for 1.11
//	DWORD			nbInventory;		//+CCC
//	DWORD* inventory;			//+CD0
//	BYTE			uk13[0x04];			//+CD4
//	DWORD			nbItems;			//+CD8
//	DWORD* items;				//+CDC
//	DWORD* itemsBis;			//+CE0
//	BYTE			uk14[0x1F8];		//+CDC
//	DWORD			nbRunes;			//+EDC
//	DWORD* runes;				//+EE0
//};
//
//extern DataTables* sgptDataTables;

struct D2PropertyStrc
{
	int32_t nProperty;					//0x00
	int32_t nLayer;						//0x04
	int32_t nMin;						//0x08
	int32_t nMax;						//0x0C
};

namespace diablo2 {
	namespace structures {
		struct unit;
		struct path;
		struct inventory;
		struct room;

		struct items_line;
		struct item_types_line;

		struct D2OpStatDataStrc
		{
			uint16_t nOpBase;						//0x00
			uint16_t nStat;							//0x02
			uint8_t nOp;							//0x04
			uint8_t nOpParam;						//0x05
		};

		struct D2ItemStatCostTxt
		{
			uint16_t wStatId;						//0x00
			uint16_t pad0x02;						//0x02
			uint32_t dwItemStatFlags;				//0x04
			uint8_t nSendBits;						//0x08
			uint8_t nSendParamBits;					//0x09
			uint8_t nCsvBits;						//0x0A
			uint8_t nCsvParam;						//0x0B
			uint32_t dwDivide;						//0x0C
			uint32_t dwMultiply;					//0x10
			uint32_t dwAdd;							//0x14
			uint8_t nValShift;						//0x18
			uint8_t nSaveBits;						//0x19
			uint8_t n09SaveBits;					//0x1A
			uint8_t pad0x1B;						//0x1B
			uint32_t dwSaveAdd;						//0x1C
			uint32_t dw09SaveAdd;					//0x20
			uint32_t dwSaveParamBits;				//0x24
			uint32_t dw09SaveParamBits;				//0x28
			int32_t dwMinAccr;						//0x2C
			uint8_t nEncode;						//0x30
			uint8_t pad0x31;						//0x31
			uint16_t wMaxStat;						//0x32
			int16_t nDescPriority;					//0x34
			uint8_t nDescFunc;						//0x36
			uint8_t nDescVal;						//0x37
			uint16_t wDescStrPos;					//0x38
			uint16_t wDescStrNeg;					//0x3A
			uint16_t wDescStr2;						//0x3C
			uint16_t wDescGrp;						//0x3E
			uint8_t nDescGrpFunc;					//0x40
			uint8_t nDescGrpVal;					//0x41
			uint16_t wDescGrpStrPos;				//0x42
			uint16_t wDescGrpStrNeg;				//0x44
			uint16_t wDescGrpStr2;					//0x46
			int16_t wItemEvent[2];					//0x48
			uint16_t wItemEventFunc[2];				//0x4C
			uint8_t nKeepZero;						//0x50
			uint8_t bIsBaseOfOtherStatOp;			//0x51
			uint8_t bHasOpStatData;					//0x52
			uint8_t bHasOpApplyingToItem;			//0x53
			uint8_t nOp;							//0x54
			uint8_t nOpParam;						//0x55
			uint16_t wOpBase;						//0x56
			uint16_t wOpStat[3];					//0x58
			uint16_t unk0x5E[64];					//0x5E - also related to op stats (see DATATBLS_LoadItemStatCostTxt)
			D2OpStatDataStrc pOpStatData[16];		//0xDE
			uint16_t pad0x13E;						//0x13E
			uint32_t dwStuff;						//0x140
		};
	}

	enum unit_stats_t {
		UNIT_STAT_STRENGTH = 0x0,
		UNIT_STAT_ENERGY = 0x1,
		UNIT_STAT_DEXTERITY = 0x2,
		UNIT_STAT_VITALITY = 0x3,
		UNIT_STAT_STATPTS = 0x4,
		UNIT_STAT_NEWSKILLS = 0x5,
		UNIT_STAT_HITPOINTS = 0x6,
		UNIT_STAT_MAXHP = 0x7,
		UNIT_STAT_MANA = 0x8,
		UNIT_STAT_MAXMANA = 0x9,
		UNIT_STAT_STAMINA = 0xA,
		UNIT_STAT_MAXSTAMINA = 0xB,
		UNIT_STAT_LEVEL = 0xC,
		UNIT_STAT_EXPERIENCE = 0xD,
		UNIT_STAT_GOLD = 0xE,
		UNIT_STAT_GOLDBANK = 0xF,
		UNIT_STAT_ITEM_ARMOR_PERCENT = 0x10,
		UNIT_STAT_ITEM_MAXDAMAGE_PERCENT = 0x11,
		UNIT_STAT_ITEM_MINDAMAGE_PERCENT = 0x12,
		UNIT_STAT_TOHIT = 0x13,
		UNIT_STAT_TOBLOCK = 0x14,
		UNIT_STAT_MINDAMAGE = 0x15,
		UNIT_STAT_MAXDAMAGE = 0x16,
		UNIT_STAT_SECONDARY_MINDAMAGE = 0x17,
		UNIT_STAT_SECONDARY_MAXDAMAGE = 0x18,
		UNIT_STAT_DAMAGEPERCENT = 0x19,
		UNIT_STAT_MANARECOVERY = 0x1A,
		UNIT_STAT_MANARECOVERYBONUS = 0x1B,
		UNIT_STAT_STAMINARECOVERYBONUS = 0x1C,
		UNIT_STAT_LASTEXP = 0x1D,
		UNIT_STAT_NEXTEXP = 0x1E,
		UNIT_STAT_ARMORCLASS = 0x1F,
		UNIT_STAT_ARMORCLASS_VS_MISSILE = 0x20,
		UNIT_STAT_ARMORCLASS_VS_HTH = 0x21,
		UNIT_STAT_NORMAL_DAMAGE_REDUCTION = 0x22,
		UNIT_STAT_MAGIC_DAMAGE_REDUCTION = 0x23,
		UNIT_STAT_DAMAGERESIST = 0x24,
		UNIT_STAT_MAGICRESIST = 0x25,
		UNIT_STAT_MAXMAGICRESIST = 0x26,
		UNIT_STAT_FIRERESIST = 0x27,
		UNIT_STAT_MAXFIRERESIST = 0x28,
		UNIT_STAT_LIGHTRESIST = 0x29,
		UNIT_STAT_MAXLIGHTRESIST = 0x2A,
		UNIT_STAT_COLDRESIST = 0x2B,
		UNIT_STAT_MAXCOLDRESIST = 0x2C,
		UNIT_STAT_POISONRESIST = 0x2D,
		UNIT_STAT_MAXPOISONRESIST = 0x2E,
		UNIT_STAT_DAMAGEAURA = 0x2F,
		UNIT_STAT_FIREMINDAM = 0x30,
		UNIT_STAT_FIREMAXDAM = 0x31,
		UNIT_STAT_LIGHTMINDAM = 0x32,
		UNIT_STAT_LIGHTMAXDAM = 0x33,
		UNIT_STAT_MAGICMINDAM = 0x34,
		UNIT_STAT_MAGICMAXDAM = 0x35,
		UNIT_STAT_COLDMINDAM = 0x36,
		UNIT_STAT_COLDMAXDAM = 0x37,
		UNIT_STAT_COLDLENGTH = 0x38,
		UNIT_STAT_POISONMINDAM = 0x39,
		UNIT_STAT_POISONMAXDAM = 0x3A,
		UNIT_STAT_POISONLENGTH = 0x3B,
		UNIT_STAT_LIFEDRAINMINDAM = 0x3C,
		UNIT_STAT_LIFEDRAINMAXDAM = 0x3D,
		UNIT_STAT_MANADRAINMINDAM = 0x3E,
		UNIT_STAT_MANADRAINMAXDAM = 0x3F,
		UNIT_STAT_STAMDRAINMINDAM = 0x40,
		UNIT_STAT_STAMDRAINMAXDAM = 0x41,
		UNIT_STAT_STUNLENGTH = 0x42,
		UNIT_STAT_VELOCITYPERCENT = 0x43,
		UNIT_STAT_ATTACKRATE = 0x44,
		UNIT_STAT_OTHER_ANIMRATE = 0x45,
		UNIT_STAT_QUANTITY = 0x46,
		UNIT_STAT_VALUE = 0x47,
		UNIT_STAT_DURABILITY = 0x48,
		UNIT_STAT_MAXDURABILITY = 0x49,
		UNIT_STAT_HPREGEN = 0x4A,
		UNIT_STAT_ITEM_MAXDURABILITY_PERCENT = 0x4B,
		UNIT_STAT_ITEM_MAXHP_PERCENT = 0x4C,
		UNIT_STAT_ITEM_MAXMANA_PERCENT = 0x4D,
		UNIT_STAT_ITEM_ATTACKERTAKESDAMAGE = 0x4E,
		UNIT_STAT_ITEM_GOLDBONUS = 0x4F,
		UNIT_STAT_ITEM_MAGICBONUS = 0x50,
		UNIT_STAT_ITEM_KNOCKBACK = 0x51,
		UNIT_STAT_ITEM_TIMEDURATION = 0x52,
		UNIT_STAT_ITEM_ADDCLASSSKILLS = 0x53,
		UNIT_STAT_UNSENTPARAM1 = 0x54,
		UNIT_STAT_ITEM_ADDEXPERIENCE = 0x55,
		UNIT_STAT_ITEM_HEALAFTERKILL = 0x56,
		UNIT_STAT_ITEM_REDUCEDPRICES = 0x57,
		UNIT_STAT_ITEM_DOUBLEHERBDURATION = 0x58,
		UNIT_STAT_ITEM_LIGHTRADIUS = 0x59,
		UNIT_STAT_ITEM_LIGHTCOLOR = 0x5A,
		UNIT_STAT_ITEM_REQ_PERCENT = 0x5B,
		UNIT_STAT_ITEM_LEVELREQ = 0x5C,
		UNIT_STAT_ITEM_FASTERATTACKRATE = 0x5D,
		UNIT_STAT_ITEM_LEVELREQPCT = 0x5E,
		UNIT_STAT_LASTBLOCKFRAME = 0x5F,
		UNIT_STAT_ITEM_FASTERMOVEVELOCITY = 0x60,
		UNIT_STAT_ITEM_NONCLASSSKILL = 0x61,
		UNIT_STAT_STATE = 0x62,
		UNIT_STAT_ITEM_FASTERGETHITRATE = 0x63,
		UNIT_STAT_MONSTER_PLAYERCOUNT = 0x64,
		UNIT_STAT_SKILL_POISON_OVERRIDE_LENGTH = 0x65,
		UNIT_STAT_ITEM_FASTERBLOCKRATE = 0x66,
		UNIT_STAT_SKILL_BYPASS_UNDEAD = 0x67,
		UNIT_STAT_SKILL_BYPASS_DEMONS = 0x68,
		UNIT_STAT_ITEM_FASTERCASTRATE = 0x69,
		UNIT_STAT_SKILL_BYPASS_BEASTS = 0x6A,
		UNIT_STAT_ITEM_SINGLESKILL = 0x6B,
		UNIT_STAT_ITEM_RESTINPEACE = 0x6C,
		UNIT_STAT_CURSE_RESISTANCE = 0x6D,
		UNIT_STAT_ITEM_POISONLENGTHRESIST = 0x6E,
		UNIT_STAT_ITEM_NORMALDAMAGE = 0x6F,
		UNIT_STAT_ITEM_HOWL = 0x70,
		UNIT_STAT_ITEM_STUPIDITY = 0x71,
		UNIT_STAT_ITEM_DAMAGETOMANA = 0x72,
		UNIT_STAT_ITEM_IGNORETARGETAC = 0x73,
		UNIT_STAT_ITEM_FRACTIONALTARGETAC = 0x74,
		UNIT_STAT_ITEM_PREVENTHEAL = 0x75,
		UNIT_STAT_ITEM_HALFFREEZEDURATION = 0x76,
		UNIT_STAT_ITEM_TOHIT_PERCENT = 0x77,
		UNIT_STAT_ITEM_DAMAGETARGETAC = 0x78,
		UNIT_STAT_ITEM_DEMONDAMAGE_PERCENT = 0x79,
		UNIT_STAT_ITEM_UNDEADDAMAGE_PERCENT = 0x7A,
		UNIT_STAT_ITEM_DEMON_TOHIT = 0x7B,
		UNIT_STAT_ITEM_UNDEAD_TOHIT = 0x7C,
		UNIT_STAT_ITEM_THROWABLE = 0x7D,
		UNIT_STAT_ITEM_ELEMSKILL = 0x7E,
		UNIT_STAT_ITEM_ALLSKILLS = 0x7F,
		UNIT_STAT_ITEM_ATTACKERTAKESLIGHTDAMAGE = 0x80,
		UNIT_STAT_IRONMAIDEN_LEVEL = 0x81,
		UNIT_STAT_LIFETAP_LEVEL = 0x82,
		UNIT_STAT_THORNS_PERCENT = 0x83,
		UNIT_STAT_BONEARMOR = 0x84,
		UNIT_STAT_BONEARMORMAX = 0x85,
		UNIT_STAT_ITEM_FREEZE = 0x86,
		UNIT_STAT_ITEM_OPENWOUNDS = 0x87,
		UNIT_STAT_ITEM_CRUSHINGBLOW = 0x88,
		UNIT_STAT_ITEM_KICKDAMAGE = 0x89,
		UNIT_STAT_ITEM_MANAAFTERKILL = 0x8A,
		UNIT_STAT_ITEM_HEALAFTERDEMONKILL = 0x8B,
		UNIT_STAT_ITEM_EXTRABLOOD = 0x8C,
		UNIT_STAT_ITEM_DEADLYSTRIKE = 0x8D,
		UNIT_STAT_ITEM_ABSORBFIRE_PERCENT = 0x8E,
		UNIT_STAT_ITEM_ABSORBFIRE = 0x8F,
		UNIT_STAT_ITEM_ABSORBLIGHT_PERCENT = 0x90,
		UNIT_STAT_ITEM_ABSORBLIGHT = 0x91,
		UNIT_STAT_ITEM_ABSORBMAGIC_PERCENT = 0x92,
		UNIT_STAT_ITEM_ABSORBMAGIC = 0x93,
		UNIT_STAT_ITEM_ABSORBCOLD_PERCENT = 0x94,
		UNIT_STAT_ITEM_ABSORBCOLD = 0x95,
		UNIT_STAT_ITEM_SLOW = 0x96,
		UNIT_STAT_ITEM_AURA = 0x97,
		UNIT_STAT_ITEM_INDESCTRUCTIBLE = 0x98,
		UNIT_STAT_ITEM_CANNOTBEFROZEN = 0x99,
		UNIT_STAT_ITEM_STAMINADRAINPCT = 0x9A,
		UNIT_STAT_ITEM_REANIMATE = 0x9B,
		UNIT_STAT_ITEM_PIERCE = 0x9C,
		UNIT_STAT_ITEM_MAGICARROW = 0x9D,
		UNIT_STAT_ITEM_EXPLOSIVEARROW = 0x9E,
		UNIT_STAT_ITEM_THROW_MINDAMAGE = 0x9F,
		UNIT_STAT_ITEM_THROW_MAXDAMAGE = 0xA0,
		UNIT_STAT_SKILL_HANDOFATHENA = 0xA1,
		UNIT_STAT_SKILL_STAMINAPERCENT = 0xA2,
		UNIT_STAT_SKILL_PASSIVE_STAMINAPERCENT = 0xA3,
		UNIT_STAT_SKILL_CONCENTRATION = 0xA4,
		UNIT_STAT_SKILL_ENCHANT = 0xA5,
		UNIT_STAT_SKILL_PIERCE = 0xA6,
		UNIT_STAT_SKILL_CONVICTION = 0xA7,
		UNIT_STAT_SKILL_CHILLINGARMOR = 0xA8,
		UNIT_STAT_SKILL_FRENZY = 0xA9,
		UNIT_STAT_SKILL_DECREPIFY = 0xAA,
		UNIT_STAT_SKILL_ARMOR_PERCENT = 0xAB,
		UNIT_STAT_ALIGNMENT = 0xAC,
		UNIT_STAT_TARGET0 = 0xAD,
		UNIT_STAT_TARGET1 = 0xAE,
		UNIT_STAT_GOLDLOST = 0xAF,
		UNIT_STAT_CONVERSION_LEVEL = 0xB0,
		UNIT_STAT_CONVERSION_MAXHP = 0xB1,
		UNIT_STAT_UNIT_DOOVERLAY = 0xB2,
		UNIT_STAT_ATTACK_VS_MONTYPE = 0xB3,
		UNIT_STAT_DAMAGE_VS_MONTYPE = 0xB4,
		UNIT_STAT_FADE = 0xB5,
		UNIT_STAT_ARMOR_OVERRIDE_PERCENT = 0xB6,
		UNIT_STAT_UNUSED183 = 0xB7,
		UNIT_STAT_UNUSED184 = 0xB8,
		UNIT_STAT_UNUSED185 = 0xB9,
		UNIT_STAT_UNUSED186 = 0xBA,
		UNIT_STAT_UNUSED187 = 0xBB,
		UNIT_STAT_ITEM_ADDSKILL_TAB = 0xBC,
		UNIT_STAT_UNUSED189 = 0xBD,
		UNIT_STAT_UNUSED190 = 0xBE,
		UNIT_STAT_UNUSED191 = 0xBF,
		UNIT_STAT_UNUSED192 = 0xC0,
		UNIT_STAT_UNUSED193 = 0xC1,
		UNIT_STAT_ITEM_NUMSOCKETS = 0xC2,
		UNIT_STAT_ITEM_SKILLONATTACK = 0xC3,
		UNIT_STAT_ITEM_SKILLONKILL = 0xC4,
		UNIT_STAT_ITEM_SKILLONDEATH = 0xC5,
		UNIT_STAT_ITEM_SKILLONHIT = 0xC6,
		UNIT_STAT_ITEM_SKILLONLEVELUP = 0xC7,
		UNIT_STAT_UNUSED200 = 0xC8,
		UNIT_STAT_ITEM_SKILLONGETHIT = 0xC9,
		UNIT_STAT_UNUSED202 = 0xCA,
		UNIT_STAT_UNUSED203 = 0xCB,
		UNIT_STAT_ITEM_CHARGED_SKILL = 0xCC,
		UNIT_STAT_UNUSED204 = 0xCD,
		UNIT_STAT_UNUSED205 = 0xCE,
		UNIT_STAT_UNUSED206 = 0xCF,
		UNIT_STAT_UNUSED207 = 0xD0,
		UNIT_STAT_UNUSED208 = 0xD1,
		UNIT_STAT_UNUSED209 = 0xD2,
		UNIT_STAT_UNUSED210 = 0xD3,
		UNIT_STAT_UNUSED211 = 0xD4,
		UNIT_STAT_UNUSED212 = 0xD5,
		UNIT_STAT_ITEM_ARMOR_PERLEVEL = 0xD6,
		UNIT_STAT_ITEM_ARMORPERCENT_PERLEVEL = 0xD7,
		UNIT_STAT_ITEM_HP_PERLEVEL = 0xD8,
		UNIT_STAT_ITEM_MANA_PERLEVEL = 0xD9,
		UNIT_STAT_ITEM_MAXDAMAGE_PERLEVEL = 0xDA,
		UNIT_STAT_ITEM_MAXDAMAGE_PERCENT_PERLEVEL = 0xDB,
		UNIT_STAT_ITEM_STRENGTH_PERLEVEL = 0xDC,
		UNIT_STAT_ITEM_DEXTERITY_PERLEVEL = 0xDD,
		UNIT_STAT_ITEM_ENERGY_PERLEVEL = 0xDE,
		UNIT_STAT_ITEM_VITALITY_PERLEVEL = 0xDF,
		UNIT_STAT_ITEM_TOHIT_PERLEVEL = 0xE0,
		UNIT_STAT_ITEM_TOHITPERCENT_PERLEVEL = 0xE1,
		UNIT_STAT_ITEM_COLD_DAMAGEMAX_PERLEVEL = 0xE2,
		UNIT_STAT_ITEM_FIRE_DAMAGEMAX_PERLEVEL = 0xE3,
		UNIT_STAT_ITEM_LTNG_DAMAGEMAX_PERLEVEL = 0xE4,
		UNIT_STAT_ITEM_POIS_DAMAGEMAX_PERLEVEL = 0xE5,
		UNIT_STAT_ITEM_RESIST_COLD_PERLEVEL = 0xE6,
		UNIT_STAT_ITEM_RESIST_FIRE_PERLEVEL = 0xE7,
		UNIT_STAT_ITEM_RESIST_LTNG_PERLEVEL = 0xE8,
		UNIT_STAT_ITEM_RESIST_POIS_PERLEVEL = 0xE9,
		UNIT_STAT_ITEM_ABSORB_COLD_PERLEVEL = 0xEA,
		UNIT_STAT_ITEM_ABSORB_FIRE_PERLEVEL = 0xEB,
		UNIT_STAT_ITEM_ABSORB_LTNG_PERLEVEL = 0xEC,
		UNIT_STAT_ITEM_ABSORB_POIS_PERLEVEL = 0xED,
		UNIT_STAT_ITEM_THORNS_PERLEVEL = 0xEE,
		UNIT_STAT_ITEM_FIND_GOLD_PERLEVEL = 0xEF,
		UNIT_STAT_ITEM_FIND_MAGIC_PERLEVEL = 0xF0,
		UNIT_STAT_ITEM_REGENSTAMINA_PERLEVEL = 0xF1,
		UNIT_STAT_ITEM_STAMINA_PERLEVEL = 0xF2,
		UNIT_STAT_ITEM_DAMAGE_DEMON_PERLEVEL = 0xF3,
		UNIT_STAT_ITEM_DAMAGE_UNDEAD_PERLEVEL = 0xF4,
		UNIT_STAT_ITEM_TOHIT_DEMON_PERLEVEL = 0xF5,
		UNIT_STAT_ITEM_TOHIT_UNDEAD_PERLEVEL = 0xF6,
		UNIT_STAT_ITEM_CRUSHINGBLOW_PERLEVEL = 0xF7,
		UNIT_STAT_ITEM_OPENWOUNDS_PERLEVEL = 0xF8,
		UNIT_STAT_ITEM_KICK_DAMAGE_PERLEVEL = 0xF9,
		UNIT_STAT_ITEM_DEADLYSTRIKE_PERLEVEL = 0xFA,
		UNIT_STAT_ITEM_FIND_GEMS_PERLEVEL = 0xFB,
		UNIT_STAT_ITEM_REPLENISH_DURABILITY = 0xFC,
		UNIT_STAT_ITEM_REPLENISH_QUANTITY = 0xFD,
		UNIT_STAT_ITEM_EXTRA_STACK = 0xFE,
		UNIT_STAT_ITEM_FIND_ITEM = 0xFF,
		UNIT_STAT_ITEM_SLASH_DAMAGE = 0x100,
		UNIT_STAT_ITEM_SLASH_DAMAGE_PERCENT = 0x101,
		UNIT_STAT_ITEM_CRUSH_DAMAGE = 0x102,
		UNIT_STAT_ITEM_CRUSH_DAMAGE_PERCENT = 0x103,
		UNIT_STAT_ITEM_THRUST_DAMAGE = 0x104,
		UNIT_STAT_ITEM_THRUST_DAMAGE_PERCENT = 0x105,
		UNIT_STAT_ITEM_ABSORB_SLASH = 0x106,
		UNIT_STAT_ITEM_ABSORB_CRUSH = 0x107,
		UNIT_STAT_ITEM_ABSORB_THRUST = 0x108,
		UNIT_STAT_ITEM_ABSORB_SLASH_PERCENT = 0x109,
		UNIT_STAT_ITEM_ABSORB_CRUSH_PERCENT = 0x10A,
		UNIT_STAT_ITEM_ABSORB_THRUST_PERCENT = 0x10B,
		UNIT_STAT_ITEM_ARMOR_BYTIME = 0x10C,
		UNIT_STAT_ITEM_ARMORPERCENT_BYTIME = 0x10D,
		UNIT_STAT_ITEM_HP_BYTIME = 0x10E,
		UNIT_STAT_ITEM_MANA_BYTIME = 0x10F,
		UNIT_STAT_ITEM_MAXDAMAGE_BYTIME = 0x110,
		UNIT_STAT_ITEM_MAXDAMAGE_PERCENT_BYTIME = 0x111,
		UNIT_STAT_ITEM_STRENGTH_BYTIME = 0x112,
		UNIT_STAT_ITEM_DEXTERITY_BYTIME = 0x113,
		UNIT_STAT_ITEM_ENERGY_BYTIME = 0x114,
		UNIT_STAT_ITEM_VITALITY_BYTIME = 0x115,
		UNIT_STAT_ITEM_TOHIT_BYTIME = 0x116,
		UNIT_STAT_ITEM_TOHITPERCENT_BYTIME = 0x117,
		UNIT_STAT_ITEM_COLD_DAMAGEMAX_BYTIME = 0x118,
		UNIT_STAT_ITEM_FIRE_DAMAGEMAX_BYTIME = 0x119,
		UNIT_STAT_ITEM_LTNG_DAMAGEMAX_BYTIME = 0x11A,
		UNIT_STAT_ITEM_POIS_DAMAGEMAX_BYTIME = 0x11B,
		UNIT_STAT_ITEM_RESIST_COLD_BYTIME = 0x11C,
		UNIT_STAT_ITEM_RESIST_FIRE_BYTIME = 0x11D,
		UNIT_STAT_ITEM_RESIST_LTNG_BYTIME = 0x11E,
		UNIT_STAT_ITEM_RESIST_POIS_BYTIME = 0x11F,
		UNIT_STAT_ITEM_ABSORB_COLD_BYTIME = 0x120,
		UNIT_STAT_ITEM_ABSORB_FIRE_BYTIME = 0x121,
		UNIT_STAT_ITEM_ABSORB_LTNG_BYTIME = 0x122,
		UNIT_STAT_ITEM_ABSORB_POIS_BYTIME = 0x123,
		UNIT_STAT_ITEM_FIND_GOLD_BYTIME = 0x124,
		UNIT_STAT_ITEM_FIND_MAGIC_BYTIME = 0x125,
		UNIT_STAT_ITEM_REGENSTAMINA_BYTIME = 0x126,
		UNIT_STAT_ITEM_STAMINA_BYTIME = 0x127,
		UNIT_STAT_ITEM_DAMAGE_DEMON_BYTIME = 0x128,
		UNIT_STAT_ITEM_DAMAGE_UNDEAD_BYTIME = 0x129,
		UNIT_STAT_ITEM_TOHIT_DEMON_BYTIME = 0x12A,
		UNIT_STAT_ITEM_TOHIT_UNDEAD_BYTIME = 0x12B,
		UNIT_STAT_ITEM_CRUSHINGBLOW_BYTIME = 0x12C,

		// unit_stats_t from Ironman mod

		UNIT_STAT_strength = 0,
		UNIT_STAT_energy = 1,
		UNIT_STAT_dexterity = 2,
		UNIT_STAT_vitality = 3,
		UNIT_STAT_statpts = 4,
		UNIT_STAT_newskills = 5,
		UNIT_STAT_hitpoints = 6,
		UNIT_STAT_maxhp = 7,
		UNIT_STAT_mana = 8,
		UNIT_STAT_maxmana = 9,
		UNIT_STAT_stamina = 10,
		UNIT_STAT_maxstamina = 11,
		UNIT_STAT_level = 12,
		UNIT_STAT_experience = 13,
		UNIT_STAT_gold = 14,
		UNIT_STAT_goldbank = 15,
		UNIT_STAT_item_armor_percent = 16,
		UNIT_STAT_item_maxdamage_percent = 17,
		UNIT_STAT_item_mindamage_percent = 18,
		UNIT_STAT_tohit = 19,
		UNIT_STAT_toblock = 20,
		UNIT_STAT_mindamage = 21,
		UNIT_STAT_maxdamage = 22,
		UNIT_STAT_secondary_mindamage = 23,
		UNIT_STAT_secondary_maxdamage = 24,
		UNIT_STAT_damagepercent = 25,
		UNIT_STAT_manarecovery = 26,
		UNIT_STAT_manarecoverybonus = 27,
		UNIT_STAT_staminarecoverybonus = 28,
		UNIT_STAT_lastexp = 29,
		UNIT_STAT_nextexp = 30,
		UNIT_STAT_armorclass = 31,
		UNIT_STAT_armorclass_vs_missile = 32,
		UNIT_STAT_armorclass_vs_hth = 33,
		UNIT_STAT_normal_damage_reduction = 34,
		UNIT_STAT_magic_damage_reduction = 35,
		UNIT_STAT_damageresist = 36,
		UNIT_STAT_magicresist = 37,
		UNIT_STAT_maxmagicresist = 38,
		UNIT_STAT_fireresist = 39,
		UNIT_STAT_maxfireresist = 40,
		UNIT_STAT_lightresist = 41,
		UNIT_STAT_maxlightresist = 42,
		UNIT_STAT_coldresist = 43,
		UNIT_STAT_maxcoldresist = 44,
		UNIT_STAT_poisonresist = 45,
		UNIT_STAT_maxpoisonresist = 46,
		UNIT_STAT_damageaura = 47,
		UNIT_STAT_firemindam = 48,
		UNIT_STAT_firemaxdam = 49,
		UNIT_STAT_lightmindam = 50,
		UNIT_STAT_lightmaxdam = 51,
		UNIT_STAT_magicmindam = 52,
		UNIT_STAT_magicmaxdam = 53,
		UNIT_STAT_coldmindam = 54,
		UNIT_STAT_coldmaxdam = 55,
		UNIT_STAT_coldlength = 56,
		UNIT_STAT_poisonmindam = 57,
		UNIT_STAT_poisonmaxdam = 58,
		UNIT_STAT_poisonlength = 59,
		UNIT_STAT_lifedrainmindam = 60,
		UNIT_STAT_lifedrainmaxdam = 61,
		UNIT_STAT_manadrainmindam = 62,
		UNIT_STAT_manadrainmaxdam = 63,
		UNIT_STAT_stamdrainmindam = 64,
		UNIT_STAT_stamdrainmaxdam = 65,
		UNIT_STAT_stunlength = 66,
		UNIT_STAT_velocitypercent = 67,
		UNIT_STAT_attackrate = 68,
		UNIT_STAT_other_animrate = 69,
		UNIT_STAT_quantity = 70,
		UNIT_STAT_value = 71,
		UNIT_STAT_durability = 72,
		UNIT_STAT_maxdurability = 73,
		UNIT_STAT_hpregen = 74,
		UNIT_STAT_item_maxdurability_percent = 75,
		UNIT_STAT_item_maxhp_percent = 76,
		UNIT_STAT_item_maxmana_percent = 77,
		UNIT_STAT_item_attackertakesdamage = 78,
		UNIT_STAT_item_goldbonus = 79,
		UNIT_STAT_item_magicbonus = 80,
		UNIT_STAT_item_knockback = 81,
		UNIT_STAT_item_timeduration = 82,
		UNIT_STAT_item_addclassskills = 83,
		UNIT_STAT_unsentparam1 = 84,
		UNIT_STAT_item_addexperience = 85,
		UNIT_STAT_item_healafterkill = 86,
		UNIT_STAT_item_reducedprices = 87,
		UNIT_STAT_item_doubleherbduration = 88,
		UNIT_STAT_item_lightradius = 89,
		UNIT_STAT_item_lightcolor = 90,
		UNIT_STAT_item_req_percent = 91,
		UNIT_STAT_item_levelreq = 92,
		UNIT_STAT_item_fasterattackrate = 93,
		UNIT_STAT_item_levelreqpct = 94,
		UNIT_STAT_lastblockframe = 95,
		UNIT_STAT_item_fastermovevelocity = 96,
		UNIT_STAT_item_nonclassskill = 97,
		UNIT_STAT_state = 98,
		UNIT_STAT_item_fastergethitrate = 99,
		UNIT_STAT_monster_playercount = 100,
		UNIT_STAT_skill_poison_override_length = 101,
		UNIT_STAT_item_fasterblockrate = 102,
		UNIT_STAT_skill_bypass_undead = 103,
		UNIT_STAT_skill_bypass_demons = 104,
		UNIT_STAT_item_fastercastrate = 105,
		UNIT_STAT_skill_bypass_beasts = 106,
		UNIT_STAT_item_singleskill = 107,
		UNIT_STAT_item_restinpeace = 108,
		UNIT_STAT_curse_resistance = 109,
		UNIT_STAT_item_poisonlengthresist = 110,
		UNIT_STAT_item_normaldamage = 111,
		UNIT_STAT_item_howl = 112,
		UNIT_STAT_item_stupidity = 113,
		UNIT_STAT_item_damagetomana = 114,
		UNIT_STAT_item_ignoretargetac = 115,
		UNIT_STAT_item_fractionaltargetac = 116,
		UNIT_STAT_item_preventheal = 117,
		UNIT_STAT_item_halffreezeduration = 118,
		UNIT_STAT_item_tohit_percent = 119,
		UNIT_STAT_item_damagetargetac = 120,
		UNIT_STAT_item_demondamage_percent = 121,
		UNIT_STAT_item_undeaddamage_percent = 122,
		UNIT_STAT_item_demon_tohit = 123,
		UNIT_STAT_item_undead_tohit = 124,
		UNIT_STAT_item_throwable = 125,
		UNIT_STAT_item_elemskill = 126,
		UNIT_STAT_item_allskills = 127,
		UNIT_STAT_item_attackertakeslightdamage = 128,
		UNIT_STAT_ironmaiden_level = 129,
		UNIT_STAT_lifetap_level = 130,
		UNIT_STAT_thorns_percent = 131,
		UNIT_STAT_bonearmor = 132,
		UNIT_STAT_bonearmormax = 133,
		UNIT_STAT_item_freeze = 134,
		UNIT_STAT_item_openwounds = 135,
		UNIT_STAT_item_crushingblow = 136,
		UNIT_STAT_item_kickdamage = 137,
		UNIT_STAT_item_manaafterkill = 138,
		UNIT_STAT_item_healafterdemonkill = 139,
		UNIT_STAT_item_extrablood = 140,
		UNIT_STAT_item_deadlystrike = 141,
		UNIT_STAT_item_absorbfire_percent = 142,
		UNIT_STAT_item_absorbfire = 143,
		UNIT_STAT_item_absorblight_percent = 144,
		UNIT_STAT_item_absorblight = 145,
		UNIT_STAT_item_absorbmagic_percent = 146,
		UNIT_STAT_item_absorbmagic = 147,
		UNIT_STAT_item_absorbcold_percent = 148,
		UNIT_STAT_item_absorbcold = 149,
		UNIT_STAT_item_slow = 150,
		UNIT_STAT_item_aura = 151,
		UNIT_STAT_item_indesctructible = 152,
		UNIT_STAT_item_cannotbefrozen = 153,
		UNIT_STAT_item_staminadrainpct = 154,
		UNIT_STAT_item_reanimate = 155,
		UNIT_STAT_item_pierce = 156,
		UNIT_STAT_item_magicarrow = 157,
		UNIT_STAT_item_explosivearrow = 158,
		UNIT_STAT_item_throw_mindamage = 159,
		UNIT_STAT_item_throw_maxdamage = 160,
		UNIT_STAT_skill_handofathena = 161,
		UNIT_STAT_skill_staminapercent = 162,
		UNIT_STAT_skill_passive_staminapercent = 163,
		UNIT_STAT_skill_concentration = 164,
		UNIT_STAT_skill_enchant = 165,
		UNIT_STAT_skill_pierce = 166,
		UNIT_STAT_skill_conviction = 167,
		UNIT_STAT_skill_chillingarmor = 168,
		UNIT_STAT_skill_frenzy = 169,
		UNIT_STAT_skill_decrepify = 170,
		UNIT_STAT_skill_armor_percent = 171,
		UNIT_STAT_alignment = 172,
		UNIT_STAT_target0 = 173,
		UNIT_STAT_target1 = 174,
		UNIT_STAT_goldlost = 175,
		UNIT_STAT_conversion_level = 176,
		UNIT_STAT_conversion_maxhp = 177,
		UNIT_STAT_unit_dooverlay = 178,
		UNIT_STAT_attack_vs_montype = 179,
		UNIT_STAT_damage_vs_montype = 180,
		UNIT_STAT_fade = 181,
		UNIT_STAT_armor_override_percent = 182,
		UNIT_STAT_killcounter = 183,
		UNIT_STAT_soulscaptured = 184,
		UNIT_STAT_spirits = 185,
		UNIT_STAT_skill_more = 186,
		UNIT_STAT_item_corrupted = 187,
		UNIT_STAT_item_addskill_tab = 188,
		UNIT_STAT_hidden_corruption = 189,
		UNIT_STAT_item_strength_spirits = 190,
		UNIT_STAT_item_dexterity_spirits = 191,
		UNIT_STAT_item_vitality_spirits = 192,
		UNIT_STAT_item_energy_spirits = 193,
		UNIT_STAT_item_numsockets = 194,
		UNIT_STAT_item_skillonattack = 195,
		UNIT_STAT_item_skillonkill = 196,
		UNIT_STAT_item_skillondeath = 197,
		UNIT_STAT_item_skillonhit = 198,
		UNIT_STAT_item_skillonlevelup = 199,
		UNIT_STAT_item_skill_souls = 200,
		UNIT_STAT_item_skillongethit = 201,
		UNIT_STAT_UNUSED_2 = 202,
		UNIT_STAT_UNUSED_3 = 203,
		UNIT_STAT_item_charged_skill = 204,
		UNIT_STAT_UNUSED_5 = 205,
		UNIT_STAT_UNUSED_6 = 206,
		UNIT_STAT_UNUSED_7 = 207,
		UNIT_STAT_UNUSED_8 = 208,
		UNIT_STAT_UNUSED_9 = 209,
		UNIT_STAT_UNUSED_10 = 210,
		UNIT_STAT_UNUSED_11 = 211,
		UNIT_STAT_UNUSED_12 = 212,
		UNIT_STAT_UNUSED_13 = 213,
		UNIT_STAT_item_armor_perlevel = 214,
		UNIT_STAT_item_armorpercent_perlevel = 215,
		UNIT_STAT_item_hp_perlevel = 216,
		UNIT_STAT_item_mana_perlevel = 217,
		UNIT_STAT_item_maxdamage_perlevel = 218,
		UNIT_STAT_item_maxdamage_percent_perlevel = 219,
		UNIT_STAT_item_strength_perlevel = 220,
		UNIT_STAT_item_dexterity_perlevel = 221,
		UNIT_STAT_item_energy_perlevel = 222,
		UNIT_STAT_item_vitality_perlevel = 223,
		UNIT_STAT_item_tohit_perlevel = 224,
		UNIT_STAT_item_tohitpercent_perlevel = 225,
		UNIT_STAT_item_cold_damagemax_perlevel = 226,
		UNIT_STAT_item_fire_damagemax_perlevel = 227,
		UNIT_STAT_item_ltng_damagemax_perlevel = 228,
		UNIT_STAT_item_pois_damagemax_perlevel = 229,
		UNIT_STAT_item_resist_cold_perlevel = 230,
		UNIT_STAT_item_resist_fire_perlevel = 231,
		UNIT_STAT_item_resist_ltng_perlevel = 232,
		UNIT_STAT_item_resist_pois_perlevel = 233,
		UNIT_STAT_item_absorb_cold_perlevel = 234,
		UNIT_STAT_item_absorb_fire_perlevel = 235,
		UNIT_STAT_item_absorb_ltng_perlevel = 236,
		UNIT_STAT_item_absorb_pois_perlevel = 237,
		UNIT_STAT_item_thorns_perlevel = 238,
		UNIT_STAT_UNUSED_84 = 239,
		UNIT_STAT_item_find_magic_perlevel = 240,
		UNIT_STAT_item_regenstamina_perlevel = 241,
		UNIT_STAT_item_stamina_perlevel = 242,
		UNIT_STAT_item_damage_demon_perlevel = 243,
		UNIT_STAT_item_damage_undead_perlevel = 244,
		UNIT_STAT_item_tohit_demon_perlevel = 245,
		UNIT_STAT_item_tohit_undead_perlevel = 246,
		UNIT_STAT_item_crushingblow_perlevel = 247,
		UNIT_STAT_item_openwounds_perlevel = 248,
		UNIT_STAT_item_kick_damage_perlevel = 249,
		UNIT_STAT_item_deadlystrike_perlevel = 250,
		UNIT_STAT_item_find_gems_perlevel = 251,
		UNIT_STAT_item_replenish_durability = 252,
		UNIT_STAT_item_replenish_quantity = 253,
		UNIT_STAT_item_extra_stack = 254,
		UNIT_STAT_item_find_item = 255,
		UNIT_STAT_item_slash_damage = 256,
		UNIT_STAT_item_slash_damage_percent = 257,
		UNIT_STAT_item_crush_damage = 258,
		UNIT_STAT_item_crush_damage_percent = 259,
		UNIT_STAT_item_thrust_damage = 260,
		UNIT_STAT_item_thrust_damage_percent = 261,
		UNIT_STAT_item_absorb_slash = 262,
		UNIT_STAT_item_absorb_crush = 263,
		UNIT_STAT_item_absorb_thrust = 264,
		UNIT_STAT_item_absorb_slash_percent = 265,
		UNIT_STAT_item_absorb_crush_percent = 266,
		UNIT_STAT_item_absorb_thrust_percent = 267,
		UNIT_STAT_UNUSED_14 = 268,
		UNIT_STAT_UNUSED_15 = 269,
		UNIT_STAT_UNUSED_16 = 270,
		UNIT_STAT_UNUSED_17 = 271,
		UNIT_STAT_UNUSED_18 = 272,
		UNIT_STAT_UNUSED_19 = 273,
		UNIT_STAT_UNUSED_20 = 274,
		UNIT_STAT_UNUSED_21 = 275,
		UNIT_STAT_UNUSED_22 = 276,
		UNIT_STAT_UNUSED_23 = 277,
		UNIT_STAT_UNUSED_24 = 278,
		UNIT_STAT_UNUSED_25 = 279,
		UNIT_STAT_UNUSED_26 = 280,
		UNIT_STAT_UNUSED_27 = 281,
		UNIT_STAT_UNUSED_28 = 282,
		UNIT_STAT_UNUSED_29 = 283,
		UNIT_STAT_UNUSED_30 = 284,
		UNIT_STAT_UNUSED_31 = 285,
		UNIT_STAT_UNUSED_32 = 286,
		UNIT_STAT_UNUSED_33 = 287,
		UNIT_STAT_UNUSED_34 = 288,
		UNIT_STAT_UNUSED_35 = 289,
		UNIT_STAT_UNUSED_36 = 290,
		UNIT_STAT_UNUSED_37 = 291,
		UNIT_STAT_UNUSED_38 = 292,
		UNIT_STAT_UNUSED_39 = 293,
		UNIT_STAT_UNUSED_40 = 294,
		UNIT_STAT_UNUSED_41 = 295,
		UNIT_STAT_UNUSED_42 = 296,
		UNIT_STAT_UNUSED_43 = 297,
		UNIT_STAT_UNUSED_44 = 298,
		UNIT_STAT_display_spirits = 299,
		UNIT_STAT_display_souls_captured = 300,
		UNIT_STAT_magharv = 301,
		UNIT_STAT_dummy = 302,
		UNIT_STAT_display_kills = 303,
		UNIT_STAT_iforge = 304,
		UNIT_STAT_item_pierce_cold = 305,
		UNIT_STAT_item_pierce_fire = 306,
		UNIT_STAT_item_pierce_ltng = 307,
		UNIT_STAT_item_pierce_pois = 308,
		UNIT_STAT_item_damage_vs_monster = 309,
		UNIT_STAT_item_damage_percent_vs_monster = 310,
		UNIT_STAT_item_tohit_vs_monster = 311,
		UNIT_STAT_item_tohit_percent_vs_monster = 312,
		UNIT_STAT_item_ac_vs_monster = 313,
		UNIT_STAT_item_ac_percent_vs_monster = 314,
		UNIT_STAT_firelength = 315,
		UNIT_STAT_burningmin = 316,
		UNIT_STAT_burningmax = 317,
		UNIT_STAT_progressive_damage = 318,
		UNIT_STAT_progressive_steal = 319,
		UNIT_STAT_progressive_other = 320,
		UNIT_STAT_progressive_fire = 321,
		UNIT_STAT_progressive_cold = 322,
		UNIT_STAT_progressive_lightning = 323,
		UNIT_STAT_item_extra_charges = 324,
		UNIT_STAT_progressive_tohit = 325,
		UNIT_STAT_poison_count = 326,
		UNIT_STAT_damage_framerate = 327,
		UNIT_STAT_pierce_idx = 328,
		UNIT_STAT_passive_fire_mastery = 329,
		UNIT_STAT_passive_ltng_mastery = 330,
		UNIT_STAT_passive_cold_mastery = 331,
		UNIT_STAT_passive_pois_mastery = 332,
		UNIT_STAT_passive_fire_pierce = 333,
		UNIT_STAT_passive_ltng_pierce = 334,
		UNIT_STAT_passive_cold_pierce = 335,
		UNIT_STAT_passive_pois_pierce = 336,
		UNIT_STAT_passive_critical_strike = 337,
		UNIT_STAT_passive_dodge = 338,
		UNIT_STAT_passive_avoid = 339,
		UNIT_STAT_passive_evade = 340,
		UNIT_STAT_passive_warmth = 341,
		UNIT_STAT_passive_mastery_melee_th = 342,
		UNIT_STAT_passive_mastery_melee_dmg = 343,
		UNIT_STAT_passive_mastery_melee_crit = 344,
		UNIT_STAT_passive_mastery_throw_th = 345,
		UNIT_STAT_passive_mastery_throw_dmg = 346,
		UNIT_STAT_passive_mastery_throw_crit = 347,
		UNIT_STAT_passive_weaponblock = 348,
		UNIT_STAT_passive_summon_resist = 349,
		UNIT_STAT_modifierlist_skill = 350,
		UNIT_STAT_modifierlist_level = 351,
		UNIT_STAT_last_sent_hp_pct = 352,
		UNIT_STAT_source_unit_type = 353,
		UNIT_STAT_source_unit_id = 354,
		UNIT_STAT_shortparam1 = 355,
		UNIT_STAT_questitemdifficulty = 356,
		UNIT_STAT_passive_mag_mastery = 357,
		UNIT_STAT_passive_mag_pierce = 358,
		UNIT_STAT_item_strength_percent = 359,
		UNIT_STAT_item_dexterity_percent = 360,
		UNIT_STAT_item_vitality_percent = 361,
		UNIT_STAT_item_energy_percent = 362,
		UNIT_STAT_item_strengthpercent_perlevel = 363,
		UNIT_STAT_item_dexteritypercent_perlevel = 364,
		UNIT_STAT_item_energypercent_perlevel = 365,
		UNIT_STAT_item_vitalitypercent_perlevel = 366,
		UNIT_STAT_item_attackergetsblind = 367,
		UNIT_STAT_item_attackerflees = 368,
		UNIT_STAT_item_attackertakesfiredamage = 369,
		UNIT_STAT_item_attackertakescolddamage = 370,
		UNIT_STAT_item_maxdamage_perstr = 371,
		UNIT_STAT_item_maxdamage_perdex = 372,
		UNIT_STAT_item_mindamage_perlvl = 373,
		UNIT_STAT_item_mindamage_perstr = 374,
		UNIT_STAT_item_mindamage_perdex = 375,
		UNIT_STAT_item_maxdamage_percent_perstr = 376,
		UNIT_STAT_item_maxdamage_percent_perdex = 377,
		UNIT_STAT_item_openwounds_perdex = 378,
		UNIT_STAT_item_openwounds_perstr = 379,
		UNIT_STAT_item_deadlystrike_perdex = 380,
		UNIT_STAT_item_deadlystrike_perstr = 381,
		UNIT_STAT_item_armor_perstr = 382,
		UNIT_STAT_item_armor_perdex = 383,
		UNIT_STAT_item_tohit_perstr = 384,
		UNIT_STAT_item_tohit_perDex = 385,
		UNIT_STAT_item_hp_pervitality = 386,
		UNIT_STAT_item_mana_perenr = 387,
		UNIT_STAT_item_fastercastrate_perenr = 388,
		UNIT_STAT_item_fasterblockrate_perdex = 389,
		UNIT_STAT_item_fastermovevelocity_pervitality = 390,
		UNIT_STAT_item_fasterswingvelocity_perstrength = 391,
		UNIT_STAT_item_mindamage_percent_perstr = 392,
		UNIT_STAT_item_mindamage_percent_perdex = 393,
		UNIT_STAT_item_mindamage_percent_perlvl = 394,
		UNIT_STAT_item_crushingblow_perstr = 395,
		UNIT_STAT_item_crushingblow_perdex = 396,
		UNIT_STAT_item_elemskillcold = 397,
		UNIT_STAT_item_elemskilllight = 398,
		UNIT_STAT_item_elemskillpoison = 399,
		UNIT_STAT_item_elemskillmagic = 400,
		UNIT_STAT_item_elemskillfire = 401,
		UNIT_STAT_item_armorpercent_perstr = 402,
		UNIT_STAT_item_armorpercent_perdex = 403,
		UNIT_STAT_item_tohitpercent_perstr = 404,
		UNIT_STAT_item_tohitpercent_perdex = 405,
		UNIT_STAT_item_fasterswingvelocity_perdex = 406,
		UNIT_STAT_item_fasterswingvelocity_perlvl = 407,
		UNIT_STAT_item_fasterblockrate_perstr = 408,
		UNIT_STAT_item_fasterblockrate_perlvl = 409,
		UNIT_STAT_item_fastermovevelocity_perdex = 410,
		UNIT_STAT_item_fastermovevelocity_perlvl = 411,
		UNIT_STAT_item_fastercastrate_perlvl = 412,
		UNIT_STAT_item_fasterhitrecovery_pervit = 413,
		UNIT_STAT_item_fasterhitrecovery_perlvl = 414,
		UNIT_STAT_item_increasedblock_perdex = 415,
		UNIT_STAT_item_increasedblock_perlvl = 416,
		UNIT_STAT_item_addexperience_perlvl = 417,
		UNIT_STAT_item_addexperience_perenr = 418,
		UNIT_STAT_item_reducedprices_perlvl = 419,
		UNIT_STAT_item_pierce_perlvl = 420,
		UNIT_STAT_item_pierce_perstr = 421,
		UNIT_STAT_resmax_display_all_zero = 422,
		UNIT_STAT_maxmagres_percent = 423,
		UNIT_STAT_maxfireres_percent = 424,
		UNIT_STAT_maxlightres_percent = 425,
		UNIT_STAT_maxcoldres_percent = 426,
		UNIT_STAT_maxpoisres_percent = 427,
		UNIT_STAT_magicresist_hidden = 428,
		UNIT_STAT_maxmagicresist_hidden = 429,
		UNIT_STAT_fireresist_hidden = 430,
		UNIT_STAT_maxfireresist_hidden = 431,
		UNIT_STAT_lightresist_hidden = 432,
		UNIT_STAT_maxlightresist_hidden = 433,
		UNIT_STAT_coldresist_hidden = 434,
		UNIT_STAT_maxcoldresist_hidden = 435,
		UNIT_STAT_poisonresist_hidden = 436,
		UNIT_STAT_maxpoisonresist_hidden = 437,
		UNIT_STAT_hiddenresist_dummy = 438,
		UNIT_STAT_item_slow_perlvl = 439,
		UNIT_STAT_item_kick_damage_perstr = 440,
		UNIT_STAT_item_kick_damage_perdex = 441,
		UNIT_STAT_damageresist_perlvl = 442,
		UNIT_STAT_damageresist_pervit = 443,
		UNIT_STAT_magic_damage_reduction_perlvl = 444,
		UNIT_STAT_magic_damage_reduction_perenr = 445,
		UNIT_STAT_magicresist_perlvl = 446,
		UNIT_STAT_magicresist_perenr = 447,
		UNIT_STAT_item_stupidity_perlvl = 448,
		UNIT_STAT_item_freeze_perlvl = 449,
		UNIT_STAT_item_freeze_perenr = 450,
		UNIT_STAT_item_poisonlengthresist_perlvl = 451,
		UNIT_STAT_item_poisonlengthresist_pervit = 452,
		UNIT_STAT_curse_resistance_perlvl = 453,
		UNIT_STAT_curse_resistance_perenr = 454,
		UNIT_STAT_curse_resistance_length = 455,
		UNIT_STAT_item_manaafterkill_perlvl = 456,
		UNIT_STAT_item_manaafterkill_perenr = 457,
		UNIT_STAT_item_damagetomana_perlvl = 458,
		UNIT_STAT_item_damagetomana_perenr = 459,
		UNIT_STAT_item_damagetomana_permana = 460,
		UNIT_STAT_UNUSED_45 = 461,
		UNIT_STAT_UNUSED_46 = 462,
		UNIT_STAT_UNUSED_47 = 463,
		UNIT_STAT_UNUSED_48 = 464,
		UNIT_STAT_UNUSED_49 = 465,
		UNIT_STAT_UNUSED_50 = 466,
		UNIT_STAT_UNUSED_51 = 467,
		UNIT_STAT_UNUSED_52 = 468,
		UNIT_STAT_UNUSED_53 = 469,
		UNIT_STAT_UNUSED_54 = 470,
		UNIT_STAT_UNUSED_55 = 471,
		UNIT_STAT_UNUSED_56 = 472,
		UNIT_STAT_UNUSED_57 = 473,
		UNIT_STAT_UNUSED_58 = 474,
		UNIT_STAT_UNUSED_59 = 475,
		UNIT_STAT_UNUSED_60 = 476,
		UNIT_STAT_UNUSED_61 = 477,
		UNIT_STAT_UNUSED_62 = 478,
		UNIT_STAT_UNUSED_63 = 479,
		UNIT_STAT_UNUSED_64 = 480,
		UNIT_STAT_UNUSED_65 = 481,
		UNIT_STAT_UNUSED_66 = 482,
		UNIT_STAT_UNUSED_67 = 483,
		UNIT_STAT_UNUSED_68 = 484,
		UNIT_STAT_UNUSED_69 = 485,
		UNIT_STAT_UNUSED_70 = 486,
		UNIT_STAT_gembag_Stones_Flourite = 487,
		UNIT_STAT_gembag_Stones_Jade = 488,
		UNIT_STAT_gembag_Stones_Argonite = 489,
		UNIT_STAT_gembag_Stones_Azurite = 490,
		UNIT_STAT_gembag_Stones_Sulpher = 491,
		UNIT_STAT_gembag_Stones_Quartz = 492,
		UNIT_STAT_gembag_Stones_TigerEye = 493,
		UNIT_STAT_BoH_Desc = 494,
		UNIT_STAT_runebag_RunesE = 495,
		UNIT_STAT_runebag_RunesF = 496,
		UNIT_STAT_passive_sum_mastery = 497,
		UNIT_STAT_item_socketmultiplier = 498,
		UNIT_STAT_gembag_Ruby = 499,
		UNIT_STAT_gembag_Amethyst = 500,
		UNIT_STAT_gembag_Diamond = 501,
		UNIT_STAT_gembag_Emerald = 502,
		UNIT_STAT_gembag_Sapphire = 503,
		UNIT_STAT_gembag_Topaz = 504,
		UNIT_STAT_gembag_Skull = 505,
		UNIT_STAT_runebag_RunesA = 506,
		UNIT_STAT_runebag_RunesB = 507,
		UNIT_STAT_runebag_RunesC = 508,
		UNIT_STAT_runebag_RunesD = 509,
		UNIT_STAT_item_aura_display = 510,
	};

	class d2_common {
	public:
		static char* get_base();
		static int8_t diablo2::d2_common::get_item_page(structures::unit* item);
		static void diablo2::d2_common::empty_inventory_1(structures::inventory* inv);
		static void diablo2::d2_common::empty_inventory_2(structures::inventory* inv);
		static void diablo2::d2_common::free_trade_inventory(structures::inventory* inv);
		static int32_t get_item_type_from_unit(structures::unit* item);
		static int32_t diablo2::d2_common::get_previous_interact_guid(structures::unit* player);
		static int32_t get_inventory_index(structures::unit* item, char page, BOOL lod);
		static void* get_inventory_data(int32_t index, int32_t zero, char* data);
		static structures::unit* get_item_at_cell(structures::inventory* inv, uint32_t cellx, uint32_t celly,
			uint32_t* pcellx, uint32_t* pcelly, int32_t invIndex, uint8_t page);
		static uint32_t can_put_into_slot(structures::inventory* inv, structures::unit* item, uint32_t x, uint32_t y,
			uint32_t invIndex, structures::unit** lastBlockingUnit, uint32_t* lastBlockingUnitIndex, uint8_t page);

		static uint32_t get_item_type(structures::unit* item);
		static uint32_t get_item_type_class(structures::unit* item);
		static uint32_t diablo2::d2_common::get_item_primary_weapon_class(structures::unit* item);
		static uint32_t diablo2::d2_common::check_item_type_equiv(uint32_t itemtype, uint32_t itemtype_equiv);

		static structures::unit* inv_remove_item(structures::inventory* inventory, structures::unit* item);
		static BOOL inv_add_item(structures::inventory* inv, structures::unit* item, uint32_t x, uint32_t y,
			uint32_t invIndex, BOOL isClient, uint8_t page);
		static BOOL inv_update_item(structures::inventory* inv, structures::unit* item, BOOL isClient);

		static structures::items_line* get_item_record(uint32_t guid);
		static structures::item_types_line* get_item_type_record(uint32_t typeId);

		static uint32_t get_maximum_character_gold(structures::unit* player);
		static uint32_t get_item_unique_index(structures::unit* item);
		static int32_t set_stat(structures::unit* unit, unit_stats_t stat, uint32_t value, int16_t param);
		static int32_t get_stat(structures::unit* unit, unit_stats_t stat, int16_t param);
		static int32_t get_stat_signed(structures::unit* unit, unit_stats_t stat, int16_t param);

		static int32_t _10111(int32_t* x, int32_t* y);
		static int32_t _10116(int32_t x1, int32_t y1, int32_t* x, int32_t* y);

		static structures::room* get_room_from_unit(structures::unit* unit);

		static int32_t get_unit_size_x(structures::unit* unit);
		static int32_t get_unit_size_y(structures::unit* unit);

		static int32_t get_distance_between_units(structures::unit* unit1, structures::unit* unit2);

		static int32_t get_unit_x(structures::path* path);
		static int32_t get_unit_y(structures::path* path);
		static int32_t get_unit_precise_x(structures::unit* unit);
		static int32_t get_unit_precise_y(structures::unit* unit);

		static int32_t get_item_quality(structures::unit* item);
		static uint32_t diablo2::d2_common::set_unit_mode(structures::unit* item, uint32_t mode);

		static structures::unit* get_target_from_path(structures::path* path);
		static structures::unit* diablo2::d2_common::get_first_inventory_item(structures::inventory* inv);
		static structures::unit* diablo2::d2_common::get_last_inventory_item(structures::inventory* inv);
		static structures::unit* diablo2::d2_common::get_next_inventory_item(structures::unit* prev_item);
		static uint32_t diablo2::d2_common::get_max_cube_recipes();
		static void diablo2::d2_common::free_inventory(structures::inventory* inventory);
		static void diablo2::d2_common::refresh_unit_inventory(structures::unit* unit, bool set_update_flags);
		static void diablo2::d2_common::update_trade(structures::inventory* inventory, structures::unit* item);
		//static void diablo2::d2_common::set_item_flags(structures::unit* item, structures::itemflags_t dwFlag, bool bSet);

		static void diablo2::d2_common::add_property(structures::unit* item, D2PropertyStrc* pProperty, int nUnused);
		static void diablo2::d2_common::ITEMS_SetItemFlag(structures::unit* item, uint32_t dwFlag, BOOL bSet);

		//D2Common.0x6FDA42B0
		// D2ItemStatCostTxt* __fastcall ITEMS_GetItemStatCostTxtRecord(int nStatId)
		// Write a function signature for this function
		static structures::D2ItemStatCostTxt* get_item_stat_cost_record(int nStatId);

		//D2Common.0x6FD57720 (#10602)
		//int __stdcall DATATBLS_GetItemIdFromItemCode(uint32_t dwCode)
		static int get_item_id_from_item_code(uint32_t dwCode);

		//D2Common.0x6FD576D0 (#10601)
		// D2ItemsTxt* __stdcall DATATBLS_GetItemRecordFromItemCode(uint32_t dwCode, int* pItemId)
		static structures::items_line* get_item_record_from_item_code(uint32_t dwCode, int* pItemId);
	};
}
