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

namespace diablo2 {
	namespace structures {
		struct unit;
		struct path;
		struct inventory;
		struct room;

		struct items_line;
		struct item_types_line;
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
		UNIT_STAT_ITEM_CRUSHINGBLOW_BYTIME = 0x12C
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
	};
}
