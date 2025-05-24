#pragma once

#ifndef _D2STRUCTS_H
#define _D2STRUCTS_H
#include "D2TemplateDataTables.h"
#include "D2TemplatePacketDef.h"
//
//struct D2ShrinesTXT {
//  BYTE     Code;                         //+00
//  BYTE     NotUsed1[3];               //+01
//  DWORD     Arg1;                         //+04
//  DWORD     Arg2;                         //+08
//  DWORD     DurInFrames;               //+0C
//  BYTE     ResetTimeInMinutes;          //+10
//  BYTE     Rarity;                         //+11
//  char     ViewName[0x1F];               //+12
//  char     NiftyPhrase[0x7F];          //+32
//  BYTE     EffectClass;               //+B2
//  BYTE     NotUsed2;                    //+B3
//  DWORD     LevelMin;                    //+B4
// };*/
//
//enum D2CubeItemFlag {
//    CUBE_COWPORTAL = 1,
//    CUBE_USEITEM = 0xFF,
//    CUBE_USETYPE = 0xFE,
//    CUBE_TYPE = 0xFD,
//    CUBE_ITEM = 0xFC
//};
//
//
//struct CubeOutput//size: 0x54=84 (source total : fct of dll) {
//    union {
//        WORD flag;                //+00
//        struct {
//            WORD    keepModifiers : 1;            //",mod"
//            WORD    haveSockets : 1;                //",sock=#nbsocket"
//            WORD    isEthereal : 1;                //",eth"
//            WORD    isSpecificItem : 1;            //"#itemUniqueName" or "#itemSetName" (set byItemID too)
//            WORD    destroysFillers : 1;            //",uns"
//            WORD    removeFillers : 1;            //",rem"
//            WORD    regeneratesUnique : 1;        //",reg"
//            WORD    upgradeToExceptional : 1;        //",exc"
//            WORD    upgradeToElite : 1;            //",eli"
//            WORD    repair : 1;                    //",rep"
//            WORD    recharge : 1;                    //",rch"
//        };
//    };
//    WORD    ID;                    //+02
//    WORD    specificID;            //+04
//    BYTE    quality;            //+06    1=",low"; 2=",nor"; 3=",hiq"; 4=",mag"; 5=",set"; 6=",rar"; 7=",uni"; 8=",crf"; 9=",tmp"
//    BYTE    quantityOrNbSockets;//+07    ",qty=#quantity" Or ",sock=#nbsocket"
//    BYTE    outputType;            //+08    02="Pand. Portal" 01="Cow Portal" FF="usetype"; FE="useitem" FC="#item4Code" FD="#type4Code"
//    BYTE    lvl;                //+09    //+55
//    BYTE    plvl;                //+0A    //+56
//    BYTE    ilvl;                //+0B    //+57
//    WORD    Prefix;                //+0C
//    WORD    PrefixMin;            //+0E (always 0)
//    WORD    PrefixMax;            //+10 (always 0)
//    WORD    Suffix;                //+12
//    WORD    SuffixMin;            //+14 (always 0)
//    WORD    SuffixMax;            //+16 (always 0)
//    DWORD    mod1;                //+18    //+64(13)
//    WORD    mod1param;            //+1C    //+68
//    WORD    mod1min;            //+1E    //+6A
//    WORD    mod1max;            //+20    //+6C
//    BYTE    mod1chance;            //+22    //+6E
//    DWORD    mod2;                //+24    //+70
//    WORD    mod2param;            //+28    //+74
//    WORD    mod2min;            //+2A    //+76
//    WORD    mod2max;            //+2C    //+78
//    BYTE    mod2chance;            //+2E    //+7A
//    DWORD    mod3;                //+30    //+7C
//    WORD    mod3param;            //+34    //+80
//    WORD    mod3min;            //+36    //+82
//    WORD    mod3max;            //+38    //+84
//    BYTE    mod3chance;            //+3A    //+86
//    DWORD    mod4;                //+3C    //+88
//    WORD    mod41param;            //+40    //+8C
//    WORD    mod4min;            //+42    //+8E
//    WORD    mod4max;            //+44    //+90
//    BYTE    mod4chance;            //+46    //+92
//    DWORD    mod5;                //+48    //+94
//    WORD    mod5param;            //+4C    //+98
//    WORD    mod5min;            //+4E    //+9A
//    WORD    mod5max;            //+50    //+9C
//    BYTE    mod5chance;            //+52    //+9E
//};
//
//
//struct CubeInput//size: 8 (source total : fct of dll) {
//    union {
//        WORD flag;            //+00
//        struct {
//            WORD    byItemID : 1;                    //"#item4Code" or "any"
//            WORD    byItemTypeID : 1;                //"#itemType4Code"
//            WORD    haveNoSocket : 1;                //",nos"
//            WORD    haveSockets : 1;                //",sock"
//            WORD    isEthereal : 1;                //",eth"
//            WORD    isNotEthereal : 1;            //",noe"
//            WORD    isSpecificItem : 1;            //"#itemUniqueName" or "#itemSetName" (set byItemID too)
//            WORD    includeUpgradedVersions : 1;    //",upg"
//            WORD    isBasic : 1;                    //",bas"
//            WORD    isExceptional : 1;            //",exc"
//            WORD    isElite : 1;                    //",eli"
//            WORD    isNotRuneword : 1;            //",nru"
//        };
//    };
//    WORD    ID;                //+02    FFFF = any items
//    WORD    specificID;        //+04
//    BYTE    quality;        //+06    1=",low"; 2=",nor"; 3=",hiq"; 4=",mag"; 5=",set"; 6=",rar"; 7=",uni"; 8=",crf"; 9=",tmp"
//    BYTE    quantity;        //+07    ",qty=#quantity"
//};
//
//
//struct CubeMainBIN//size: 0x148=328  (source total : fct of dll) (total size in LoD1.10 : BDA0) {
//    BYTE        enabled;    //+00
//    BYTE        ladder;        //+01
//    BYTE        minDiff;    //+02
//    BYTE        playerClass;//+03 FF=all classes
//    BYTE        op;            //+04
//    DWORD        param;        //+08
//    DWORD        value;        //+0C
//    BYTE        numinputs;    //+10
//    WORD        version;    //+12
//    CubeInput    input1;        //+14
//    CubeInput    input2;        //+1C
//    CubeInput    input3;        //+24
//    CubeInput    input4;        //+2C
//    CubeInput    input5;        //+34
//    CubeInput    input6;        //+3C
//    CubeInput    input7;        //+44
//    CubeOutput    output1;    //+4C
//    CubeOutput    output2;    //+A0
//    CubeOutput    output3;    //+F4
//};
//
//struct TreasureClassBIN {
//
//};
//
//struct ObjectsBIN {
//
//};
//
//struct ExperienceBIN//size=0x20 {
//    DWORD Amazon;
//    DWORD Sorceress;
//    DWORD Necromancer;
//    DWORD Paladin;
//    DWORD Barbarian;
//    DWORD Druid;
//    DWORD Assassin;
//    DWORD ExpRatio;
//};
//
//struct LevelsBIN//size=0x220 {
//};
//
//struct GemsBIN //size: 0xC0=192 (source : fct of dll) {
//    char    name[0x20];        //+00
//    char    letter[6];        //+20
//    BYTE    uk1[2];            //+26
//    DWORD    code;            //+28
//    BYTE    uk2[2];            //+2C
//    BYTE    nummods;        //+2E
//    BYTE    transform;        //+2F
//    int        weaponmod1code;    //+30
//    DWORD    weaponmod1param;//+34
//    DWORD    weaponmod1min;    //+38
//    DWORD    weaponmod1max;    //+3C
//    int        weaponmod2code;    //+40
//    DWORD    weaponmod2param;//+44
//    DWORD    weaponmod2min;    //+48
//    DWORD    weaponmod2max;    //+4C
//    int        weaponmod3code;    //+50
//    DWORD    weaponmod3param;//+54
//    DWORD    weaponmod3min;    //+58
//    DWORD    weaponmod3max;    //+5C
//    int        helmmod1code;    //+60
//    DWORD    helmmod1param;    //+64
//    DWORD    helmmod1min;    //+68
//    DWORD    helmmod1max;    //+6C
//    int        helmmod2code;    //+70
//    DWORD    helmmod2param;    //+74
//    DWORD    helmmod2min;    //+78
//    DWORD    helmmod2max;    //+7C
//    int        helmmod3code;    //+80
//    DWORD    helmmod3param;    //+84
//    DWORD    helmmod3min;    //+88
//    DWORD    helmmod3max;    //+8C
//    int        shieldmod1code;    //+90
//    DWORD    shieldmod1param;//+94
//    DWORD    shieldmod1min;    //+98
//    DWORD    shieldmod1max;    //+9C
//    int        shieldmod2code;    //+A0
//    DWORD    shieldmod2param;//+A4
//    DWORD    shieldmod2min;    //+A8
//    DWORD    shieldmod2max;    //+AC
//    int        shieldmod3code;    //+B0
//    DWORD    shieldmod3param;//+B4
//    DWORD    shieldmod3min;    //+B8
//    DWORD    shieldmod3max;    //+BC
//};
//
//struct ItemsBIN //size: 0x1A8=424 (source : fct of dll) {
//    char    FlippyFile[0x20];        //+00
//    char    InvFile[0x20];            //+20
//    char    UniqueInvFile[0x20];    //+40
//    char    SetInvFile[0x20];        //+60
//    DWORD    ItemCode;                //+80
//    DWORD    NormCode;                //+84
//    DWORD    UberCode;                //+88
//    DWORD    HellCode;                //+8C
//    DWORD    AlternateGfx;            //+90
//    DWORD    pSpell;                    //+94
//    WORD    State;                    //+98
//    WORD    cState1;                //+9A
//    WORD    cState2;                //+9C
//    WORD    Stat1;                    //+9E
//    WORD    Stat2;                    //+A0
//    WORD    Stat3;                    //+A2
//    DWORD    Calc1;                    //+A4
//    DWORD    Calc2;                    //+A8
//    DWORD    Calc3;                    //+AC
//    DWORD    Len;                    //+B0
//    WORD    SpellDesc;                //+B4
//    WORD    SpellDescStr;            //+B6
//    DWORD    SpellDescCalc;            //+B8
//    DWORD    BetterGem;                //+BC
//    DWORD    WClass;                    //+C0
//    DWORD    TwoHandedWClass;        //+C4
//    DWORD    TMogType;                //+C8
//    DWORD    MinAC;                    //+CC
//    DWORD    MaxAC;                    //+D0
//    DWORD    GambleCost;                //+D4
//    DWORD    Speed;                    //+D8
//    DWORD    BitField;                //+DC
//    DWORD    Cost;                    //+E0
//    DWORD    MinStack;                //+E4
//    DWORD    MaxStack;                //+E8
//    DWORD    SpawnStack;                //+EC
//    DWORD    GemOffset;                //+F0
//    WORD    NameStr;                //+F4
//    WORD    Version;                //+F6
//    WORD    AutoPrefix;                //+F8
//    WORD    MissileType;            //+FA
//    BYTE    Rarity;                    //+FC
//    BYTE    Level;                    //+FD
//    BYTE    MinDam;                    //+FE
//    BYTE    MaxDam;                    //+FF
//    BYTE    MinMisDam;                //+100
//    BYTE    MaxMisDam;                //+101
//    BYTE    TwoHandMinDam;            //+102
//    BYTE    TwoHandMaxDam;            //+103
//    BYTE    RangeAdder;                //+104
//    BYTE    NotUsed1;                //+105
//    WORD    StrBonus;                //+106
//    WORD    DexBonus;                //+108
//    WORD    RegStr;                    //+10A
//    WORD    RegDex;                    //+10C
//    BYTE    Absorbs;                //+10E
//    BYTE    InvWidth;                //+10F
//    BYTE    InvHeight;                //+110
//    BYTE    Block;                    //+111
//    BYTE    Durability;                //+112
//    BYTE    NoDurability;            //+113
//    BYTE    Missile;                //+114
//    BYTE    Component;                //+115
//    BYTE    RArm;                    //+116
//    BYTE    LArm;                    //+117
//    BYTE    Torso;                    //+118
//    BYTE    Legs;                    //+119
//    BYTE    RSpad;                    //+11A
//    BYTE    LSpad;                    //+11B
//    BYTE    TwoHanded;                //+11C
//    BYTE    Usable;                    //+11D
//    WORD    Type;                    //+11E
//    WORD    Type2;                    //+120
//    BYTE    SubType;                //+122
//    BYTE    NotUsed2;                //+123
//    WORD    DropSound;                //+124
//    WORD    UseSound;                //+126
//    BYTE    DropSfxFrame;            //+128
//    BYTE    Unique;                    //+129
//    BYTE    Quest;                    //+12A
//    BYTE    QuestDiffCheck;            //+12B
//    BYTE    Transparent;            //+12C
//    BYTE    TransTbl;                //+12D
//    BYTE    NotUsed3;                //+12E
//    BYTE    LightRadius;            //+12F
//    BYTE    Belt;                    //+130
//    BYTE    AutoBelt;                //+131
//    BYTE    Stackable;                //+132
//    BYTE    Spawnable;                //+133
//    BYTE    SpellIcon;                //+134
//    BYTE    DurWarning;                //+135
//    BYTE    QntWaning;                //+136
//    BYTE    HasInv;                    //+137
//    BYTE    GemSockets;                //+138
//    BYTE    TransmoGrify;            //+139
//    BYTE    TMogMin;                //+13A
//    BYTE    TMogMax;                //+13B
//    BYTE    HitClass;                //+13C
//    BYTE    OneOrTwoHanded;            //+13D
//    BYTE    GemApplyType;            //+13E
//    BYTE    LevelReq;                //+13F
//    BYTE    MagicLvl;                //+140
//    BYTE    Transform;                //+141
//    BYTE    InvTrans;                //+142
//    BYTE    CompactSave;            //+143
//    BYTE    SkipName;                //+144
//    BYTE    NotUsed4;                //+145
//    BYTE    VendorMin[17];            //+146
//    BYTE    VendorMax[17];            //+157
//    BYTE    VendorMagicMin[17];        //+168
//    BYTE    VendorMagicMax[17];        //+179
//    BYTE    VendorMagicLvl[17];        //+18A
//    BYTE    NotUsed5;                //+19B
//    DWORD    NightmareUpgrade;        //+19C
//    DWORD    HellUpgrade;            //+1A0
//    BYTE    PermStoreItem;            //+1A4
//    BYTE    Multibuy;                //+1A5
//    WORD    zeros;                    //+1A6
//};
//
//struct RunesBIN    //size: 0x120=288 (1.10 ok) {
//    char    Name[0x40];        //+00
//    char    RuneName[0x40];    //+40
//    BYTE    Complete;        //+80
//    BYTE    Server;            //+81
//    WORD    RuneNameID;        //+82
//    WORD    zero;            //+84 (=0x0000)
//    WORD    Itypes[6];        //+86
//    WORD    Etypes[3];        //+92
//    int        Runes[6];        //+98
//    DWORD    t1code1;        //+B0
//    DWORD    t1param1;        //+B4
//    DWORD    t1min1;            //+B8
//    DWORD    t1max1;            //+BC
//    DWORD    t1code2;        //+C0
//    DWORD    t1param2;        //+C4
//    DWORD    t1min2;            //+C8
//    DWORD    t1max2;            //+CC
//    DWORD    t1code3;        //+D0
//    DWORD    t1param3;        //+D4
//    DWORD    t1min3;            //+D8
//    DWORD    t1max3;            //+DC
//    DWORD    t1code4;        //+E0
//    DWORD    t1param4;        //+E4
//    DWORD    t1min4;            //+E8
//    DWORD    t1max4;            //+EC
//    DWORD    t1code5;        //+F0
//    DWORD    t1param5;        //+F4
//    DWORD    t1min5;            //+F8
//    DWORD    t1max5;            //+FC
//    DWORD    t1code6;        //+100
//    DWORD    t1param6;        //+104
//    DWORD    t1min6;            //+108
//    DWORD    t1max6;            //+10C
//    DWORD    t1code7;        //+110
//    DWORD    t1param7;        //+114
//    DWORD    t1min7;            //+118
//    DWORD    t1max7;            //+11C
//};
//
///*struct ItemStatsBIN    //size: 0x234=564 {
//    BYTE    uk1[0x80];            //+00
//    DWORD    code;                //+80        // ou 0x90
//    BYTE    uk2[0xB3];            //+84
//    BYTE    itemStat137;        //+137
//};*/
//
//
//struct CharStatsBIN //size= 0xC4=196 {
//    WCHAR    nameU[0x10];        //+00
//    char    name[0x10];            //+20
//    BYTE    baseSTR;            //+30
//    BYTE    baseDEX;            //+31
//    BYTE    baseENE;            //+32
//    BYTE    baseVIT;            //+33
//    BYTE    baseStamina;        //+34
//    BYTE    hpadd;                //+35
//    BYTE    percentStr;            //+36
//    BYTE    percentInt;            //+37
//    BYTE    percentDex;            //+38
//    BYTE    percentVit;            //+39
//    BYTE    manaRegen;            //+3A
//    BYTE    __3B;                //+3B
//    DWORD    toHitFactor;        //+3C
//    BYTE    walkVelocity;        //+40
//    BYTE    runVelocity;        //+41
//    BYTE    runDrain;            //+42
//    BYTE    lifePerLevel;        //+43
//    BYTE    staminaPerLevel;    //+44
//    BYTE    manaPerLevel;        //+45
//    BYTE    lifePerVitality;    //+46
//    BYTE    staminaPerVitality;    //+47
//    BYTE    manaPerMagic;        //+48
//    BYTE    blockFactor;        //+49
//    WORD    __4A_startSkill;    //+4A
//    DWORD    baseWClass;            //+4C
//    BYTE    StatPerLevel;        //+50
//    BYTE    __51;                //+51
//    WORD    StrAllSkills;        //+52
//    WORD    StrSkillTab1;        //+54
//    WORD    StrSkillTab2;        //+56
//    WORD    StrSkillTab3;        //+58
//    WORD    StrClassOnly;        //+5A
//    DWORD    item1;
//    BYTE    item1Loc;
//    BYTE    item1Count;
//    WORD    item1Unknown;
//    DWORD    item2;
//    BYTE    item2Loc;
//    BYTE    item2Count;
//    WORD    item2Unknown;
//    DWORD    item3;
//    BYTE    item3Loc;
//    BYTE    item3Count;
//    WORD    item3Unknown;
//    DWORD    item4;
//    BYTE    item4Loc;
//    BYTE    item4Count;
//    WORD    item4Unknown;
//    DWORD    item5;
//    BYTE    item5Loc;
//    BYTE    item5Count;
//    WORD    item5Unknown;
//    DWORD    item6;
//    BYTE    item6Loc;
//    BYTE    item6Count;
//    WORD    item6Unknown;
//    DWORD    item7;
//    BYTE    item7Loc;
//    BYTE    item7Count;
//    WORD    item7Unknown;
//    DWORD    item8;
//    BYTE    item8Loc;
//    BYTE    item8Count;
//    WORD    item8Unknown;
//    DWORD    item9;
//    BYTE    item9Loc;
//    BYTE    item9Count;
//    WORD    item9Unknown;
//    DWORD    item10;
//    BYTE    item10Loc;
//    BYTE    item10Count;
//    WORD    item10Unknown;
//    BYTE    unknown2[0x18];
//};
///*
//struct D2CharStats {
//     wchar_t     Class_t[0x10];               //+00
//     char     Class[0x10];               //+20
//     BYTE     Str;                         //+30
//     BYTE     Dex;                         //+31
//     BYTE     Int;                         //+32
//     BYTE     Vit;                         //+33
//     BYTE     Stamina;                    //+34
//     BYTE     HpAdd;                         //+35
//     BYTE     PercentStr;                    //+36
//     BYTE     PercentInt;                    //+37
//     BYTE     PercentDex;                    //+38
//     BYTE     PercentVit;                    //+39
//     BYTE     ManaRegen;                    //+3A
//     BYTE     NotUsed1;                    //+3B
//     DWORD     ToHitFactor;               //+3C
//     BYTE     WalkVelocity;               //+40
//     BYTE     RunVelocity;               //+41
//     BYTE     RunDrain;                    //+42
//     BYTE     LifePerLevel;               //+43
//     BYTE     StamianPerLevel;          //+44
//     BYTE     ManaPerLevel;               //+45
//     BYTE     LifePerVitality;          //+46
//     BYTE     StaminaPerVitality;          //+47
//     BYTE     ManaPerMagic;               //+48
//     BYTE     BlockFactor;               //+49
//     BYTE     NotUsed2[2];               //+4A
//     DWORD     BaseWClass;                    //+4C
//     BYTE     StatPerLevel;               //+50
//     BYTE     NotUsed3;                    //+51
//     WORD     StrAllSkills;               //+52
//     WORD     StrSkillTab1;               //+54
//     WORD     StrSkillTab2;               //+56
//     WORD     StrSkillTab3;               //+58
//     WORD     StrClassOnly;               //+5A
//     DWORD     Item1;                         //+5C
//     BYTE     Item1Loc;                    //+60
//     BYTE     Item1Count;                    //+61
//     BYTE     NotUsed4[2];               //+62
//     DWORD     Item2;                         //+64
//     BYTE     Item2Loc;                    //+68
//     BYTE     Item2Count;                    //+69
//     BYTE     NotUsed5[2];               //+6A
//     DWORD     Item3;                         //+6C
//     BYTE     Item3Loc;                    //+70
//     BYTE     Item3Count;                    //+71
//     BYTE     NotUsed6[2];               //+72
//     DWORD     Item4;                         //+74
//     BYTE     Item4Loc;                    //+78
//     BYTE     Item4Count;                    //+79
//     BYTE     NotUsed7[2];               //+7A
//     DWORD     Item5;                         //+7C
//     BYTE     Item5Loc;                    //+80
//     BYTE     Item5Count;                    //+81
//     BYTE     NotUsed8[2];               //+82
//     DWORD     Item6;                         //+84
//     BYTE     Item6Loc;                    //+88
//     BYTE     Item6Count;                    //+89
//     BYTE     NotUsed9[2];               //+8A
//     DWORD     Item7;                         //+8C
//     BYTE     Item7Loc;                    //+90
//     BYTE     Item7Count;                    //+91
//     BYTE     NotUsed10[2];               //+92
//     DWORD     Item8;                         //+94
//     BYTE     Item8Loc;                    //+98
//     BYTE     Item8Count;                    //+99
//     BYTE     NotUsed11[2];               //+9A
//     DWORD     Item9;                         //+9C
//     BYTE     Item9Loc;                    //+A0
//     BYTE     Item9Count;                    //+A1
//     BYTE     NotUsed12[2];               //+A2
//     DWORD     Item10;                         //+A4
//     BYTE     Item10Loc;                    //+A8
//     BYTE     Item10Count;               //+A9
//     BYTE     NotUsed13[2];               //+AA
//     WORD     StartSkill;                    //+AC
//     WORD     Skill1;                         //+AE
//     WORD     Skill2;                         //+B0
//     WORD     Skill3;                         //+B2
//     WORD     Skill4;                         //+B4
//     WORD     Skill5;                         //+B6
//     WORD     Skill6;                         //+B8
//     WORD     Skill7;                         //+BA
//     WORD     Skill8;                         //+BC
//     WORD     Skill9;                         //+BE
//     WORD     Skill10;                    //+C0
//     BYTE     NotUSed14[2];               //+C2
//};
//
//*/
//
//struct UniqueItemsBIN //size=0x14C (332) {
//    WORD    uniqueId;        //+00
//    BYTE    uk1[0x20];        //+02
//    WORD    uniqueNameId;    //+22
//    BYTE    uk2[0x08];        //+24
//    union {
//        BYTE    flag;        //+2C
//        struct {
//            BYTE ukf : 2;
//            BYTE carry1 : 1;
//            BYTE ladder : 1;
//        };
//    };
//    BYTE    uk3[0x11F];        //+2D
//};
//
//struct SetsBIN //size=0x128 (source : fct of dll) {
//    WORD    setNum;            //+00
//    WORD    setNameIndex;    //+02
//    BYTE    uk3[0x124];        //+04
//};
//
//struct SetItemsBIN //size=0x1B8 (source : fct of dll) {
//    WORD    setNum;            //+00
//    char    index[0x20];    //+02
//    BYTE    uk1;            //+04
//    WORD    setNameId;        //+24
//    BYTE    uk2[0x6];        //+26
//    WORD    setId;            //+2C
//    BYTE    uk3[0x18A];        //+2E
//};
//
//
//struct SkillDescBIN //size=0x120=288  (source : fct of dll) {
//    BYTE    uk1[0x08];        //+00
//    WORD    strNameID;        //+08
//    WORD    strShortID;        //+0A
//    WORD    strLongID;        //+0C
//    WORD    strAltID;        //+0E
//    BYTE    uk2[0x110];        //+10
//};
//
//
//struct SkillsBIN //size=0x23C (572) {
//    WORD    skillID;        //+00
//    BYTE    uk1[0x0A];        //+02
//    BYTE    charclass;        //+0C
//    BYTE    uk2[0x08];        //+10
//    WORD    itypea1;        //+18
//    WORD    itypea2;        //+1A
//    WORD    itypea3;        //+1C
//    WORD    itypeb1;        //+1E
//    WORD    itypeb2;        //+20
//    WORD    itypeb3;        //+22
//    WORD    etypea1;        //+24
//    WORD    etypea2;        //+26
//    WORD    etypeb1;        //+28
//    WORD    etypeb2;        //+2A
//    BYTE    uk3[0x100];        //+2C
//    WORD    maxlvl;            //+12C
//    BYTE    uk4[0x42];        //+12E
//    int        skpoints;        //+170
//    WORD    reqlevel;        //+174
//    BYTE    uk5[0x1E];        //+176
//    DWORD    skilldesc;        //+194 (DWORD ?)
//    BYTE    uk6[0xA4];        //+198
//};
//
///*
//struct D2SkillsTXT {
//     WORD     SkillID;               //+000
//     BYTE     Mask1;                    //+002
//     BYTE     Mask2;                    //+003
//     BYTE     Unknown[8];               //+004
//     BYTE     CharClass;               //+00C
//     BYTE     unknown_1[0x17D];     //+00D
//     WORD     Mana;                    //+18A
//     short     LvlMana;               //+18C
//     BYTE     Attackrank;               //+18E
//     BYTE     LineOfSight;          //+18F
//     DWORD     Delay;                    //+190
//     WORD     SkillDesc;               //+194
//     BYTE     unknown_2[0xA6];     //+196
//};
//
//struct D2SkillsTXT {
//    WORD     SkillID;               //+000
//    BYTE     Mask1;                    //+002
//    BYTE     Mask2;                    //+003
//    BYTE     Unknown[8];               //+004
//    BYTE     CharClass;               //+00C
//    BYTE    Unknown2[3];          //+00D
//    BYTE     anim;                    //+010
//    BYTE     monanim;               //+011
//    BYTE     seqtrans;               //+012
//    BYTE     seqnum;                    //+013
//    BYTE     Unknown7;               //+014
//    BYTE     SelectProc;               //+015
//    BYTE     seqinput;               //+016
//    BYTE     Unknown3;               //+017
//    WORD     itypea1;               //+018
//    WORD     itypea2;               //+01A
//    WORD     itypea3;               //+01C
//    WORD     itypeb1;               //+01E
//    WORD     itypeb2;               //+020
//    WORD     itypeb3;               //+022
//    WORD     etypea1;               //+024
//    WORD     etypea2;               //+026
//    WORD     etypeb1;               //+028
//    WORD     etypeb2;               //+02A
//    WORD     srvstfunc;               //+02C
//    WORD     srvdofunc;               //+02E
//    WORD     srvprgfunc1;          //+030
//    WORD     srvprgfunc2;          //+032
//    WORD     srvprgfunc3;          //+034
//    WORD     Unknown8;               //+036
//    DWORD     prgcalc1;               //+038
//    DWORD     prgcalc2;               //+03C
//    DWORD     prgcalc3;               //+040
//    WORD     prgdam;                    //+044
//    WORD     srvmissile;               //+046
//    WORD     srvmissilea;          //+048
//    WORD     srvmissileb;          //+04A
//    WORD     srvmissilec;          //+04C
//    WORD     srvoverlay;               //+04E
//    DWORD     aurafilter;               //+050
//    WORD     aurastat1;               //+054
//    WORD     aurastat2;               //+056
//    WORD     aurastat3;               //+058
//    WORD     aurastat4;               //+05A
//    WORD     aurastat5;               //+05C
//    WORD     aurastat6;               //+05E
//    DWORD     auralencalc;          //+060
//    DWORD     aurarangecalc;          //+064
//    DWORD     aurastatcalc1;          //+068
//    DWORD     aurastatcalc2;          //+06C
//    DWORD     aurastatcalc3;          //+070
//    DWORD     aurastatcalc4;          //+074
//    DWORD     aurastatcalc5;          //+078
//    DWORD     aurastatcalc6;          //+07C
//    WORD     aurastate;               //+080
//    WORD     auratargetstate;     //+082
//    WORD     auraevent1;               //+084
//    WORD     auraevent2;               //+086
//    WORD     auraevent3;               //+088
//    WORD     auraeventfunc1;          //+08A
//    WORD     auraeventfunc2;          //+08C
//    WORD     auraeventfunc3;          //+08E
//    WORD     auratgtevent;          //+090
//    WORD     auratgteventfunc;     //+092
//    WORD     passivestate;          //+094
//    WORD     passiveitype;          //+096
//    WORD     passivestat1;          //+098
//    WORD     passivestat2;          //+09A
//    WORD     passivestat3;          //+09C
//    WORD     passivestat4;          //+09E
//    WORD     passivestat5;          //+0A0
//    DWORD     passivecalc1;          //+0A4
//    DWORD     passivecalc2;          //+0A8
//    DWORD     passivecalc3;          //+0AC
//    DWORD     passivecalc4;          //+0B0
//    DWORD     passivecalc5;          //+0B4
//    WORD     passiveevent;          //+0B8
//    WORD     passiveeventfunc;     //+0BA
//    WORD     summon;                    //+0BC
//    BYTE     pettype;               //+0BE
//    BYTE     summode;               //+0BF
//    DWORD     petmax;                    //+0C0
//    WORD     sumskill1;               //+0C4
//    WORD     sumskill2;               //+0C6
//    WORD     sumskill3;               //+0C8
//    WORD     sumskill4;               //+0CA
//    WORD     sumskill5;               //+0CC
//    DWORD     sumskcalc1;               //+0D0
//    DWORD     sumskcalc2;               //+0D4
//    DWORD     sumskcalc3;               //+0D8
//    DWORD     sumskcalc4;               //+0DC
//    DWORD     sumskcalc5;               //+0E0
//    WORD     sumumod;               //+0E4
//    WORD     sumoverlay;               //+0E6
//    WORD     cltmissile;               //+0E8
//    WORD     cltmissilea;          //+0EA
//    WORD     cltmissileb;          //+0EC
//    WORD     cltmissilec;          //+0EE
//    WORD     cltmissiled;          //+0F0
//    WORD     cltstfunc;               //+0F2
//    WORD     cltdofunc;               //+0F4
//    WORD     cltprgfunc1;          //+0F6
//    WORD     cltprgfunc2;          //+0F8
//    WORD     cltprgfunc3;          //+0FA
//    WORD     stsound;               //+0FC
//    WORD     stsoundclass;          //+0FE
//    WORD     dosound;               //+100
//    WORD     dosounda;               //+102
//    WORD     dosoundb;               //+104
//    WORD     castoverlay;          //+106
//    WORD     tgtoverlay;               //+108
//    WORD     tgtsound;               //+10A
//    WORD     prgoverlay;               //+10C
//    WORD     prgsound;               //+10E
//    WORD     cltoverlaya;          //+110
//    WORD     cltoverlayb;          //+112
//    DWORD     cltcalc1;               //+114
//    DWORD     cltcalc2;               //+118
//    DWORD     cltcalc3;               //+11C
//    WORD     ItemTarget;               //+120
//    WORD     ItemCastSound;          //+122
//    WORD     ItemCastOverlay;     //+124
//    WORD     Unknown4;               //+126
//    DWORD     perdelay;               //+128
//    WORD     maxlvl;                    //+12C
//    WORD     ResultFlags;          //+12E
//    DWORD     HitFlags;               //+130
//    DWORD     HitClass;               //+134
//    DWORD     calc1;                    //+138
//    DWORD     calc2;                    //+13C
//    DWORD     calc3;                    //+140
//    DWORD     calc4;                    //+144
//    DWORD     Param1;                    //+148
//    DWORD     Param2;                    //+14C
//    DWORD     Param3;                    //+150
//    DWORD     Param4;                    //+154
//    DWORD     Param5;                    //+158
//    DWORD     Param6;                    //+15C
//    DWORD     Param7;                    //+160
//    DWORD     Param8;                    //+164
//    WORD     weapsel;               //+168
//    WORD     ItemEffect;               //+16A
//    BYTE     ItemCltEffect;          //+16C
//    BYTE     ItemTgtDo;               //+16D
//    BYTE     ItemCheckStart;          //+16E
//    BYTE     ItemCltCheckStart;     //+16F
//    DWORD     skpoints;               //+170
//    WORD     reqlevel;               //+174
//    WORD     reqstr;                    //+176
//    WORD     reqdex;                    //+178
//    WORD     reqint;                    //+17A
//    WORD     reqvit;                    //+17C
//    WORD     reqskill1;               //+17E
//    WORD     reqskill2;               //+180
//    WORD     reqskill3;               //+182
//    WORD     startmana;               //+184
//    WORD     minmana;               //+186
//    WORD     manashift;               //+188
//    WORD     Mana;                    //+18A
//    short     LvlMana;               //+18C
//    BYTE     Attackrank;               //+18E
//    BYTE     LineOfSight;          //+18F
//    DWORD     Delay;                    //+190
//    WORD     SkillDesc;               //+194
//    WORD     Unknown5;               //+196
//    DWORD     ToHit;                    //+198
//    DWORD     LevToHit;               //+19C
//    DWORD     ToHitCalc;               //+1A0
//    BYTE     HitShift;               //+1A4
//    WORD     SrcDam;                    //+1A5
//    BYTE     Unknown6;               //+1A7
//    DWORD     MinDam;                    //+1A8
//    DWORD     MaxDam;                    //+1AC
//    DWORD     MinLevDam1;               //+1B0
//    DWORD     MinLevDam2;               //+1B4
//    DWORD     MinLevDam3;               //+1B8
//    DWORD     MinLevDam4;               //+1BC
//    DWORD     MinLevDam5;               //+1C0
//    DWORD     MaxLevDam1;               //+1C4
//    DWORD     MaxLevDam2;               //+1C8
//    DWORD     MaxLevDam3;               //+1CC
//    DWORD     MaxLevDam4;               //+1D0
//    DWORD     MaxLevDam5;               //+1D4
//    DWORD     DmgSymPerCalc;          //+1D8
//    DWORD     EType;                    //+1DC
//    DWORD     Emin;                    //+1E0
//    DWORD     Emax;                    //+1E4
//    DWORD     EMinLev1;               //+1E8
//    DWORD     EMinLev2;               //+1EC
//    DWORD     EMinLev3;               //+1F0
//    DWORD     EMinLev4;               //+1F4
//    DWORD     EMinLev5;               //+1F8
//    DWORD     EMaxLev1;               //+1FC
//    DWORD     EMaxLev2;               //+200
//    DWORD     EMaxLev3;               //+204
//    DWORD     EMaxLev4;               //+208
//    DWORD     EMaxLev5;               //+20C
//    DWORD     EDmgSymPerCalc;          //+210
//    DWORD     ELen;                    //+214
//    DWORD     ELevLen1;               //+218
//    DWORD     ELevLen2;               //+21C
//    DWORD     ELevLen3;               //+220
//    DWORD     ELenSymPerCalc;          //+224
//    WORD     restrict;               //+228
//    WORD     state1;                    //+22A
//    WORD     state2;                    //+22C
//    WORD     state3;                    //+22E
//    WORD     aitype;                    //+230
//    WORD     aibonus;               //+232
//    DWORD     CostMult;               //+234
//    DWORD     CostAdd;               //+238
//};
//Yohann: thx
//Kingpin: /*
//usemanaondo
//repeat
//TgtPlaceCheck
//interrupt
//leftskill
//AttackNoMana
//TargetItem
//TargetAlly
//TargetPet
//TargetCorpse
//SearchOpenXY
//SearchEnemyNear
//SearchEnemyXY
//TargetableOnly
//UseAttackRate
//durability
//enhanceable
//noammo
//InGame
//scroll
//general
//periodic
//aura
//passive
//Kick
//InTown
//prgstack
//finishing
//progressive
//range
//warp
//weaponsnd
//stsounddelay
//stsuccessonly
//lob
//decquant
//immediate
//
//Kingpin: those columns should be there also
//*/
//
//struct DifficultyLevelsBIN {
//    DWORD    resistPenalty;    //+00
//    BYTE    uk1[1];            //+04
//};
//
//struct SuperUniqueBIN {
//};
//
//struct MonStatsBIN    //size=0x1A8 (424) {
//    BYTE    uk1[0x6];        //+00
//    WORD    monsterNameID;    //+06
//    BYTE    uk2[0x168];        //+08
//    WORD    skill[8];        //+170
//    BYTE    skillArg[8];    //+180
//    BYTE    uk3[0x20];        //+188
//};                            //+1A8
//
//struct ItemTypesBIN //size=0xE4 (228) (source : fct of dll) {
//    DWORD    code;            //+00
//    BYTE    uk1[0x11];        //+04
//    BYTE    rare;            //+15
//    BYTE    uk2[0x09];        //+16
//    BYTE    staffmods;        //+1F
//    BYTE    uk3[0xC4];        //+20
//};                            //+E4
//
//struct AffixBIN //size=0x90 (144) (source : fct of dll) magicsuffix + magicprefix + automagic {
//    char    name[0x20];        //+00
//    WORD    nameCode;        //+20
//    WORD    version;        //+22
//    DWORD    mod1code;        //+24
//    DWORD    mod1param;        //+28
//    DWORD    mod1min;        //+2C
//    DWORD    mod1max;        //+30
//    DWORD    mod2code;        //+34
//    DWORD    mod2param;        //+38
//    DWORD    mod2min;        //+3C
//    DWORD    mod2max;        //+40
//    DWORD    mod3code;        //+44
//    DWORD    mod3param;        //+48
//    DWORD    mod3min;        //+4C
//    DWORD    mod3max;        //+50
//    BYTE    spawnable;        //+54
//    BYTE    uk1[0x1];        //+55
//    WORD    transformcolor;    //+56
//    DWORD    level;            //+58
//    DWORD    group;            //+5C
//    DWORD    maxlevel;        //+60
//    BYTE    rare;            //+64
//    BYTE    levelreq;        //+65
//    BYTE    classspecific;    //+66
//    BYTE    classf;            //+67
//    BYTE    classlevelreq;    //+68
//    BYTE    uk2[0x1];        //+69
//    WORD    itype1;            //+6A
//    WORD    itype2;            //+6C
//    WORD    itype3;            //+6E
//    WORD    itype4;            //+70
//    WORD    itype5;            //+72
//    WORD    itype6;            //+74
//    WORD    itype7;            //+76
//    WORD    etype1;            //+78
//    WORD    etype2;            //+7A
//    WORD    etype3;            //+7C
//    WORD    etype4;            //+7E
//    WORD    etype5;            //+80
//    BYTE    frequency;        //+82
//    BYTE    uk3[0x1];        //+83
//    DWORD    divide;            //+84
//    DWORD    multiply;        //+88
//    DWORD    add;            //+8C
//};
//
//struct PropertiesBIN // size=0x2E (46) (source total : fct of dll) {
//    WORD    modcode;        //+00
//    BYTE    set1;            //+02
//    BYTE    set2;            //+03
//    BYTE    set3;            //+04
//    BYTE    set4;            //+05
//    BYTE    set5;            //+06
//    BYTE    set6;            //+07
//    BYTE    set7;            //+08
//    BYTE    uk1[0x1];        //+09
//
//    WORD    val1;            //+0A
//    WORD    val2;            //+0C
//    WORD    val3;            //+0E
//    WORD    val4;            //+10
//    WORD    val5;            //+12
//    WORD    val6;            //+14
//    WORD    val7;            //+16
//
//    BYTE    func1;            //+18
//    BYTE    func2;            //+19
//    BYTE    func3;            //+1A
//    BYTE    func4;            //+1B
//    BYTE    func5;            //+1C
//    BYTE    func6;            //+1D
//    BYTE    func7;            //+1E
//    BYTE    uk2[0x1];        //+1F
//
//    WORD    stat1;            //+20
//    WORD    stat2;            //+22
//    WORD    stat3;            //+24
//    WORD    stat4;            //+26
//    WORD    stat5;            //+28
//    WORD    stat6;            //+2A
//    WORD    stat7;            //+2C
//};
//
//
//struct ItemStatCostBIN // size=0x144 (324) (source total : fct of dll) maxline = 0x1FF = 511 {
//    DWORD    statID;            //+00
//    union                    //+04
//    {
//        DWORD    flags;
//        struct
//        {
//            DWORD sendOther : 1;            //0x00000001
//            DWORD signedf : 1;            //0x00000002
//            DWORD damagerelated : 1;        //0x00000004
//            DWORD itemspecific : 1;        //0x00000008
//            DWORD direct : 1;                //0x00000010
//            DWORD fuk1 : 4;                //0x000001E0
//            DWORD updateanimrate : 1;        //0x00000200
//            DWORD fmin : 1;                //0x00000400
//            DWORD fcallback : 1;            //0x00000800
//            DWORD saved : 1;                //0x00001000
//            DWORD cvssigned : 1;            //0x00002000
//            DWORD fuk2 : 18;                //0xFFFFC000
//        };
//    };
//    BYTE    sendBits;        //+08
//    BYTE    sendParamBits;    //+09
//    BYTE    csvbits;        //+0A
//    BYTE    csvparam;        //+0B
//    DWORD    divide;            //+0C
//    DWORD    multiply;        //+10
//    DWORD    add;            //+14
//    BYTE    valshift;        //+18
//    BYTE    saveBits;        //+19
//    BYTE    saveBits109;    //+1A    //are only for conversion
//    BYTE    uk2[0x1];        //+1B
//    DWORD    saveAdd;        //+1C
//    DWORD    saveAdd109;        //+20    //are only for conversion
//    DWORD    saveParamBits;    //+24
//    BYTE    uk3[0x4];        //+28
//    DWORD    minaccr;        //+2C
//    BYTE    encode;            //+30
//    BYTE    uk4[0x1];        //+31
//    WORD    maxstat;        //+32
//    WORD    descpriority;    //+34
//    BYTE    descfunc;        //+36
//    BYTE    descval;        //+37
//    WORD    descstrpos;        //+38
//    WORD    descstrneg;        //+3A
//    WORD    descstr2;        //+3C
//    WORD    dgrp;            //+3E
//    BYTE    dgrpfunc;        //+40
//    BYTE    dgrpval;        //+41
//    WORD    dgrpstrpos;        //+42
//    WORD    dgrpstrneg;        //+44
//    WORD    dgrpstr2;        //+46
//    WORD    itemevent1;        //+48
//    WORD    itemevent2;        //+4A
//    WORD    itemeventfunc1;    //+4C
//    WORD    itemeventfunc2;    //+4E
//    BYTE    keepzero;        //+50
//    BYTE    uk5[0x3];        //+51
//    BYTE    op;                //+54
//    BYTE    opParam;        //+55
//    WORD    opStat1;        //+58
//    WORD    opStat2;        //+5A
//    WORD    opStat3;        //+5C
//    BYTE    uk6[0xE2];        //+5E
//    DWORD    stuff;            //+140
//};
//
//struct InventoryBIN // size=0xF0 (240) (source total : fct of dll) {
//    DWORD    invLeft;        //+00
//    DWORD    invRight;        //+04
//    DWORD    invTop;            //+08
//    DWORD    invBottom;        //+0C
//    BYTE    gridX;            //+10
//    BYTE    gridY;            //+11
//    BYTE    uk1[2];            //+12
//    DWORD    gridLeft;        //+14
//    DWORD    gridRight;        //+18
//    DWORD    gridTop;        //+1C
//    DWORD    gridBottom;        //+20
//    BYTE    gridBoxWidth;    //+24
//    BYTE    gridBoxHeight;    //+25
//    BYTE    uk2[2];            //+26
//    DWORD    rArmLeft;        //+28
//    DWORD    rArmRight;        //+2C
//    DWORD    rArmTop;        //+30
//    DWORD    rArmBottom;        //+34
//    BYTE    rArmWidth;        //+38
//    BYTE    rArmHeight;        //+39
//    BYTE    uk3[2];            //+3A
//    DWORD    torsoLeft;        //+3C
//    DWORD    torsoRight;        //+40
//    DWORD    torsoTop;        //+44
//    DWORD    torsoBottom;    //+48
//    BYTE    torsoWidth;        //+4C
//    BYTE    torsoHeight;    //+4D
//    BYTE    uk4[2];            //+4E
//    DWORD    lArmLeft;        //+50
//    DWORD    lArmRight;        //+54
//    DWORD    lArmTop;        //+58
//    DWORD    lArmBottom;        //+5C
//    BYTE    lArmWidth;        //+60
//    BYTE    lArmHeight;        //+61
//    BYTE    uk5[2];            //+62
//    DWORD    headLeft;        //+64
//    DWORD    headRight;        //+68
//    DWORD    headTop;        //+6C
//    DWORD    headBottom;        //+70
//    BYTE    headWidth;        //+74
//    BYTE    headHeight;        //+75
//    BYTE    uk6[2];            //+76
//    DWORD    neckLeft;        //+78
//    DWORD    neckRight;        //+7C
//    DWORD    neckTop;        //+80
//    DWORD    neckBottom;        //+84
//    BYTE    neckWidth;        //+88
//    BYTE    neckHeight;        //+89
//    BYTE    uk7[2];            //+8A
//    DWORD    rHandLeft;        //+8C
//    DWORD    rHandRight;        //+90
//    DWORD    rHandTop;        //+94
//    DWORD    rHandBottom;    //+98
//    BYTE    rHandWidth;        //+9C
//    BYTE    rHandHeight;    //+9D
//    BYTE    uk8[2];            //+9E
//    DWORD    lHandLeft;        //+A0
//    DWORD    lHandRight;        //+A4
//    DWORD    lHandTop;        //+A8
//    DWORD    lHandBottom;    //+AC
//    BYTE    lHandWidth;        //+B0
//    BYTE    lHandHeight;    //+B1
//    BYTE    uk9[2];            //+B2
//    DWORD    beltLeft;        //+B4
//    DWORD    beltRight;        //+B8
//    DWORD    beltTop;        //+BC
//    DWORD    beltBottom;        //+C0
//    BYTE    beltWidth;        //+C4
//    BYTE    beltHeight;        //+C5
//    BYTE    uk10[2];        //+C6
//    DWORD    feetLeft;        //+C8
//    DWORD    feetRight;        //+CC
//    DWORD    feetTop;        //+D0
//    DWORD    feetBottom;        //+D4
//    BYTE    feetWidth;        //+D8
//    BYTE    feetHeight;        //+D9
//    BYTE    uk11[2];        //+DA
//    DWORD    glovesLeft;        //+DC
//    DWORD    glovesRight;    //+E0
//    DWORD    glovesTop;        //+E4
//    DWORD    glovesBottom;    //+E8
//    BYTE    glovesWidth;    //+EC
//    BYTE    glovesHeight;    //+ED
//    BYTE    uk12[2];        //+EE
//};
//
//
//struct DataTables//01EE6A20 * 01FDA2D0 //second comments=1.11 {
//    BYTE            uk1[0xA78];            //+000
//    MonStatsBIN* monStats;            //+A78 //1.11 and 1.10
//    BYTE            uk2[0x4];            //+A7C
//    DWORD            nbMonStats;            //+A80 //1.11 and 1.10
//    BYTE            uk3[0x108];            //+A84
//    SkillDescBIN* skilldesc;            //+B8C
//    BYTE            uk4[0x4];            //+B90
//    DWORD            nbSkilldesc;        //+B94
//    SkillsBIN* skills;                //+B98
//    BYTE            uk5[0x4];            //+B9C
//    DWORD            nbSkills;            //+BA0
//    int* nbSkillsPerPlayer;    //+BA4
//    int                maxSkillsPerPlayer;    //+BA8
//    WORD* playerSkills;        //+BAC
//    BYTE            uk6[0x14];            //+BB0
//    CharStatsBIN* charStats;            //+BC4
//    DWORD            nbCharStats;        //+BC8
//    ItemStatCostBIN* itemStatCost;        //+BCC
//    BYTE            uk7[4];                //+BD0
//    DWORD            nbItemStatCosts;    //+BD4
//    WORD* statsDescPriority;    //+BD8
//    DWORD            nbStatsDescPriority;//+BDC
//    BYTE            uk8[0x18];            //+BE0
//    ItemTypesBIN* itemTypes;            //+BF8
//    DWORD            nbItemTypes;        //+BFC
//    BYTE            uk9[0x0C];            //+C00
//    SetsBIN* sets;                //+C0C //1.11 and 1.10
//    DWORD            nbSets;                //+C10 //1.11 and 1.10
//    BYTE            uk9b[0x4];            //+C14
//    SetItemsBIN* setItems;            //+C18 //1.11 and 1.10
//    DWORD            nbSetItems;            //+C1C //1.11 and 1.10
//    BYTE            uk10[0x4];            //+C20
//    UniqueItemsBIN* uniqueItems;        //+C24 //1.11 and 1.10
//    DWORD            nbUniqueItems;        //+C28 //1.11 and 1.10
//    BYTE            uk11[0x2C];            //+C2C
//    LevelsBIN* levels;                //+C58
//    DWORD            nbLevels;            //+C5C
//    BYTE            uk12[0x64];            //+C60
//    CubeMainBIN* cubemain;            //+CC4 //14C0 by 148 for 1.11
//    DWORD            nbCubemain;            //+CC8 //14C4 for 1.11
//    DWORD            nbInventory;        //+CCC
//    InventoryBIN* inventory;            //+CD0
//    BYTE            uk13[0x04];            //+CD4
//    DWORD            nbItems;            //+CD8
//    ItemsBIN* items;                //+CDC
//    ItemsBIN* itemsBis;            //+CE0
//    BYTE            uk14[0x1F8];        //+CDC
//    DWORD            nbRunes;            //+EDC
//    RunesBIN* runes;                //+EE0
//};
//
//
///* NEXT NOT TESTED IN 1.10 (1.09b)
//
//struct AffixTxt {
//    BYTE    mName[0x21];     // +00
//    WORD    mVersion;        // +22
//    DWORD   mMod1code;       // +24
//    DWORD   mMod1param;      // +28
//    DWORD   mMod1min;        // +2c
//    DWORD   mMod1max;        // +30
//    DWORD   mMod2code;       // +34
//    DWORD   mMod2param;      // +38
//    DWORD   mMod2min;        // +3c
//    DWORD   mMod2max;        // +40
//    DWORD   mMod3code;       // +44
//    DWORD   mMod3param;      // +48
//    DWORD   mMod3min;        // +4c
//    DWORD   mMod3max;        // +50
//    WORD    mSpawnable;      // +54
//    WORD    mTransformcolor; // +56
//    DWORD   mLevel;          // +58
//    DWORD   mGroup;          // +5c
//    DWORD   mMaxlevel;       // +60
//    BYTE    mRare;           // +64
//    BYTE    mLevelreq;       // +65
//    BYTE    mClassspecific;  // +66
//    BYTE    mClass;          // +67
//    BYTE    classlevelreq;   // +68
//    BYTE    itype1;          // +69
//    BYTE    itype2;          // +6a
//    BYTE    itype3;          // +6b
//    BYTE    itype4;          // +6c
//    BYTE    itype5;          // +6d
//    BYTE    itype6;          // +6e
//    BYTE    itype7;          // +6f
//    BYTE    etype1;          // +70
//    BYTE    etype2;          // +71
//    BYTE    etype3;          // +72
//    BYTE    etype4;          // +73
//    BYTE    etype5;          // +74
//    DWORD   frequency;       // +75
//    DWORD   mDivide;         // +79
//    DWORD   mMultiply;       // +7D
//    DWORD   mAdd;            // +81
//};
//
//struct AffixTxtTable {
//    DWORD       mNbAffix;           // +00 = total affixes
//    AffixTxt*   mStartTable;        // +04 = beginnig ptr
//    AffixTxt*   mSuffixTable;       // +08 = suffix.txt ptr
//    AffixTxt*   mPrefixTable;       // +0c = prefix.txt ptr
//    AffixTxt*   mAutomagicTable;    // +10 = automagic.txt ptr
//};
//*/
//
//struct SkillsBIN;
//struct SkillData;
//struct ObjectsBIN;
//
//#pragma pack(push, 1)
//
//struct CBPlayerData;
//struct CBItemData;
//
//struct NetClient;
//
//struct Game;
//struct Unit;
//struct ActMap;
//struct ActData;
//struct Level;
//struct RoomEx;
//struct Room;
//
//struct UnitAny;
//struct CellContext;
//
//
//struct DT1SubBlock {
//    WORD xPos;                        //0x00
//    WORD yPos;                        //0x02
//    WORD _1;                        //0x04
//    BYTE gridX;                        //0x06
//    BYTE gridY;                        //0x07
//    WORD wTileFormat;                //0x08
//    DWORD dwSubLength;                //0x0A
//    WORD _2;                        //0x0E
//    DWORD* pDataOffset;                //0x10
//};
//
//struct TileContext // *.dt1 {
//    DWORD dwDirection;                //0x00
//    WORD wRoofIndex;                //0x04
//    BYTE bSound;                    //0x06
//    BYTE bAnimated;                    //0x07
//    DWORD dwSizeY;                    //0x08
//    DWORD dwSizeX;                    //0x0C
//    DWORD dwZeros1;                    //0x10
//    DWORD dwOrientation;            //0x14
//    DWORD dwMainIndex;                //0x18
//    DWORD dwSubIndex;                //0x1C
//    DWORD dwFrame;                    //0x20
//    BYTE  _1a;                        //0x24 DT1's unknown_a ...
//    BYTE  _1c;                        //0x25
//    BYTE  _1b;                        //0x26
//    BYTE  _1d;                        //0x27
//    BYTE  bFlags[25];                //0x28 For each tile <not sure>
//    BYTE  _2;                        //0x39
//    WORD sCacheIndex;                //0x40
//    DWORD _18;                        //0x44
//    DWORD dwDataPtr;                //0x48 pointer to sub-block headers
//    DWORD dwSize;                    //0x4C length of the sub-blocks
//    DWORD dwSubBlocks;                //0x50
//    DT1SubBlock* pBlocks;            //0x54
//    char* szTileName;                //0x58
//    DWORD **ptBlock;                //0x5C <not sure - maybe its a struct>
//};
//
//struct GFXSettings // size 0x18 {
//    DWORD bPerspectiveEnable; // 0
//    DWORD bPerspectiveCapable; // 1
//    DWORD bLowQuality; //2
//    DWORD dwGammaValue; //3
//    DWORD bVSync; //4
//    DWORD bBlendedShadows; // 5
//};
//
//struct GFXHelpers {
//    void(__fastcall *FillYBufferTable)(void *ppvBits, int nWidth, int nHeight, int aZero);
//    void(__fastcall *DrawVisTile)(int a1, int a2, int a3);
//    void(__fastcall *f3)(int a1, int a2, int a3, int a4);
//    void(__fastcall *DrawGroundTile)(TileContext* tc, int xPos, int yPos, int* pLight);
//    void(__fastcall *DrawWallTile)(int a1, int a2, int a3, int a4);
//    void(__fastcall *DrawBlendedVisTile)(int a1, int a2, int a3, int a4);
//    void(__fastcall *DrawRoofTile)(int a1, int a2, int a3, int a4, int a5);
//};
//
//struct D2RGB {
//    BYTE r;
//    BYTE g;
//    BYTE b;
//};
//
//struct fnRendererCallbacks {
//    BOOL(__fastcall *InitWindow)(HINSTANCE); // 0
//    BOOL(__fastcall *InitPerspective)(GFXSettings* pgfxSettings, GFXHelpers* pfnHelpers); // 1
//    BOOL(__fastcall *Release)(); // 2
//    BOOL(__fastcall *Init)(HANDLE hWnd, int nResolution); // 3
//    BOOL(__fastcall *Shutdown)(); // 4
//    BOOL(__fastcall *EndCutscene)(HANDLE hWnd, int nResolution, int); // 5
//    BOOL(__fastcall *BeginScene)(BOOL bClear, BYTE nRed, BYTE nGreen, BYTE nBlue); // 6
//    BOOL(__fastcall *EndScene1)(); // 7
//    BOOL(__fastcall *EndScene2)(); // 8
//    BOOL(__fastcall *ResizeWin)(HANDLE HWND, int bResolution); // 9
//    BOOL(__fastcall *GetBackBuffer)(D2RGB* pBuffer); // 10
//    BOOL(__fastcall *ActivateWindow)(int Unk, int Contrast); // 11
//    BOOL(__fastcall *SetOption)(int nOption, int nValue); // 12
//    BOOL(__fastcall *BeginCutscene)(); // 13
//    BOOL(__fastcall *PlayCutscene)(const char* szFile, int nResolution, void *pfframe); // 14
//    BOOL(__fastcall *CheckCutscene)(); // 15
//    BOOL(__fastcall *DecodeSmacker)(const char *szsmacker, BYTE *pbuffer, int nVersion); // 16
//    BOOL(__fastcall *PlaySmacker)(void *pcontext); // 17
//    BOOL(__fastcall *CloseSmacker)(void *pcontext); // 18
//    int(__fastcall *GetRenderStats)(); // 19
//    void(__fastcall *GetScreenSize)(int *pwidth, int *pheight); // 20
//    void(__fastcall *SetScaleFactor)(int nFactor); // 21
//    BOOL(__fastcall *SetGamma)(int nGamma); // 22
//    BOOL(__fastcall *CheckGamma)(); // 23
//    BOOL(__fastcall *SetPerspectiveScale)(); // 24
//    BOOL(__fastcall *AdjustPerspective)(int nXpos, int nYpos, int nBais, int *pxadjust, int *pyadjust); // 25
//    BOOL(__fastcall *ScalePerspectivepos)(int nXpos, int nYpos, int nAngle, int *pxpos, int *pypos, BOOL bOrder); // 26
//    BOOL(__fastcall *SetDefperspectivefactor)(); // 27
//    void(__fastcall *SetPalette)(BYTE* pPalette); // 28
//    BOOL(__fastcall *SetPalettetable)(BYTE** pPalette); // 29
//    BOOL(__fastcall *SetGlobalLight)(BYTE nRed, BYTE nGreen, BYTE nBlue); // 30
//    BOOL(__fastcall *DrawGroundTile)(TileContext* pTile, DWORD** pLight, int nXpos, int nYpos, int nWorldXpos, int nWorldYpos, BYTE nAlpha, int nScreenPanels, BOOL bOne); // 31
//    BOOL(__fastcall *DrawPerspectiveImage)(CellContext* pData, unsigned int nXpos, unsigned int nYpos, BYTE dwGamma, D2C_DrawModes nDrawMode, int nScreenMode, BYTE* pPalette); // 32
//    BOOL(__fastcall *DrawImage)(CellContext* pData, unsigned int nXpos, unsigned int nYpos, BYTE dwGamma, D2C_DrawModes nDrawMode, BYTE* pPalette); // 33
//    BOOL(__fastcall *DrawColouredImage)(CellContext* pData, int nXpos, int nYpos, BYTE dwGamma, D2C_DrawModes nDrawMode, int nColor); // 34
//    BOOL(__fastcall *DrawVerticalCropImage)(CellContext* pData, int nXpos, int nYpos, int nSkipLines, int nDrawLines, D2C_DrawModes nDrawMode); // 35
//    BOOL(__fastcall *DrawShadows)(CellContext* pData, int nXpos, int nYpos); // 36
//    BOOL(__fastcall *DrawImageFast)(CellContext* pData, int nXpos, int nYpos, BYTE nPaletteIndex); // 37
//    BOOL(__fastcall *DrawClippedImage)(CellContext* pData, int nXpos, int nYpos, void* pCropRect, D2C_DrawModes nDrawMode); // 38
//    BOOL(__fastcall *DrawWallTile)(TileContext* pTile, int nXpos, int nYpos, DWORD** pLight, int nScreenPanels); // 39
//    BOOL(__fastcall *DrawRoofTile)(TileContext* pTile, int nXpos, int nYpos, DWORD** pLight, int nScreenPanels, BYTE nAlpha); // 40
//    BOOL(__fastcall *DrawVisTile)(TileContext* pTile, int nXpos, int nYpos, D2C_DrawModes nDrawMode, int nScreenPanels); // 41
//    BOOL(__fastcall *DrawRect)(RECT *Prect, BYTE nColor); // 42
//    BOOL(__fastcall *DrawRectEx)(RECT *Prect, BYTE nColor); // 43
//    BOOL(__fastcall *DrawSolidRect)(RECT *Prect, BYTE nColor); // 44
//    BOOL(__fastcall *DrawSquare)(POINT *pos, int nSize, BYTE nColor); // 45
//    BOOL(__fastcall *DrawSolidRectEx)(int nXStart, int nYStart, int nXEnd, int nYEnd, BYTE nColor, D2C_DrawModes nDrawMode); // 46
//    BOOL(__fastcall *DrawSolidRectAlpha)(int nXStart, int nYStart, int nXEnd, int nYEnd, BYTE nColor, BYTE nAlpha); // 47
//    BOOL(__fastcall *DrawLine)(int nXStart, int nYStart, int nXEnd, int nYEnd, BYTE nColor, BYTE nAlpha); // 48
//    BOOL(__fastcall *ClearScreen)(BOOL bPartial); // 49
//    BOOL(__fastcall *DrawString)(int nXpos, int nYpos, const char *msg, ...); // 50
//    BOOL(__fastcall *DrawLight)(DWORD *plight, DWORD *pplayerlight, int nXpos, int nYpos); // 51
//    BOOL(__fastcall *DebugFillBackBuffer)(int xPos, int yPos); // 52
//    BOOL(__fastcall *ClearCaches)(); // 53
//};
//
////struct Room//size=0x80
////{
////    //ptRoom +48 0 = spawn new units (monster, objects e.tc), 1 = don't spawn any new units
////    DWORD    seed1;                        //+00
////    DWORD    seed2;                        //+04
////    BYTE    uk8[0x1C];                    //+08
////    Room*    ptNearRooms;                //+24
////    DWORD    nbNearRooms;                //+28
////    Unit*    unit;                        //+2C
////    BYTE    uk5[0x44];                    //+30
////    Room*    ptNextRoom;                    //+74
////    union
////    {
////        BYTE    flags;                    //+78
////        struct
////        {
////            BYTE isGenerated:1;
////            BYTE isGenerated2:1;//???
////        };
////    };
////};
//
///****************************************************************************
//*                                                                           *
//* DEFINITIONS                                                               *
//*                                                                           *
//*****************************************************************************/
//
//
//
//#pragma pack(1)
//struct sWinMessage {
//    DWORD    msg;
//    DWORD    type;
//    DWORD    key;
//    WORD    x;
//    WORD    y;
//    DWORD    uk3;
//    DWORD    uk4;
//    DWORD    managed;
//    DWORD    unmanaged;
//};
//#pragma pack()
//
///*
//struct sDrawImageInfo//size = 0x48 (until 1.10) {
//    void* reserved;
//    void* image;
//    DWORD frame;
//    DWORD zero[0xF];
//};*/
//
//struct sDrawImageInfo//size = 0x48 {
//    DWORD uk1;
//    DWORD uk2;
//    void* image;
//    DWORD uk3;
//    DWORD uk4;
//    void* reserved;
//    DWORD uk5[0xB];
//    DWORD frame;
//};
//
//
//struct saveBitField {
//    BYTE*    data;
//    DWORD    maxBits;
//    DWORD    currentByte;
//    DWORD    currentBit;
//    DWORD    overflaw;
//};
//
//struct BINField {
//   char*    fieldName;
//   int        type;
//   DWORD    strLength;
//   DWORD    offset;
//   void*    lookup;
//};
//
//struct ItemGenerationData //size=0x84 {
//    Unit*    ptMonster;        //+00
//    BYTE    uk1[0x04];        //+04
//    Game*    ptGame;            //+08
//    DWORD    ilvl;            //+0C
//    BYTE    uk2[0x04];        //+10
//    DWORD    nTxtFileNo;        //+14
//    DWORD    unknow1;        //+18
//    DWORD    x;                //+1C
//    DWORD    y;                //+20
//    Room*    ptRoom;            //+24
//    WORD    unknow2;        //+28
//    WORD    gameUnknown2;    //+2A
//    BYTE    uk3[0x04];        //+2C
//    DWORD    quality;        //+30
//    BYTE    uk4[0x34];        //+34
//    int        prefixID[3];    //+68 (-1: no prefix; 0:random prefix)
//    int        suffixID[3];    //+74 (-1: no suffix; 0:random suffix)
//    union {                    //+80
//        DWORD    flags;
//        struct {
//            DWORD    fuk1:5;        //0x0000001F
//            DWORD    unknow3:1;    //0x00000020
//        };
//    };
//};
//
//struct AIParam {
//    void*            control;            //+0x00
//    void*            u_0x04;             //+0x04
//    Unit*            target;                //+0x08
//    void*            u_0x0C;             //+0x0C
//    void*            u_0x10;             //+0x10
//    DWORD            distanceToTarget;   //+0x14
//    BOOL            engagedInCombat;    //+0x18
//    MonStatsBIN*    ptMonStatsBIN;        //+0x1C
//    void*            ptMonStats2BIN;        //+0x20
//};
//
///*=================================================================*/
///*   Data Structure to Update Client                               */
///*=================================================================*/
//struct DataPacket {
//    BYTE mType;            // +00 Packet Type
//    BYTE mFunc;            // +02 Function to Call
//    BYTE mSize;            // +04 Size of the Packet
//    DWORD mReserved;    // +06 Used with items
//    BYTE mUnitType;        // +0A Always '0'
//    DWORD mPlayerID;    // +0C The Player ID (ptUnit + 08h)
//    DWORD mItemID;        // +10 The Item ID
//    DWORD mMod1_ID;        // +14 The Stat/Mod ID
//    DWORD mParam1;        // +18 3 Paramters
//    DWORD mParam2;        // +1C
//    DWORD mParam3;        // +20
//};
//
///*=================================================================*/
///*  Skill Structure.                                               */
///*=================================================================*/
//
//struct SkillData {                                //Offset from Code. (size = 0x3C or 0x40)
//    SkillsBIN* ptSkillBin;        //+00
//    SkillData* ptNextSkill;    //+04
//    DWORD    mode;           //+08
//    DWORD    uk1[3];         //+0C
//    DWORD    targetInfo;     //+18
//    DWORD    targetType;     //+1c
//    DWORD    targetUNID;     //+20
//    DWORD    uk2;            //+24
//    DWORD    slvl;           //+28
//    DWORD    slvlBonus;      //+2c
//    DWORD    uk3;            //+30
//    int        state;          //+34 (-1 if index exist)
//    DWORD    param1;            //+38 (nb current charge)
//    DWORD    param2;            //+3C
//};
//
//struct Skills {                                //Offset from Code.
//    DWORD        gameRelated;    //+00
//    SkillData* ptFirstSkill;    //+04
//    SkillData* ptLeftSkill;    //+08
//    SkillData* ptRightSkill;    //+0c
//    SkillData* ptCurrentSkill;    //+10
//};
//
//
///*=================================================================*/
///*   Unit Data Structure.                                          */
///*=================================================================*/
//
//struct Position {
//    DWORD x;
//    DWORD y;
//};
//
///*struct Room//size=0x80 {
////ptRoom +48 0 = spawn new units (monster, objects e.tc), 1 = don't spawn any new units
//    DWORD    seed1;                        //+00
//    DWORD    seed2;                        //+04
//    BYTE    uk8[0x1C];                    //+08
//    Room*    ptNearRooms;                //+24
//    DWORD    nbNearRooms;                //+28
//    Unit*    unit;                        //+2C
//    BYTE    uk5[0x44];                    //+30
//    Room*    ptNextRoom;                    //+74
//    union
//    {
//        BYTE    flags;                    //+78
//        struct
//        {
//            BYTE isGenerated:1;
//            BYTE isGenerated2:1;//???
//        };
//    };
//};*/
//
//struct RoomEx //size=5C {
//    DWORD        __00[2];        //0x00
//    RoomEx**    roomExNear;        //0x08
//    DWORD        __0C[5];        //0x0C
//    struct {
//        INT32    roomNumber;        //0x00
//        void*    __04;            //0x04
//        PINT32    subNumber;        //0x08
//    } *pType2Info;                //0x20
//    RoomEx*        ptNextRoomEx;    //0x24
//    UINT32        roomFlags;        //0x28
//    UINT32        roomsNearCount;    //0x2C
//    Room*        ptRoom;            //0x30
//    INT32        posX;            //0x34
//    INT32        posY;            //0x38
//    INT32        sizeX;            //0x3C
//    INT32        sizeY;            //0x40
//    DWORD        __44;            //0x44
//    UINT32        presetType;        //0x48
//    void*        ptWarpTiles;    //0x4C
//    DWORD        __50[2];        //0x50
//    Level*        ptLevel;        //0x58
//};
//
//struct Room //size=0x80 {
//    Room**        ptNearRooms;        //0x00
//    DWORD        __04[3];            //0x04
//    RoomEx*        ptRoomEx;            //0x10
//    DWORD        __14[3];            //0x14
//    void*        coll;                //0x20
//    INT32        nbNearRooms;        //0x24
//    DWORD        __28[9];            //0x28
//    DWORD        startX;                //0x4C
//    DWORD        startY;                //0x50
//    DWORD        sizeX;                //0x54
//    DWORD        sizeY;                //0x58
//    DWORD        __5C[6];            //0x5C
//    Unit*        ptFirstUnit;        //0x74
//    DWORD        __78;                //0x78
//    Room*        ptNextRoom;            //0x7C
//};
//
//struct items_line {
//    char flippy_file[32]; //0x00
//    char inv_file[32]; //0x20
//    char unique_inv_file[32]; //0x40
//    char setinv_file[32]; //0x60
//    union {
//        uint32_t number_code; //0x80
//        char string_code[4]; //0x80
//    };
//
//    uint32_t norm_code; //0x84
//    uint32_t uber_code; //0x88
//    uint32_t ultra_code; //0x8C
//    uint32_t alternate_gfx; //0x90
//    void* spell; //0x94
//    uint16_t state; //0x98
//    uint16_t cstate1; //0x9A
//    uint16_t cstate2; //0x9C
//    uint16_t stat1; //0x9E
//    uint16_t stat2; //0xA0
//    uint16_t stat3; //0xA2
//    uint32_t calc1; //0xA4
//    uint32_t calc2; //0xA8
//    uint32_t calc3; //0xAC
//    uint32_t len; //0xB0
//    uint16_t spell_desc; //0xB4
//    uint16_t spell_desc_str; //0xB6
//    uint32_t spell_desc_calc; //0xB8
//    uint32_t better_gem; //0xBC
//    uint32_t weapon_class; //0xC0
//    uint32_t two_handed_weapon_class; //0xC4
//    uint32_t transmog_type; //0xC8
//    uint32_t min_ac; //0xCC
//    uint32_t max_ac; //0xD0
//    uint32_t gamble_cost; //0xD4
//    uint32_t speed; //0xD8
//    uint32_t bit_field1; //0xDC
//    uint32_t cost; //0xE0
//    uint32_t min_stack; //0xE4
//    uint32_t max_stack; //0xE8
//    uint32_t spawn_stack; //0xEC
//    uint32_t gem_offset; //0xF0
//    uint16_t name_str; //0xF4
//    uint16_t version; //0xF6
//    uint16_t auto_prefix; //0xF8
//    uint16_t missile_type; //0xFA
//    uint8_t rarity; //0xFC
//    uint8_t level; //0xFD
//    uint8_t min_damage; //0xFE
//    uint8_t max_damage; //0xFF
//    uint8_t min_misdamage; //0x100
//    uint8_t max_misdamage; //0x101
//    uint8_t two_hand_min_damage; //0x102
//    uint8_t two_hand_max_damage; //0x103
//    uint16_t range_adder; //0x104
//    uint16_t str_bonus; //0x106
//    uint16_t dex_bonus; //0x108
//    uint16_t req_str; //0x10A
//    uint16_t req_dex; //0x10C
//    uint8_t absorbs; //0x10E
//    uint8_t inv_width; //0x10F
//    uint8_t inv_height; //0x110
//    uint8_t block; //0x111
//    uint8_t durability; //0x112
//    uint8_t no_durability; //0x11
//    uint8_t missile; //0x114
//    uint8_t component; //0x115
//    uint8_t right_arm; //0x116
//    uint8_t left_arm; //0x117
//    uint8_t torso; //0x118
//    uint8_t legs; //0x119
//    uint8_t right_spad; //0x11A
//    uint8_t left_spad; //0x11B
//    uint8_t two_handed; //0x11C
//    uint8_t useable; //0x11D
//    uint16_t type; //0x11E
//    uint16_t type2; //0x120
//    uint16_t subtype; //0x122
//    uint16_t drop_sound; //0x124
//    uint16_t use_sound; //0x126
//    uint8_t drop_sfx_frame; //0x128
//    uint8_t unique; //0x129
//    uint8_t quest; //0x12A
//    uint8_t quest_diff_check; //0x12B
//    uint8_t transparent; //0x12C
//    uint8_t trans_tbl; //0x12D
//    uint8_t pad0_x12_e; //0x12E
//    uint8_t light_radius; //0x12F
//    uint8_t belt; //0x130
//    uint8_t auto_belt; //0x131
//    uint8_t stackable; //0x132
//    uint8_t spawnable; //0x133
//    uint8_t spell_icon; //0x134
//    uint8_t dur_warning; //0x135
//    uint8_t quantity_warning; //0x136
//    uint8_t has_inv; //0x137
//    uint8_t gem_sockets; //0x138
//    uint8_t transmogrify; //0x139
//    uint8_t transmog_min; //0x13A
//    uint8_t transmog_max; //0x13B
//    uint8_t hit_class; //0x13C
//    uint8_t one_or_two_handed; //0x13D
//    uint8_t gem_apply_type; //0x13E
//    uint8_t level_req; //0x13F
//    uint8_t magic_lvl; //0x140
//    uint8_t transform; //0x141
//    uint8_t inv_trans; //0x142
//    uint8_t compact_save; //0x143
//    uint8_t skip_name; //0x144
//    uint8_t nameable; //0x145
//    uint8_t akara_min; //0x146
//    uint8_t gheed_min; //0x147
//    uint8_t charsi_min; //0x148
//    uint8_t fara_min; //0x149
//    uint8_t lysander_min; //0x14A
//    uint8_t drognan_min; //0x14B
//    uint8_t hralti_min; //0x14C
//    uint8_t alkor_min; //0x14D
//    uint8_t ormus_min; //0x14E
//    uint8_t elzix_min; //0x14F
//    uint8_t asheara_min; //0x150
//    uint8_t cain_min; //0x151
//    uint8_t halbu_min; //0x152
//    uint8_t jamella_min; //0x153
//    uint8_t malah_min; //0x154
//    uint8_t larzuk_min; //0x155
//    uint8_t drehya_min; //0x156
//    uint8_t akara_max; //0x157
//    uint8_t gheed_max; //0x158
//    uint8_t charsi_max; //0x159
//    uint8_t fara_max; //0x15A
//    uint8_t lysander_max; //0x15B
//    uint8_t drognan_max; //0x15C
//    uint8_t hralti_max; //0x15D
//    uint8_t alkor_max; //0x15E
//    uint8_t ormus_max; //0x15F
//    uint8_t elzix_max; //0x160
//    uint8_t asheara_max; //0x161
//    uint8_t cain_max; //0x162
//    uint8_t halbu_max; //0x163
//    uint8_t jamella_max; //0x164
//    uint8_t malah_max; //0x165
//    uint8_t larzuk_max; //0x166
//    uint8_t drehya_max; //0x167
//    uint8_t akara_magic_min; //0x168
//    uint8_t gheed_magic_min; //0x169
//    uint8_t charsi_magic_min; //0x16A
//    uint8_t fara_magic_min; //0x16B
//    uint8_t lysander_magic_min; //0x16C
//    uint8_t drognan_magic_min; //0x16D
//    uint8_t hralti_magic_min; //0x16E
//    uint8_t alkor_magic_min; //0x16F
//    uint8_t ormus_magic_min; //0x170
//    uint8_t elzix_magic_min; //0x171
//    uint8_t asheara_magic_min; //0x172
//    uint8_t cain_magic_min; //0x173
//    uint8_t halbu_magic_min; //0x174
//    uint8_t jamella_magic_min; //0x175
//    uint8_t malah_magic_min; //0x176
//    uint8_t larzuk_magic_min; //0x177
//    uint8_t drehya_magic_min; //0x178
//    uint8_t akara_magic_max; //0x179
//    uint8_t gheed_magic_max; //0x17A
//    uint8_t charsi_magic_max; //0x17B
//    uint8_t fara_magic_max; //0x17C
//    uint8_t lysander_magic_max; //0x17D
//    uint8_t drognan_magic_max; //0x17E
//    uint8_t hralti_magic_max; //0x17F
//    uint8_t alkor_magic_max; //0x180
//    uint8_t ormus_magic_max; //0x181
//    uint8_t elzix_magic_max; //0x182
//    uint8_t asheara_magic_max; //0x183
//    uint8_t cain_magic_max; //0x184
//    uint8_t halbu_magic_max; //0x185
//    uint8_t jamella_magic_max; //0x186
//    uint8_t malah_magic_max; //0x187
//    uint8_t larzuk_magic_max; //0x188
//    uint8_t drehya_magic_max; //0x189
//    uint8_t akara_magic_lvl; //0x18A
//    uint8_t gheed_magic_lvl; //0x18B
//    uint8_t charsi_magic_lvl; //0x18C
//    uint8_t fara_magic_lvl; //0x18D
//    uint8_t lysander_magic_lvl; //0x18E
//    uint8_t drognan_magic_lvl; //0x18F
//    uint8_t hralti_magic_lvl; //0x190
//    uint8_t alkor_magic_lvl; //0x191
//    uint8_t ormus_magic_lvl; //0x192
//    uint8_t elzix_magic_lvl; //0x193
//    uint8_t asheara_magic_lvl; //0x194
//    uint8_t cain_magic_lvl; //0x195
//    uint8_t halbu_magic_lvl; //0x196
//    uint8_t jamella_magic_lvl; //0x197
//    uint8_t malah_magic_lvl; //0x198
//    uint8_t larzuk_magic_lvl; //0x199
//    uint8_t drehya_magic_lvl; //0x19A
//    uint8_t pad0_x19_b; //0x19B
//    uint32_t nightmare_upgrade; //0x19C
//    uint32_t hell_upgrade; //0x1A0
//    uint8_t perm_store_item; //0x1A4
//    uint8_t multi_buy; //0x1A5
//    uint16_t pad0_x1_a6; //0x1A6
//};
//
//struct item_types_line {
//    char code[4];                        //0x00
//    int16_t equiv1;                        //0x04
//    int16_t equiv2;                        //0x06
//    int8_t repair;                        //0x08
//    int8_t body;                            //0x09
//    int8_t body_loc1;                        //0x0A
//    int8_t body_loc2;                        //0x0B
//    int16_t shoots;                        //0x0C
//    int16_t quiver;                        //0x0E
//    int8_t throwable;                    //0x10
//    int8_t reload;                        //0x11
//    int8_t re_equip;                        //0x12
//    int8_t auto_stack;                    //0x13
//    int8_t magic;                        //0x14
//    int8_t rare;                            //0x15
//    int8_t normal;                        //0x16
//    int8_t charm;                        //0x17
//    int8_t gem;                            //0x18
//    int8_t beltable;                        //0x19
//    int8_t max_sock1;                        //0x1A
//    int8_t max_sock25;                    //0x1B
//    int8_t max_sock40;                    //0x1C
//    int8_t treasure_class;                //0x1D
//    int8_t rarity;                        //0x1E
//    int8_t staff_mods;                    //0x1F
//    int8_t cost_formula;                    //0x20
//    int8_t item_class;                        //0x21
//    int8_t store_page;                    //0x22
//    int8_t var_inv_gfx;                    //0x23
//    char inv_gfx1[32];                    //0x24
//    char inv_gfx2[32];                    //0x44
//    char inv_gfx3[32];                    //0x64
//    char inv_gfx4[32];                    //0x84
//    char inv_gfx5[32];                    //0xA4
//    char inv_gfx6[32];                    //0xC4
//};
//
//
//enum unit_type_t : DWORD  {
//    UNIT_TYPE_PLAYER = 0,
//    UNIT_TYPE_MONSTER = 1,
//    UNIT_TYPE_OBJECT = 2,
//    UNIT_TYPE_MISSILE = 3,
//    UNIT_TYPE_ITEM = 4,
//    UNIT_TYPE_VIS_TILE = 5 //unused?
//};
//
//enum player_class_t : DWORD  {
//    PLAYER_CLASS_AMAZON = 0,
//    PLAYER_CLASS_SORCERESS = 1,
//    PLAYER_CLASS_NECROMANCER = 2,
//    PLAYER_CLASS_PALADIN = 3,
//    PLAYER_CLASS_BARBARIAN = 4,
//    PLAYER_CLASS_DRUID = 5,
//    PLAYER_CLASS_ASSASSIN = 6
//};
//
//struct unit3 {
//    unit_type_t type;
//
//    union {
//        player_class_t player_class;
//        DWORD data_record_index;
//    };
//
//    void* memory_pool;
//    DWORD guid;
//    DWORD mode;
//
//    union {
//        DWORD* player_data;
//        DWORD* monster_data;
//        DWORD* object_data;
//        //missile_data* missile_data;
//        DWORD* item_data;
//    };
//
//    BYTE act;
//    BYTE act_padding[0x03];
//    DWORD* drlg_act;
//
//    struct {
//        DWORD low_seed;
//        DWORD high_seed;
//    } seed;
//    DWORD init_seed;
//
//    DWORD* path;
//
//    DWORD* skill_sequence;
//    DWORD skill_sequence_frame_count;
//    DWORD skill_sequence_frame;
//
//    DWORD anim_speed;
//    DWORD skill_sequence_mode;
//
//    DWORD current_frame;
//    DWORD frame_count;
//    WORD anim_speed_w;
//
//    BYTE action_frame;
//    BYTE pad1;
//
//    DWORD* anim_data;
//
//    DWORD* gfxdata;
//    DWORD* gfxdata_copy;
//
//    DWORD* statslistex;
//    DWORD* inventory;
//
//    union {
//        struct {
//            DWORD interact_guid;
//            DWORD interact_type;
//            BYTE interacting;
//        } interaction;
//
//        struct {
//            void* light_map;
//            DWORD start_light_radius;
//            WORD p12_shift_index;
//        } lightning;
//    };
//
//    WORD update_type;
//    unit3* update_unit;
//
//    DWORD* quest_record;
//    DWORD sparky_chest;
//    void* timer_args;
//
//    union {
//        DWORD* game;
//        DWORD sound_sync;
//    };
//
//    char pad2[0x0C];
//
//    void* event;
//
//    unit_type_t owner_type;
//    DWORD owner_guid;
//
//    char pad3[8];
//
//    char* hover_text;
//
//    void* skills;
//
//    void* combat;
//    DWORD hit_class;
//
//    char pad4[4];
//
//    DWORD drop_code;
//
//    char pad5[8];
//
//    union {
//        struct {
//            DWORD unit_flags;
//            DWORD unit_flags_ex;
//        } flags;
//        unsigned long long flags64;
//    };
//
//    char pad6[4];
//
//    DWORD node_index;
//    DWORD get_tick_count;
//
//    union {
//        DWORD get_tick_count2;
//        void* particle_stream;
//    };
//
//    void* timer;
//
//    unit3* change_next_unit; //?
//    unit3* prev_unit;
//    unit3* prev_unit_in_room;
//
//    void* msg_first;
//    void* msg_last;
//
//    bool is_hireling() const {
//        if (type != unit_type_t::UNIT_TYPE_MONSTER)
//            return false;
//
//        return (flags.unit_flags & 0x00000200) == 0x00000200;
//    }
//
//    bool is_pet() const {
//        if (type != unit_type_t::UNIT_TYPE_MONSTER)
//            return false;
//
//        return (flags.unit_flags & 0x80000000) == 0x80000000;
//    }
//};
//
//struct room3//size=0x80 {
//    //ptRoom +48 0 = spawn new units (monster, objects e.tc), 1 = don't spawn any new units
//    DWORD    seed1;                        //+00
//    DWORD    seed2;                        //+04
//    BYTE    uk8[0x1C];                    //+08
//    room3* pt_near_rooms;                //+24
//    DWORD    nb_near_rooms;                //+28
//    unit3* unit;                        //+2C
//    BYTE    uk5[0x44];                    //+30
//    room3* pt_next_room;                    //+74
//    union {
//        BYTE    flags;                    //+78
//        struct {
//            BYTE is_generated : 1;
//            BYTE is_generated2 : 1;//???
//        };
//    };
//};
//
//
//struct Level //size=0x234 {
//    DWORD        type;                //+000
//    DWORD        flags;                //+004
//    DWORD        __004_010[2];        //+008
//    RoomEx*        ptFirstRoomEx;        //+010
//    DWORD        __014_01C[2];        //+014
//    INT32        posX;                //+01C
//    INT32        posY;                //+020
//    INT32        sizeX;                //+024
//    INT32        sizeY;                //+028
//    DWORD        __0C2_1AC[96];        //+02C
//    Level*        ptNextLevel;        //+1AC
//    DWORD        __1B0;                //+1B0
//    ActData*    ptActData;            //+1B4
//    DWORD        __1B8_1C0[2];        //+1B8
//    DWORD        levelType;            //+1C0
//    DWORD        seed1;                //+1C4
//    DWORD        seed2;                //+1C8
//    DWORD        uk_1CC;                //+1CC
//    UINT32        levelNo;            //+1D0
//    DWORD        __1D4_234[96];        //+1D4
//};
//
//struct ActData //size=0x488 {
//    DWORD        seed1;                //+000
//    DWORD        seed2;                //+004
//    DWORD        nbRooms;            //+008
//    DWORD        __00C_0094[34];        //+00C
//    DWORD        nTalRashaTombLevel;    //094
//    DWORD        __098;                //+098
//    Game*        ptGame;                //+09C
//    DWORD        __0A0_450[237];        //+0A0
//    BYTE        difficulty;            //+450
//    BYTE        __451_46C[27];        //+451
//    ActMap*        ptActMap;            //+46C
//    DWORD        __470_484[5];        //+470
//    DWORD        nBossMummyTombLevel;//+484
//};
//
//struct ActMap //size=0x60 {
//    DWORD        isNotManaged;        //+00
//    DWORD        __04;                //+04
//    DWORD        townLevel;            //+08
//    DWORD        seed;                //+0C
//    Room*        ptFirstRoom;        //+10
//    DWORD        actNumber;            //+14
//    DWORD        __18_48[12];        //+18
//    ActData*    ptActData;            //+48
//    DWORD        __50_5C[4];            //+4C
//    void*        ptMemoryPool;        //+5C
//};
//
////ptGame : 04E4007C
//struct Game {                                        //Offset from Code.
//    BYTE    uk1[0x18];                    //+00
//    DWORD    _ptLock;                    //+18 Unknown
//    DWORD    memoryPool;                    //+1C Memory Pool (??)
//    BYTE    uk2[0x4D];                    //+20
//    BYTE    difficultyLevel;            //+6D (Difficulty 0,1 or 2)
//    WORD    unknown1;                    //+6E Cube puts 4 here
//    DWORD    isLODGame;                    //+70 (D2=0 LOD =1) (DWORD ?)
//    BYTE    uk3[0x04];                    //+71
//    WORD    unknown2;                    //+78
//    BYTE    uk4[0x0E];                    //+7A
//    NetClient*    ptClient;                //+88
//    BYTE    __8C[0x1C];                    //+8C
//    DWORD    gameFrame;                    //+A8
//    BYTE    __AC[0x10];                    //+AC
//    ActMap*    mapAct[5];                    //+BC
//    BYTE    ukD0[0x1024];                //+D0
//    DWORD*    game10F4;                    //+10F4
//    BYTE    uk6[0x28];                    //+10F8
//    Unit*    units[0xA00];                //+1120
//    Unit*    roomtitles[0x200];            //+1B20
//};
////WORD ptGame+28 game ID ?
//
//
//
//struct Path //(8 dword) {                                        //Offset from Code.        Size: 20
//    WORD    uk1;                        //+00
//    WORD    mapx;                        //+02
//    WORD    uk2;                        //+04
//    WORD    mapy;                        //+06
//    DWORD    uk3;                        //+08
//    DWORD    x;                            //+0C
//    DWORD    y;                            //+10
//    DWORD    uk6;                        //+14
//    DWORD    uk7;                        //+18
//    Room*    ptRoom;                        //+1C
//};
//
//
//struct Inventory {                                        //Offset from Code.        Size: 30 to 40
//    DWORD    tag;                        //+00    = 0x01020304
//    BYTE    uk1[0x04];                    //+04    =? 0
//    Unit*    ptChar;                        //+08
//    Unit*    ptItem;                        //+0C
//    BYTE    uk2[0x10];                    //+10
//    Unit*    inventory1;                    //+20
//    BYTE    uk3[0x04];                    //+24
//    DWORD    currentUsedSocket;            //+28 //Kingpin : a variable to know how many sockets that have added to item
//    DWORD    Inventory2C;                //+2C //one value
//    DWORD    Inventory30;                //+30
//    void*    ptCorpse;                    //+34
//    BYTE    uk5[0x04];                    //+38
//    DWORD    nextCorpseId;                //+3C //max = 15
//    BYTE    uk6[0x04];                    //+40
//};
//
//struct D2Stat {
//    WORD    index;
//    WORD    id;
//    int        value;
//};
//
//
//struct Stats                //sizeof(Stats)=0x64 {
//    DWORD    nUnitId;                //+00
//    Unit*    ptUnit;                    //+04
//    DWORD    nUnitType;                //+08
//    DWORD    nItemNum;                //+0C
//    union
//    {
//        DWORD    flags;                //+10
//        struct
//        {
//            DWORD fuk1:13;            //0x00001FFF
//            DWORD isDisabled:1;        //0x00002000
//            DWORD fuk2:17;            //0x7FFFC000
//            DWORD dontUseBaseValue:1;//0x80000000
//        };
//    };
//    DWORD    id;                        //+14
//    DWORD    uk18;                    //+18
//    BYTE    uk2[0x08];                //+1C
//    D2Stat*    ptBaseStatsTable;        //+24
//    WORD    nbBaseStats;            //+28
//    WORD    sizeOfBaseStatsTable;    //+2A ??
//    Stats*    ptStats;                //+2C
//    BYTE    uk3[0x04];                //+30
//    Stats*    ptItemStats;            //+34
//    BYTE    uk4[0x04];                //+38
//    Stats*    ptAffixStats;            //+3C
//    Stats*    ptNextStats2;            //+40
//    union
//    {
//        Unit*    ptChar;                //+44
//        Unit*    ptItem;
//    };
//    D2Stat*    ptStatsTable;            //+48
//    WORD    nbStats;                //+4C
//    WORD    sizeOfStatsTable;        //+4E ??
//    BYTE    uk5[0x8];                //+50
//    BYTE*    unknow0;                //+58 (sizeof(*unknow0)=0x30 (calculated)
//    DWORD    unknow1;                //+5C (=0)
//    DWORD    unknow2;                //+60 (=0)
//};
//
//struct AIControl {
//    DWORD    specialState;        // +00 SpecialState - stuff like terror, confusion goes here
//    void*    aiFunction;            // +04 fpAiFunction(); - the primary ai function to call (void * __fastcall)(pGame,pUnit,pAiTickArgs);
//    DWORD    aiFlags;            // +08 AiFlags
//    DWORD    ownerGUID;            // +0C OwnerGUID - the global unique identifier of the boss or minion owner
//    DWORD    ownerType;            // +10 eOwnerType - the unit type of the boss or minion owner
//    DWORD    args[3];            // +14 dwArgs[3] - three dwords holding custom data used by ai func to store counters (etc)
//    void*    cmdCurrent;            // +20 pCmdCurrents
//    void*    cmdLast;            // +24 pCmdLast
//    Game*    ptGame;                // +28 pGame
//    DWORD    ownerGUID2;            // +2C OwnerGUID - the same as +008
//    DWORD    ownerType2;            // +30 eOwnerType - the same as +00C
//    void*    minionList;            // +34 pMinionList - list of all minions, for boss units (SetBoss in MonStats, Unique, SuperUnique etc)
//    DWORD    trapNo;                // +3C eTrapNo - used by shadows for summoning traps (so they stick to one type usually)
//};
//
//struct MonsterData                    // sizeof(MonsterData)=0x60 {                                        //Offset from Code.
//    MonStatsBIN*    ptMonStats;            //+000 - pMonStats - record in monstats.txt
//    BYTE            components[0x10];    //+004 - Components[16] - bytes holding the component Ids for each component; Order: HD, TR, LG, RA, LA, RH, LH, SH, S1, S2, S3, S4, S5, S6, S7, S8
//    WORD            seed;                //+014 - NameSeed
//    union
//    {
//        BYTE        flags;                //+016 - TypeFlags
//        struct
//        {
//            BYTE    isOther:1;            // MONTYPE_OTHER(set for some champs, uniques)
//            BYTE    isSuperUnique:1;    // MONTYPE_SUPERUNIQUE
//            BYTE    isChampion:1;        // MONTYPE_CHAMPION
//            BYTE    isUnique:1;            // MONTYPE_UNIQUE
//            BYTE    isMinion:1;            // MONTYPE_MINION
//            BYTE    isPossessed:1;        // MONTYPE_POSSESSED
//            BYTE    isGhostly:1;        // MONTYPE_GHOSTLY
//            BYTE    isMultishot:1;        // MONTYPE_MULTISHOT
//        };
//    };
//    BYTE            lastMode;            //+017 - eLastMode
//    DWORD            duriel;                //+018 - dwDuriel - set only for duriel
//    BYTE            monUModList[10];    //+01C - MonUModList[9] - nine bytes holding the Ids for each MonUMod assigned to the unit
//    DWORD            superUniqueID;        //+026
//    AIControl*        ptAiGeneral;        //+02A
//    BYTE            uk2[0x32];            //+02E
//};
//
//struct ObjectData2 {
//    ObjectsBIN*    ptObjectsBIN;
//    BYTE        levelID;
//};
///*
//Kingpin: struct MonsterData {
//     BYTE     uk1[0x13];          // +00
//     DWORD     Flags;               // +14
//     DWORD     uk2;               // +18
//     DWORD     MinionOwnerID;     // +1C
//     BYTE     uk3[0x5];          // +20
//     WORD     SuperUniqueID;     // +26 Superunique ID of monster
//     D2Game*     ptGame;               // +28 ptGame
//};
//*/
//
//#define F2_ITEM_IS_IDENTIFIED 0x10 //or F_IS_IDENTIFIED ?
//
//struct ItemData//size=0x74 {                                        //Offset from Code.
//    DWORD    quality;                    //+00
//    DWORD    seedLow;                    //+04
//    DWORD    seedHi;                        //+08
//    DWORD    playerID;                    //+0C #10734 / #10735 (PCInventory->ptPlayer->0C)
//    DWORD    seedStarting;                //+10
//    DWORD    flags1;                        //+14
//    union {
//        DWORD    flags2;                    //+18
//        struct {
//            DWORD    fuk1:1;                //0x00000001
//            DWORD    isIndentified:1;    //0x00000002
//            DWORD    fuk2:2;                //0x0000000C
//            DWORD    isUnindentified:1;    //0x00000010
//            DWORD    fuk3:3;                //0x000000E0
//            DWORD    isBroken:1;            //0x00000100
//            DWORD    fuk4:2;                //0x00000600
//            DWORD    isSocketed:1;        //0x00000800
//            DWORD    fuk5:10;            //0x003FF000
//            DWORD    isEtheral:1;        //0x00400000
//            DWORD    fuk6:3;                //0x03800000
//            DWORD    isRuneword:1;        //0x04000000
//            DWORD    fuk7:1;                //0x08000000
//            DWORD    isPersonalized:1;    //0x10000000
//            DWORD    fuk8:3;                //0xE0000000
//        };
//    };
///*
//ITEMFLAG_NEWITEM               = 0x00000001,
//ITEMFLAG_TAGETING               = 0x00000004,
//ITEMFLAG_UNIDENTIFIED               = 0x00000010,
//ITEMFLAG_QUANTITY               = 0x00000020,
//ITEMFLAG_Durability               = 0x00000100,
//ITEMFLAG_UNKNOWN2               = 0x00000400,
//ITEMFLAG_SOCKETED               = 0x00000800,
//ITEMFLAG_NON_SELLABLE               = 0x00001000,
//ITEMFLAG_NEWITEM2               = 0x00002000,
//ITEMFLAG_UNKNOWN3               = 0x00004000,
//ITEMFLAG_CHECKSECPRICE               = 0x00010000,
//ITEMFLAG_CHECKGAMBLEPRICE          = 0x00020000,
//ITEMFLAG_UNKNOWN4               = 0x00080000,
//ITEMFLAG_INDESTRUCTIBLE(ETHERAL) = 0x00400000,
//ITEMFLAG_UNKNOWN5               = 0x00800000,
//ITEMFLAG_FROMPLAYER               = 0x01000000,
//ITEMFLAG_RUNEWORD               = 0x04000000
//*/
//    DWORD    guid1;                        //+1C Global Unique ID 1
//    DWORD    guid2;                        //+20 Global Unique ID 2
//    DWORD    guid3;                        //+24 Global Unique ID 3
//    DWORD    uniqueID;                    //+28
//    BYTE    ilvl;                        //+2C
//    BYTE    uk1[0x03];                    //+2D
//    WORD    version;                    //+30
//    WORD    rarePrefix;                    //+32
//    WORD    rareSuffix;                    //+34
//    WORD    autoPref;                    //+36
//    WORD    prefix[3];                    //+38
//    WORD    suffix[3];                    //+3E
//    BYTE    equipLoc;                    //+44
//    /*    location if equipped
//    *    00 = noequip/inBelt
//    *   01 = head
//    *    02 = neck
//    *    03 = tors
//    *    04 = rarm
//    *    05 = larm
//    *    06 = lrin
//    *    07 = rrin
//    *    08 = belt
//    *    09 = feet
//    *    0A = glov
//    *    0B = ralt
//    *    0C = lalt
//*/
//    BYTE    page;                        //+45
//    /*    page dans laquel se trouve l'item
//    *    FF = mouse/equip/onEarth
//    *    00 = inventory
//    *   01 = cube
//    *    04 = stash
//    */
//    BYTE    uk4[0x01];                    //+46
//    BYTE    ItemData3;                    //+47 //D2Common10854 D2Common10853
//    BYTE    pEarLevel;                    //+48
//    BYTE    varGfx;                        //+49
//    char    IName[0x12];                //+4A //inscribed/ear name
//    Inventory*    ptInventory;            //+5C
//    Unit*    ptPrevItem;                    //+60
//    Unit*    ptNextItem;                    //+64
//    BYTE    uk8[0x01];                    //+68
//    BYTE    ItemData2;                    //+69
//    BYTE    uk9[0x0A];                    //+6A
//};
//
//
//struct NetClient {
//    DWORD    clientID;                    //+00
//    BYTE    uk1[0x06];                    //+04
//    union {                                //+0A
//        WORD    flag;
//        struct {
//            WORD f1:1;
//            WORD f2:1;
//            WORD isHardCoreGame:1;
//        };
//    };
//    BYTE    uk2;                        //+0C
//    char    name[0x10];                    //+0D
//    BYTE    uk3[0x15F];                    //+1D
//    BYTE*    savefile;                    //+17C
//    DWORD    finalSize;                    //+180
//    DWORD    counter;                    //+184
//    DWORD    currentSize;                //+188
//    BYTE    uk4[0x1C];                    //+18C
//    Game*    ptGame;                        //+1A8
//    //+1A8 is ptGame
//    //+4A8 ptNextClient
///*
//Kingpin: ptPclient
//+16C is either act or unit type
//+170 Active UniqueID for player
//+174 Active ptPlayer on Client
//+1a8 ptGame
//+1b4 Current or next ptRoom
//*/
//};
///*
//struct PlayerData {
//    char        name[0x10];                //+00 (size min 0x08 max 0x11)
//    BYTE        uk1[0x8C];                //+10
//    NetClient*    ptNetClient;            //+9C
//};
//*/
//struct PlayerData {
//    char        name[0x10];                //+00    Player Name
//    void*        ptQuest[3];                //+10    Quest Pointers for each difficulty
//    BYTE        uk1[0x18];                //+1C        //before : 0x14
//    void*        ptArenaUnit;            //+34    ptArena for the Unit
//    BYTE        uk2[0x4];                //+38        //before : 0x7
//    WORD        MPSourcePortalUniqueID;    //+3C    Source Portal Unique_ID
//    BYTE        uk3[0x2];                //+3E
//    WORD        MPDestPortalUniqueID;    //+40    Destination Portal Unique_ID
//    BYTE        uk4[0x06];                //+42
//    BYTE        ptObjectUnID;            //+48    Object UniqueID for TownPortals
//    BYTE        uk5[0x53];                //+49
//    NetClient*    ptNetClient;            //+9C    ptClient
//};
//
//
//#define F1_IS_SPAWNED 0x10 //or F_IS_IDENTIFIED ?
//#define F2_IS_LOD 0x2000000
//struct Unit {                                        //Offset from Code.        Size: 0xF4+4
//    DWORD        nUnitType;                //+00
//    union {
//        DWORD            nPlayerClass;
//        DWORD            nTxtFileNo;
//    };                                    //+04
//    DWORD        nUnitId;                //+08
//    DWORD        nItemNum;                //+0C
//    DWORD        mode;                    //+10
//    union {
//        MonsterData* ptMonsterData;
//        DWORD* ptObjectData;
//        ItemData* ptItemData;
//        PlayerData* ptPlayerData;
//    };                                    //+14
//    BYTE        act;                    //+18
//    BYTE        uk12[3];                //+19
//    void* ptAct;                    //+1C
//    DWORD        SeedLow;                //+20
//    DWORD        SeedHi;                    //+24
//    DWORD        SeedStarting;            //+28
//    Path* path;                    //+2C
//    BYTE        uk1[0x08];                //+30
//    Path* ptPos;                    //+38
//    DWORD        animSpeed;                //+3C
//    BYTE        uk2[0x04];                //+40
//    DWORD        curFrame;                //+44
//    DWORD        remainingFrame;            //+48
//    WORD        animSpeedW;                //+4C
//    BYTE        actionFlag;                //+4E
//    BYTE        uk3[0x1];                //+4F
//    void* ptAnimData;                //+50
//    BYTE        uk4[0x08];                //+54
//    Stats* ptStats;                //+5C
//    Inventory* ptInventory;            //+60
//    BYTE        uk5[0x1C];                //+64
//    Game* ptGame;                    //+80
//    BYTE        uk6[0x24];                //+84
//    Skills* ptSkills;                //+A8
//    void* ptCombatData;            //+AC
//    BYTE        uk7[0x08];                //+B0
//    DWORD        itemCode;                //+B8 for an item being dropped by this unit
//    BYTE        uk8[0x08];                //+BC
//    DWORD        flags1;                    //+C4
//    union {
//        DWORD        flags2;                //+C8    //flags
//        struct {
//            DWORD    ukf1 : 25;
//            DWORD    isLod : 1;
//        };
//    };
//    BYTE        uk9[0x04];                //+CC
//    DWORD        clientId;                //+D0
//    BYTE        uk10[0x0C];                //+D4
//    Unit* ptFirstMonster;            //+E0
//    Unit* Unit1;                    //+E4
//    Unit* ptNextUnitInRoom;        //+E8
//    BYTE        uk11[0x08];                //+EC
//    union {
//        CBPlayerData* ptCBPlayerData;
//        CBItemData* ptCBItemData;
//        void* ptCBData;
//    };                                    //+F4
//};
//
//
//
///*
//struct D2Game {
//    DWORD unknown[19];
//    WORD  difficulty;
//};
//
//struct ItemMod {
//    DWORD mID;            // +00 Mod ID, get it from properties.txt
//    DWORD mParam;        // +04 First Property
//    DWORD mMinimum;        // +04 Second Property, Min Value
//    DWORD mMaximum;        // +04 Third Property, Max Value
//};
//
//struct AIStruct {
//    DWORD mSpawn;       // +00
//    void* mAIFunc;      // +04
//    DWORD mUnknown1[8]; // +08
//    D2Game* mGame;      // +28
//    DWORD mMinionOwner; // +2C
//    DWORD mOwnerType;   // +30
//};
//
//
//struct LocInfo {                       //Offset from Code
//    Unit* noneLoc;      //+00
//    Unit* headLoc;      //+04
//    Unit* neckLoc;      //+08
//    Unit* torsoLoc;     //+0c
//    Unit* rightArmLoc;  //+10
//    Unit* leftArmLoc;   //+14
//    Unit* rightRingLoc; //+18
//    Unit* leftRingLoc;  //+1c
//    Unit* beltLoc;      //+20
//    Unit* feetLoc;      //+24
//    Unit* glovesLoc;    //+28
//};
//
//struct ItemData {                        // Offset from Code
//    DWORD quality;       // +00.
//    DWORD lowSeed;       // +04.
//    DWORD highSeed;      // +08.
//    DWORD unknown1;      // +0c.
//    DWORD startLowSeed;  // +10.
//    DWORD cmdFlags;      // +14.
//    DWORD flags;         // +18.
//    DWORD unknown2[2];   // +1C
//    DWORD originID;      // +24.
//    DWORD iLvl;          // +28.
//    WORD  version;       // +2C.
//    WORD  prefix;        // +2E.
//    WORD  suffix;        // +30.
//    WORD  autoPrefix;    // +32.
//    WORD  prefix1;       // +34.
//    WORD  prefix2;       // +36.
//    WORD  prefix3;       // +38.
//    WORD  suffix1;       // +3A.
//    WORD  suffix2;       // +3C.
//    WORD  suffix3;       // +3E.
//    BYTE  bodyLoc;       // +40.    // 0=inv/stash 4=gaucheS 5=droiteS
//    BYTE  inPage;        // +41.    // FF=equip 00=inv 04=stash
//    BYTE  unknown3[3];   // +42
//    BYTE  picture[8];    // +45
//    DWORD ilvl;          // +4C
//};
//
//struct PresetUnit {                                   //Offset from Code
//    DWORD           unitType;       //+00
//    DWORD           txtFileNo;      //+04
//    DWORD           unknown1[1];    //Unkonwn Data at +08
//    DWORD           xPos;           //+0c
//    DWORD           yPos;           //+10
//    DWORD           unknown2[2];    //Unknown data at +14
//    PresetUnit*     pNext;          //+1c
//};
//
//struct MissileData {
//+04 flags
//+08 0 on start
//+0c -1 on start
//+20 nDirection
//+28 range?
//+2c range?
//+30 level
//+34 number of pierces (byte)
//+35 (byte)
//+36 (byte)
//+37 (byte)
//+38 range-activate
//};
//
//
//struct Combat {
//    DWORD mHeader;        // +00 : Always 0x32
//    DWORD mDisplayFlag;    // +04 : Affect critical hit etc ...
//    DWORD mPhysicalDmg; // +08 : Values are << 8
//    DWORD mUnknown1;    // +0C
//    DWORD mFireDmg;        // +10 : Values are << 8
//    DWORD mUnknown2;    // +14
//    DWORD mBurnLeng;    // +18
//    DWORD mLtngDmg;     // +1C : values are << 8
//    DWORD mMagDmg;      // +20 : values are << 8
//    DWORD mColdDmg;     // +24 : values are << 8
//    DWORD mPoisDmg;     // +28 : ??
//    DWORD mPoisLeng;    // +2C
//    DWORD mColdLeng;    // +30
//    DWORD mFreezeLeng;  // +34
//    DWORD mLifeSteal;   // +38
//    DWORD mManaSteal;   // +3C
//    DWORD mUnknwon3;    // +40
//    DWORD mStunLeng;    // +44
//    DWORD mUnknown4;    // +48
//    DWORD mTotalDamage; // +4C
//    DWORD mUnknown5[6]; // +50
//    DWORD mOverlay;     // +68
//    DWORD mUnknwon6;    // +6C
//};
//
//
//struct Node {                       //Offset from Code.
//    DWORD flag;         //+00 //Yoh : null->flag
//    Unit* ptItem;        //+04 //Yoh : ItemData=>Unit
//    DWORD unID;         //+08
//    DWORD page;         //+0c //Yoh plutot etat : inventory/stash=1; inv-belt=2; equip=3
//    Node* nextNode;     //+10
//};
//
//
//
//struct MonsterData                // size110 : 60 {
//    AIStruct*     AIPtr;
//    void*         unkonwPtr1;
//    void*         uniqueInfo;
//    void*         spawnInfo;
//    void*         unknownPtr2;
//    DWORD         unkown[8];
//    void*         lvlPtr;
//    MonStatTxt*   monstatPtr;
//};
//
//
//struct Unit {                               //Offset from Code.
//    DWORD   nUnitType;          //+00
//    union
//    {
//        DWORD   nTxtFileNo;
//        DWORD   nPlayerClass;
//    };                          //+04
//
//    DWORD   nUnitId;            //+08
//    DWORD   eMode;              //+0c
//    DWORD*  unknow1;            //+10
//    DWORD   nAct;               //+14
//    DWORD   pActRelated;        //+18
//    DWORD   unknown2[4];        //Unknown Data at +1C
//    D2Seed  nSeed;              //+2C
//    DWORD   startSeed;          //+34
//    Path*   pPos;               //+38
//    DWORD   unknown3[6];        //Unknown Data at +3C
//    DWORD   animSpeed;          //+54
//    DWORD   unknown32[2];       //+58
//    BYTE    COFString[12];      //+60
//    Stats*  ptStats;            //+6C
//
//    union
//    {
//        PlayerData*     pPlayerData;
//        MonsterData*    pMonsterData;
//        ItemData*       pItemData;
//        MissileData*    pMissile;
//        ObjectData*     pObjectData;
//    };                          //+70
//
//    DWORD   unknown5[2];        //+74
//    BYTE    unknown6[2];        //+7C
//    BYTE    rndDmg[2];          //+7e
//    DWORD   unknown7;           //+80
//    Inventory*  ptInventory;    //+84
//    DWORD   unknown8[4];        //+88
//    DWORD   startNull;          //+98
//    DWORD   unknown9[2];        //+9c
//    D2Game* ptGame;             //+a4
//    DWORD   unknown10[4];       //+a8
//    DWORD   nOwnerType;         //+b8
//    DWORD   nOwnerId;           //+bc
//    DWORD   unknown11[3];       //+c0
//    D2Info* ptInfo;             //+cc
//    Combat* ptCombat;           //+d0
//    DWORD   unknown12[5];       //+d4
//    DWORD   flags;              //+e8
//    DWORD   LODflag;            //+ec
//    DWORD   unknown13[7];       //+f0
//    Unit*   pNext;              //+10c
//    DWORD    unknown14[2];        //+110    //YOH
//    union
//    {
//        ItemData*       ptItemData;
//        void*            ptData;
//    };                          //+118    //YOH
//};
//*/
//
//struct DrlgRoom2;
//struct DrlgLevel;
//struct DrlgMisc;
//struct DrlgAct;
//struct UnitAny;
//struct AutomapLayer;
//struct AutomapCell;
//
//struct D2Seed {
//    DWORD d1,d2;
//};
//
//struct PresetUnit {
//    DWORD nUnitType; //+00
//    DWORD nTxtFileNo; //+04
//    DWORD _1[1];
//    DWORD xPos; //+0c
//    DWORD yPos; //+10
//    DWORD _2[2];
//    PresetUnit *pNext; //+1c
//};
//
//struct RoomTile {
//    DrlgRoom2 *pRoom2; //+00
//    DWORD *nNum; //+04
//    DWORD _1[3];
//    RoomTile *pNext; //+14
//};
//
//struct DrlgRoom1 {
//    DWORD xpos1; //+00
//    DWORD ypos1; //+04
//    DWORD xsize1; //+08
//    DWORD ysize1; //+0c
//    DWORD xpos2; //+10
//    DWORD ypos2; //+14
//    DWORD xsize2; //+18
//    DWORD ysize2; //+1c
//    DWORD _1[1];
//    DrlgRoom1 **paRoomsNear; //+24
//    DWORD nRoomsNear; //+28;
//    UnitAny *pUnitFirst; //+2c
//    DWORD _2[2];
//    DrlgRoom2 *pRoom2; //+38
//    D2Seed seed; //+3c
//    DWORD _3[14];
//    DrlgRoom1 *pNext; // +7c
//};
//
//struct DrlgRoom2 {
//    DrlgLevel *pDrlgLevel; //+00
//    DWORD xPos; //+04
//    DWORD yPos; //+08
//    DWORD xPos1; // +0c
//    DWORD yPos1; // +10
//    DWORD _1[2];
//    DWORD nPresetType; //+1c
//    DWORD *nPresetType2No; //+20
//    DWORD _2[39];
//    DrlgRoom2 **paRoomsNear; //+c0
//    DWORD nRoomsNear; //+c4
//    RoomTile *pRoomTiles; //+c8
//    PresetUnit *pPresetUnits; //+cc
//    DWORD _3[1];
//    D2Seed seed; //+d4
//    DWORD _3b[2];
//    DrlgRoom1 *pRoom1; //+e4
//    DrlgRoom2 *pNext; //+e8
//};
//
//struct DrlgLevel {
//    DrlgMisc *pDrlgMisc; //+00
//    DWORD nLevelNo; //+04
//    DWORD _1[10];
//    DrlgRoom2 *pRoom2First; //+30
//};
//
////struct DrlgLevel { //sizeof(DrlgLevel) = 0x230
////                                                    // +00 dwseed_l
////                                                    // +04 dwseed_h
////    DWORD _1[5];
////    DWORD nLevelNo; //+14
////    DWORD _1a[120];
////    // +138 rand_step(4)
////    // +1B0 rand_step(1)
////    // +1EC LevelType
////    D2Seed seed; //+1f8
////                                                    //+1FC    dwseed
////    DWORD _2[1];                        // +200 RoomCounter
////    DrlgRoom2* pRoom2First; //+204
////    DrlgMisc* pDrlgMisc; //+208
////                                                    //+20C    LevelFlag?
////                                                    //+21C    DrlgType
////    DWORD _3[8];
////    DrlgLevel* pLevelNext; //+22c
////};
//
//struct DrlgMisc {
//    DrlgLevel *pLevelFirst; //+00
//    DWORD _1[2];
//    DrlgAct *pDrlgAct; //+0c
//    DWORD _2[250];
//    DWORD nStaffTombLvl; // +3f8
//    DWORD nBossTombLvl; //+3fc
//};
//
//struct DrlgMisc_109 {
//    DrlgLevel *pLevelFirst; //+00
//    DWORD _1[2];
//    DrlgAct *pDrlgAct; //+0c
//    DWORD _2[249];
//    DWORD nStaffTombLvl; //+3f4
//    DWORD nBossTombLvl; //+3f8
//};
//
//struct DrlgAct {
//    DWORD _1;
//    DrlgRoom1 *pRoom1; //+04
//    DrlgMisc *pDrlgMisc; //+08
//};
//
//struct DrlgAct_109 {
//    DWORD _1[2];
//    DrlgMisc *pDrlgMisc; //+08
//};
//
//struct AutomapCell {
//    DWORD fSaved; //+00
//    WORD nCellNo; //+04
//    WORD xPixel; //+06
//    WORD yPixel; //+08
//    WORD _1[1];
//    AutomapCell *pLess; //+0c
//    AutomapCell *pMore; //+10
//};
//
//struct AutomapLayer {
//    DWORD nLayerNo;
//    DWORD fSaved;
//    AutomapCell *pFloors; //+08
//    AutomapCell *pWalls; //+0c
//    AutomapCell *pObjects; //+10
//    AutomapCell *pExtras; //+14
//};
//
//struct AutomapCellNode {
//    DWORD _1[4];
//    AutomapCell *pAutomapCell; // +10
//    DWORD _2;
//    AutomapCellNode *pNext; // +18
//};
//
//struct ObjectTxt {
//    char szName[0x40];    // +00
//    wchar_t wszName[0x40]; // +40
//    BYTE _1[4]; // +c0
//    BYTE nSelectable0; //+c4
//    BYTE _2[0x87];
//    BYTE nOrientation; //+14c
//    BYTE _2b[0x19];
//    BYTE nSubClass; // +166
//    BYTE _3[0x11];
//    BYTE nParm0; //+178
//    BYTE _4[0x39];
//    BYTE nPopulateFn; //+1b2
//    BYTE nOperateFn; //+1b3
//    BYTE _5[8];
//    DWORD nAutoMap; // +1bb
//};
//
//struct ObjectTxt_109 {
//    BYTE _1[0xc4];
//    BYTE nSelectable0; //+c4
//    BYTE _2[0x87];
//    BYTE nOrientation; //+14c
//    BYTE _2b[0x2a];
//    BYTE nSubClass; //+177
//    BYTE _3[0x10];
//    BYTE nParm0; //+188
//    BYTE _4[0x39];
//    BYTE nPopulateFn; //+1c2
//    BYTE nOperateFn; //+1c3
//    BYTE _5[8];
//    DWORD nAutoMap; //+1cc
//};
//
//struct ObjgroupTxt {
//    DWORD nId[8]; //+00
//    BYTE nDensity[8]; //+20
//    BYTE nProb[8]; //+28
//};
//
//struct LevelTxt {
//    BYTE nLevelNo;
//    BYTE nflag0;
//    BYTE nPal;
//    BYTE nAct;
//    BYTE _0[0xb]; //+04
//    WORD nMonLv[3];//+0F
//    WORD nMonLvEx[3];//+15 Area Level
//    BYTE _1[0xd8];
//    char cszName[40]; //+f5
//    char cszName2[40]; //+11d
//    char szLvlDesc[41]; //+145
//    wchar_t szName[40]; //+16e
//    wchar_t szName2[40]; //+16e
//    BYTE nObjGrp[8]; // unk
//    BYTE nObjPrb[8]; // unk
//    BYTE _1b[0x79];
//};
//
//struct LevelTxt_109 {
//    BYTE _1[0x18d];
//    BYTE nObjGrp[8]; //+18d
//    BYTE nObjPrb[8]; //+195
//    BYTE _1b[0x79];
//    wchar_t szName[40]; //+216
//};
//
//struct ItemTxt {
//    wchar_t szName2[0x40]; //+00
//    union {
//        DWORD dwCode; // +80
//        char szCode[4];
//    };
//    BYTE _2[0x70]; // +84
//    WORD nLocaleTxtNo; // +f4
//    BYTE _2a[0x28]; // +f6
//    BYTE nType; // +11e
//    BYTE _3[0x0d];
//    BYTE fQuest; // +12a
//};
//
//struct ItemTxt_109 {
//    char szName[0x40]; //+00
//    wchar_t szName2[0x40]; //+40
//    BYTE _1[0x84];
//    union {
//        DWORD dwCode; //+144
//        char szCode[4];
//    };
//    BYTE _2[0x6b];
//    BYTE nType; //+1b3
//    BYTE _3[4];
//    BYTE fQuest; //+1b8
//};
//
//struct MonsterTxt {
//    BYTE _1[0x6];
//    WORD nLocaleTxtNo; // +06
//    WORD flag; // +08
//    WORD _1a;
//    union {
//        DWORD flag1; // +0c
//        struct {
//            BYTE flag1a; // +0c
//            BYTE flag1b; // +0d
//            BYTE flag1c[2]; // +0e
//        };
//    };
//    BYTE _2[0x22]; // +10
//    WORD velocity; // +32
//    BYTE _2a[0x52]; // +34
//    WORD tcs[3][4]; //+86
//    BYTE _2b[0x52]; // +9e
//    wchar_t szDescriptor[0x3c]; //+f0
//    BYTE _3[0x1a0];
//};
//
//struct MonsterTxt_109 {
//    BYTE _1[0xf0];
//    wchar_t szDescriptor[0x3c]; //+f0
//    BYTE _2[0x1a0];
//    DWORD tcs[3][4]; //+308
//};
//
//struct SuperuniqueTxt {
//    WORD nIndex; //+00
//    WORD nLocaleTxtNo; //+02
//    DWORD _1[9]; //+04
//    BYTE _1a[4]; //+28
//    WORD tcs[4]; //+2c
//};
//
//struct SuperuniqueTxt_109 {
//    BYTE _1[0xd8];
//    DWORD tcs[3]; //+d8
//};
//
////struct PlayerData {
////    wchar_t szName[1]; //+00
////};
////
////
////struct MonsterData {
////    BYTE _1[0x16];
////    struct {
////        BYTE fUnk:1;
////        BYTE fNormal:1;
////        BYTE fChamp:1;
////        BYTE fBoss:1;
////        BYTE fMinion:1;
////    }; //+16
////    BYTE _1aa;
////    wchar_t szName[1]; //+18
////    BYTE _1a[2]; // +1a
////    BYTE anEnchants[9]; // +1c
////    BYTE _2a; // +25
////    WORD uniqueno; // +26
////    BYTE _2[50*4-14];
////    struct {
////        DWORD _3[1];
////        WORD uniqueno; //+04
////        WORD _4[1];
////        DWORD anEnchants[9]; //+08
////    } *pEnchants; //+cc
////};
//
//struct MonsterData_109 {
//    struct {
//        WORD fBoss:1;
//        WORD fMinion:1;
//        WORD fChamp:1;
//    }; //+00
//    wchar_t szName[1]; //+02
//    DWORD _2[50];
//    struct {
//        DWORD _3[1];
//        WORD uniqueno; //+04
//        WORD _4[1];
//        DWORD anEnchants[9]; //+08
//    } *pEnchants; //+cc
//};
//
////struct ItemData {
////    DWORD nQuality;
////    BYTE _1[0x14]; // +04
////    DWORD nFlag; // +18
////    BYTE _2[0x10]; // +1c
////    DWORD nItemLevel; // +2c
////    BYTE _3[0x14]; // +30
////    BYTE nLocation1; // +44                    // location on body = xpos of item (only if on body) (spread)
////    BYTE nLocation2; // +45                    // 0 = inventory, 4=stash (spread)
////};
//
//struct ItemData_109 {
//    DWORD nQuality;
//};
//
//struct UnitInventory {
//    DWORD nFlag; // +00
//    DWORD _1[2];
//    DWORD nItem; // +0c
//};
//
//struct ObjectPath {
//    DWORD xpos1; //+00
//    DWORD ypos1; //+04
//    DWORD xpos2; //+08
//    DWORD ypos2; //+0c
//    WORD targetx; //+10
//    WORD targety; //+12
//    DWORD _1[2];
//    DrlgRoom1 *pRoom1; //+1c
//    DWORD _2[14];
//    UnitAny *pTargetUnit; //+58
//};
//
//struct ObjectData1 {
//    ObjectTxt *pTxtFile; //+00 ??
//    union {
//        BYTE nShrineNo;
//        struct {
//            BYTE _1:7;
//            BYTE fChestLocked:1;
//        };
//    }; //+04
//};
//
//struct ItemPath {
//    void * ptRoom;                // 0x00
//    DWORD _1[2];                // 0x04
//    DWORD xpos;                // 0x0C
//    DWORD ypos;                // 0x10
//    DWORD _2[5];                // 0x14
//    BYTE * ptUnk;                // 0x28
//    DWORD _3[5];                // 0x2C
//};
//struct Info;
//struct StatsList;
//struct GameEvent;
//
//struct UnitAny {
//    DWORD nUnitType; //+00
//    DWORD nTxtFileNo; //+04
//    DWORD _1;
//    DWORD nUnitId; // +0C
//    DWORD eMode; // +10
//    union {
//        PlayerData *pPlayerData; //+14
//        MonsterData *pMonsterData; //+14
//        ObjectData1 *pObjectData; //+14
//        ItemData *pItemData; //+14
//    };
//    DWORD _2;
//    UnitAny *pNext; //+1c
//    WORD _2a0;
//    WORD unkNo;
//    DWORD _2a[2];
//    union {
//        ObjectPath *pPos;
//        ItemPath *pItemPath;
//    };
//    //struct {
//    //    DWORD xpos1; //+00
//    //    DWORD ypos1; //+04
//    //    DWORD xpos2; //+08
//    //    DWORD ypos2; //+0c
//    //    WORD targetx; //+10
//    //    WORD targety; //+12
//    //    DWORD _1[2];
//    //    DrlgRoom1 *pRoom1; //+1c
//    //    DWORD _2[14];
//    //    UnitAny *pTargetUnit; //+58
//    //} *pPos; // +2c
//    DWORD _2b[11];
//    StatsList * ptStatsList;         // 0x5C
//    UnitInventory *pInventory; //+60
//    DWORD _4b[12];
//    DWORD nOwnerType; // +94
//    DWORD nOwnerId; // +98
//    BYTE _4c[42];
//    BYTE  fFlags0:1; //+0c6, new added
//    BYTE _4d;
//    DWORD _5[10];
//    DWORD fFlags; //+e8
//    DWORD fFlags2; //+ec
//    DWORD _6[7];
//};
//
//
//struct UnitAny_109 {
//    DWORD nUnitType; //+00
//    DWORD nTxtFileNo; //+04
//    DWORD nUnitId; //+08
//    DWORD eMode; //+0c
//    DWORD _1[10];
//    struct {
//        DWORD xpos1; //+00
//        DWORD ypos1; //+04
//        DWORD xpos2; //+08
//        DWORD ypos2; //+0c
//        WORD targetx; //+10
//        WORD targety; //+12
//        DWORD _1[2];
//        DrlgRoom1 *pRoom1; //+1c
//        DWORD _2[14];
//        UnitAny *pTargetUnit; //+58
//    } *pPos; //+38
//    DWORD _2[13];
//    union {
//        PlayerData *pPlayerData; //+70
//        MonsterData *pMonsterData; //+70
//        ObjectData1 *pObjectData; //+70
//        ItemData *pItemData; //+70
//    };
//    DWORD _4a[4];
//    UnitInventory *pInventory; //+84
//    DWORD _4b[12];
//    DWORD nOwnerType; //+b8
//    DWORD nOwnerId; //+bc
//    DWORD _5[10];
//    DWORD fFlags; //+e8
//    DWORD fFlags2; //+ec
//    DWORD _6[7];
//    UnitAny *pNext; //+10c
//};
//
//struct GfxCell {
//    DWORD flags; //+00 ??
//    DWORD width; //+04
//    DWORD height; //+08
//    DWORD xoffs; //+0c
//    DWORD yoffs; //+10
//    DWORD _2[1]; //0x14
//    DWORD lpParent; //+18
//    DWORD length; //+1c
//    BYTE cols[1]; //+20
//};
//
//struct CellFile {
//    DWORD dwVersion; //+00
//    struct {
//        WORD dwFlags;
//        BYTE mylastcol;
//        BYTE mytabno:1;
//    }; //+04
//    DWORD eFormat; //+08
//    DWORD termination; //+0c
//    DWORD numdirs; //+10
//    DWORD numcells; //+14
//    GfxCell *cells[1]; //+18
//};
//
////struct CellContext {
////    GfxCell *hCell; //+00
////    CellFile *pCellFile; //+04
////    DWORD nCellNo; //+08
////    DWORD direction; //+0c ??
////    DWORD _1[14];
////};
//
//struct CellContext {
//    GfxCell* pCellInit;    //+00
//    CellFile* pCellFile;   //+04
//    DWORD nCellNo;                //+08 // nFrame
//    DWORD nDirection;            //+0C
//    int nMaxDirections;            //+10
//    int nMaxFrames;                //+14
//    DWORD fFlags;                //+18
//    BYTE fState;                //+1C
//    union
//    {
//        BYTE nComponent;            //+1D
//        BYTE fItemFlags;            //+1D
//    };
//    BYTE bUnk1E;                //+1E - padding no doubt
//    BYTE bUnk1F;                //+1F
//    int nUnitType;                //+20
//    int nUnitIndex;                //+24
//    int nMode;                    //+28
//    int nOverlay;                //+2C
//    union
//    {
//        // [token][component][type][mode][hitclass]
//        struct
//        {
//            DWORD dwToken;                //+30
//            DWORD dwComponent;            //+34
//            DWORD dwArmorType;            //+38 - lit, med , hvy
//            DWORD dwMode;                //+3C
//            DWORD dwHitClass;            //+40
//        };
//        DWORD dwName[5];                //+30
//    };
//    const char* pName;                //+44
//};
//
//struct TreasureClassItem {
//    DWORD _1[2];
//    DWORD thingno; //+08
//    struct {
//        DWORD _3:2;
//        DWORD tcflag:1;
//    }; //+0c
//    DWORD _2[3];
//};
//
//struct TreasureClass {
//    DWORD _1a;
//    DWORD numitems; //+04
//    DWORD _1[8];
//    TreasureClassItem *items; //+28
//};
//
//struct TreasureClass_109 {
//    DWORD numitems; //+00
//    DWORD _1[8];
//    TreasureClassItem *items; //+24
//};
//
//struct D2EditBox2 {
//    DWORD _1;
//    DWORD _2[6];
//    void (__fastcall *func)(D2EditBox2*); // +1c
//    DWORD _3[0x0F];
//    wchar_t* text;        // +5c
//};
//struct D2EditBox {
//    DWORD _1;
//    DWORD _2[6];
//    void (__fastcall *func)(D2EditBox*); // +1c
//    DWORD _3[0x0D];
//    DWORD    dwSelectStart; // +54
//    DWORD    dwSelectEnd; // +58
//    wchar_t text[0x100];        // +5c
//    DWORD    dwCursorPos;        // 0x25C
//};
//
//struct GameStructInfo {
//    DWORD _1[6];
//    WORD _1a;
//    char szGameName[0x18];
//    char szGameServerIp[0x56];
//    char szAccountName[0x30];
//    char szCharName[0x18];
//    char szRealmName[0x18];
//    BYTE _2[0x102];//+E8
//    //BYTE nGameMode; //+1EA
//    struct
//    {
//        BYTE nCharStat:2;
//        BYTE nHardcore:2;
//        BYTE _unk1:1;
//        BYTE nExpansion:1;
//        BYTE nLadder:2;
//    };//+1EA
//    BYTE nReadyAct; //+1EB  quest info
//    BYTE _3[0x3B];
//    char szServerVersion[24];//227  battle net gameversion
//    char szGamePassword[0x18];//+23F
//};
//
//struct D2MSG {                    // 0x20 bytes long
//    HWND hwnd;                // 0x00
//    DWORD message;                // 0x04
//    WPARAM wParam;                // 0x08
//    union {
//        DWORD lParam;
//        struct {
//            WORD xpos;
//            WORD ypos;
//        };
//    };
//    DWORD _1;                    // 0x10
//    HWND hwnd2;                // 0x14
//    DWORD _2[2];                // 0x18
//};
//
//struct FontInfo_t {
//    DWORD hFontHandle;
//    DWORD hMemHandle1;
//    DWORD hMemHandle2;
//    DWORD hMemHandle3;
//    DWORD nLastTick;
//};
//
//// Add to check skills by marsgod
//
//struct SkillInfo {
//    WORD Id;                // 0x00
//    BYTE bUnks[0x32];        // 0x02
//    DWORD dwUnk;            // 0x34
//};
//
//struct Skill {                // 0x40 bytes long
//    SkillInfo * pSkillInfo;    // 0x00
//    Skill * pNextSkill;        // 0x04
//    DWORD dwUnk1;            // 0x08
//    DWORD dwUnk2[0x7];        // 0x0C
//    DWORD SkillLevel;        // 0x28
//    DWORD dwUnk3[2];        // 0x2C
//    DWORD Flags;            // 0x34
//    DWORD dwUnk5[2];        // 0x38
//};
//
//struct Info {
//    BYTE * ptGame_1C;              // 0x00
//    Skill * ptFirstSkill;          // 0x04
//    Skill * ptlSkill;             // 0x08
//    Skill * ptrSkill;             // 0x0C
//    Skill * ptCurrentSkill;                // 0x10
//
//};
//
//
//
////struct Stats {
////    WORD  Param;
////    WORD  StatID;
////    DWORD Value;
////};
//
//struct StatsInfo {
//    Stats *pStats; // pointer to an array of Stats structs.
//    WORD   nUsed;  // how many Stats in array
//    WORD   nSize;  // size allocated in Stats
//};
//
//struct StatsList {
//    DWORD _1[0x9];            // 0x00
//    StatsInfo sStats;        // 0x24
//    StatsList *pPrevLink;            // 0x2C
//    StatsList *pNextLink;        // 0x30
//    StatsList *pPrev;            // 0x34
//    DWORD _4;                // 0x38
//    StatsList *pNext;        // 0x3C
//
//    StatsList *pSetList;     // +40
//    DWORD _5;
//    StatsInfo sStats1;       // +48
//    StatsInfo sStats2;       // +50 unused ???
//    DWORD *ptStateFlags;  //+58 - get with 10487(ptUnit,id) return ptUnit->ptStatsList->ptStateFlags[id/32]&(1<<(id%31))
//};
//
//// end of file --------------------------------------------------------------
//#pragma pack(pop)
//
//
//
//
//
//
//
//struct monster_data;
//struct object_data;
//struct item_data;
//struct player_data;
//struct path_t;
//struct stats;
//struct inventory;
//struct game;
//struct skills;
//struct cb_player_data;
//struct cb_item_data;
//struct objects_bin;
//struct unit;
//
//struct client_packet_buffer {
//    char data[1976];
//    int32_t data_size;
//};
//
//struct packet_wrapper_sc {
//    char data[516];
//    int32_t data_size;
//    int32_t receive_time;
//    packet_wrapper_sc* next;
//};
//
//struct client_unit_list {
//    unit* unit_list[5][128]; //0x1120
//};
//
////struct unit {
////    uint32_t unit_type; //+00
////    uint32_t txt_file_no; //+04
////    uint32_t unit_id; //+08
////    uint32_t eMode; //+0c
////    uint32_t _1[10];
////    struct {
////        uint32_t xpos1; //+00
////        uint32_t ypos1; //+04
////        uint32_t xpos2; //+08
////        uint32_t ypos2; //+0c
////        uint16_t targetx; //+10
////        uint16_t targety; //+12
////        uint32_t _1[2];
////        uint32_t* pRoom1; //+1c
////        uint32_t _2[14];
////        unit* pTargetUnit; //+58
////    } *pPos; //+38
////    uint32_t _2[13];
////    union {
////        player_data* player_data; //+70
////        monster_data* monster_data; //+70
////        object_data* object_data; //+70
////        item_data* item_data; //+70
////    };
////    uint32_t _4a[4];
////    inventory* inventory; //+84
////    uint32_t _4b[12];
////    uint32_t nOwnerType; //+b8
////    uint32_t nOwnerId; //+bc
////    uint32_t _5[10];
////    uint32_t fFlags; //+e8
////    uint32_t fFlags2; //+ec
////    uint32_t _6[7];
////    unit* pNext; //+10c
////
////    unit* pt_first_monster;            //+E0
////    unit* unit1;                    //+E4
////    unit* unit2;                    //+E8
////};
//
//
////enum class unit_type_t : int32_t {
////    UNIT_TYPE_PLAYER = 0,
////    UNIT_TYPE_MONSTER = 1,
////    UNIT_TYPE_OBJECT = 2,
////    UNIT_TYPE_MISSILE = 3,
////    UNIT_TYPE_ITEM = 4,
////    UNIT_TYPE_VIS_TILE = 5 //unused?
////};
////
////enum player_class_t : int32_t {
////    PLAYER_CLASS_AMAZON = 0,
////    PLAYER_CLASS_SORCERESS = 1,
////    PLAYER_CLASS_NECROMANCER = 2,
////    PLAYER_CLASS_PALADIN = 3,
////    PLAYER_CLASS_BARBARIAN = 4,
////    PLAYER_CLASS_DRUID = 5,
////    PLAYER_CLASS_ASSASSIN = 6
////};
//
//struct unit {
//    unit_type_t type;
//
//    union {
//        player_class_t player_class;
//        int32_t data_record_index;
//    };
//
//    void* memory_pool;
//    uint32_t guid;
//    uint32_t mode;
//
//    union {
//        player_data* player_data;
//        monster_data* monster_data;
//        object_data* object_data;
//        //missile_data* missile_data;
//        item_data* item_data;
//    };
//
//    int8_t act;
//    int8_t act_padding[0x03];
//    void* drlg_act; // drlg_act*
//
//    struct {
//        uint32_t low_seed;
//        uint32_t high_seed;
//    } seed;
//    uint32_t init_seed;
//
//    void* path; //path*
//
//    void* skill_sequence; //skill_sequence*
//    uint32_t skill_sequence_frame_count;
//    uint32_t skill_sequence_frame;
//
//    uint32_t anim_speed;
//    uint32_t skill_sequence_mode;
//
//    uint32_t current_frame;
//    uint32_t frame_count;
//    uint16_t anim_speed_w;
//
//    uint8_t action_frame;
//    uint8_t pad1;
//
//    void* anim_data; // anim_data*
//
//    void* gfxdata; // unit_gfxdata*
//    void* gfxdata_copy; // unit_gfxdata*
//
//    void* statslistex; //statslistex*
//    inventory* inventory;
//
//    union {
//        struct {
//            uint32_t interact_guid;
//            uint32_t interact_type;
//            uint8_t interacting;
//        } interaction;
//
//        struct {
//            void* light_map;
//            uint32_t start_light_radius;
//            uint16_t p12_shift_index;
//        } lightning;
//    };
//
//    uint16_t update_type;
//    unit* update_unit;
//
//    void* quest_record; //quest_record*
//    uint32_t sparky_chest;
//    void* timer_args;
//
//    union {
//        game* game;
//        uint32_t sound_sync;
//    };
//
//    char pad2[0x0C];
//
//    void* event;
//
//    unit_type_t owner_type;
//    uint32_t owner_guid;
//
//    char pad3[8];
//
//    char* hover_text;
//
//    void* skills;
//
//    void* combat;
//    uint32_t hit_class;
//
//    char pad4[4];
//
//    uint32_t drop_code;
//
//    char pad5[8];
//
//    union {
//        struct {
//            uint32_t unit_flags;
//            uint32_t unit_flags_ex;
//        } flags;
//        uint64_t flags64;
//    };
//
//    char pad6[4];
//
//    uint32_t node_index;
//    uint32_t get_tick_count;
//
//    union {
//        uint32_t get_tick_count2;
//        void* particle_stream;
//    };
//
//    void* timer;
//
//    unit* change_next_unit; //?
//    unit* prev_unit;
//    unit* prev_unit_in_room;
//
//    void* msg_first;
//    void* msg_last;
//
//    bool is_hireling() const {
//        if (type != unit_type_t::UNIT_TYPE_MONSTER)
//            return false;
//
//        return (flags.unit_flags & 0x00000200) == 0x00000200;
//    }
//
//    bool is_pet() const {
//        if (type != unit_type_t::UNIT_TYPE_MONSTER)
//            return false;
//
//        return (flags.unit_flags & 0x80000000) == 0x80000000;
//    }
//};
//
//
////struct unit {                                        //Offset from Code.        Size: 0xF4+4
////    uint32_t        unit_type;                //+00
////    union {
////        uint32_t            player_class;
////        uint32_t            txt_file_no;
////    };                                    //+04
////    uint32_t        unit_id;                //+08
////    uint32_t        item_num;                //+0C
////    uint32_t        current_anim;            //+10
////    union {
////        monster_data* monster_data;
////        object_data* object_data;
////        item_data* item_data;
////        player_data* player_data;
////    };                                    //+14
////    uint8_t        act;                    //+18
////    uint8_t        uk12[3];                //+19
////    void* pt_act;                    //+1C
////    uint32_t        seed_low;                //+20
////    uint32_t        seed_hi;                    //+24
////    uint32_t        seed_starting;            //+28
////    path_t* path;                    //+2C
////    uint8_t        uk1[0x08];                //+30
////    path_t* pt_pos;                    //+38
////    uint32_t        anim_speed;                //+3C
////    uint8_t        uk2[0x04];                //+40
////    uint32_t        cur_frame;                //+44
////    uint32_t        remaining_frame;            //+48
////    uint16_t        anim_speed_w;                //+4C
////    uint8_t        action_flag;                //+4E
////    uint8_t        uk3[0x1];                //+4F
////    void* pt_anim_data;                //+50
////    uint8_t        uk4[0x08];                //+54
////    stats* stats;                //+5C
////    uint8_t        pad1[0x24];
////    inventory* inventory;         //+84  //+60
////    uint8_t        uk5[0x1C];                //+64
////    game* game;                    //+80
////    uint8_t        uk6[0x24];                //+84
////    skills* skills;                //+A8
////    void* pt_combat_data;            //+AC
////    uint8_t        uk7[0x08];                //+B0
////    uint32_t        item_code;                //+B8 for an item being dropped by this unit
////    uint8_t        uk8[0x08];                //+BC
////    uint32_t        flags1;                    //+C4
////    union {
////        uint32_t        flags2;                //+C8    //flags
////        struct {
////            uint32_t    ukf1 : 25;
////            uint32_t    is_lod : 1;
////        };
////    };
////    uint8_t        uk9[0x04];                //+CC
////    uint32_t        client_id;                //+D0
////    uint8_t        uk10[0x0C];                //+D4
////    unit* pt_first_monster;            //+E0
////    unit* unit1;                    //+E4
////    unit* unit2;                    //+E8
////    uint8_t        uk11[0x08];                //+EC
////    union {
////        cb_player_data* pt_cb_player_data;
////        cb_item_data* pt_cb_item_data;
////        void* pt_cb_data;
////    };                                    //+F4
////};
//
//struct monster_data                    // sizeof(MonsterData)=0x60 {                                        //Offset from Code.
//    uint8_t    uk[0x16];                    //+00
//    union {
//        uint16_t    flags;                    //+16
//        struct {
//            uint16_t    fuk1 : 1;
//            uint16_t    is_super_unique : 1;
//            uint16_t    is_champion : 1;
//            uint16_t    is_unique : 1;
//            uint16_t    fuk2 : 13;
//        };
//    };
//    uint8_t        uk1[0x0E];                    //+18
//    uint16_t        super_unique_id;                //+26
//    void* unknow1;                    //+28
//    uint8_t        uk2[0x38];                    //+28
//};
//
//struct object_data {
//    objects_bin* pt_objects_bin;
//    uint8_t        level_id;
//};
//
//struct item_data {                        // Offset from Code
//    uint32_t quality;       // +00.
//    uint32_t lowSeed;       // +04.
//    uint32_t highSeed;      // +08.
//    uint32_t unknown1;      // +0c.
//    uint32_t startLowSeed;  // +10.
//    uint32_t cmdFlags;      // +14.
//    uint32_t flags;         // +18.
//    uint32_t unknown2[2];   // +1C
//    uint32_t originID;      // +24.
//    uint32_t iLvl;          // +28.
//    uint16_t  version;       // +2C.
//    uint16_t  prefix;        // +2E.
//    uint16_t  suffix;        // +30.
//    uint16_t  autoPrefix;    // +32.
//    uint16_t  prefix1;       // +34.
//    uint16_t  prefix2;       // +36.
//    uint16_t  prefix3;       // +38.
//    uint16_t  suffix1;       // +3A.
//    uint16_t  suffix2;       // +3C.
//    uint16_t  suffix3;       // +3E.
//    uint8_t  bodyLoc;       // +40.    // 0=inv/stash 4=gaucheS 5=droiteS
//    uint8_t  page;        // +41.    // FF=equip 00=inv 04=stash
//    uint8_t  unknown3[3];   // +42
//    uint8_t  picture[8];    // +45
//    uint32_t ilvl;          // +4C
//};
//
////struct item_data//size=0x74
////{                                        //Offset from Code.
////    uint32_t    quality;                    //+00
////    uint32_t    seed_low;                    //+04
////    uint32_t    seed_hi;                        //+08
////    uint32_t    player_id;                    //+0C #10734 / #10735 (PCInventory->ptPlayer->0C)
////    uint32_t    seed_starting;                //+10
////    uint32_t    flags1;                        //+14
////    union {
////        uint32_t    flags2;                    //+18
////        struct {
////            uint32_t    fuk1 : 1;                //0x00000001
////            uint32_t    is_indentified : 1;    //0x00000002
////            uint32_t    fuk2 : 2;                //0x0000000C
////            uint32_t    is_unindentified : 1;    //0x00000010
////            uint32_t    fuk3 : 3;                //0x000000E0
////            uint32_t    is_broken : 1;            //0x00000100
////            uint32_t    fuk4 : 2;                //0x00000600
////            uint32_t    is_socketed : 1;        //0x00000800
////            uint32_t    fuk5 : 10;            //0x003FF000
////            uint32_t    is_etheral : 1;        //0x00400000
////            uint32_t    fuk6 : 3;                //0x03800000
////            uint32_t    is_runeword : 1;        //0x04000000
////            uint32_t    fuk7 : 1;                //0x08000000
////            uint32_t    is_personalized : 1;    //0x10000000
////            uint32_t    fuk8 : 3;                //0xE0000000
////        };
////    };
////    /*
////ITEMFLAG_NEWITEM               = 0x00000001,
////ITEMFLAG_TAGETING               = 0x00000004,
////ITEMFLAG_UNIDENTIFIED               = 0x00000010,
////ITEMFLAG_QUANTITY               = 0x00000020,
////ITEMFLAG_Durability               = 0x00000100,
////ITEMFLAG_UNKNOWN2               = 0x00000400,
////ITEMFLAG_SOCKETED               = 0x00000800,
////ITEMFLAG_NON_SELLABLE               = 0x00001000,
////ITEMFLAG_NEWITEM2               = 0x00002000,
////ITEMFLAG_UNKNOWN3               = 0x00004000,
////ITEMFLAG_CHECKSECPRICE               = 0x00010000,
////ITEMFLAG_CHECKGAMBLEPRICE          = 0x00020000,
////ITEMFLAG_UNKNOWN4               = 0x00080000,
////ITEMFLAG_INDESTRUCTIBLE(ETHERAL) = 0x00400000,
////ITEMFLAG_UNKNOWN5               = 0x00800000,
////ITEMFLAG_FROMPLAYER               = 0x01000000,
////ITEMFLAG_RUNEuint16_t               = 0x04000000
////*/
////    uint32_t    guid1;                        //+1C Global Unique ID 1
////    uint32_t    guid2;                        //+20 Global Unique ID 2
////    uint32_t    guid3;                        //+24 Global Unique ID 3
////    uint32_t    unique_id;                    //+28
////    uint8_t    ilvl;                        //+2C
////    uint8_t    uk1[0x03];                    //+2D
////    uint16_t    version;                    //+30
////    uint16_t    rare_prefix;                    //+32
////    uint16_t    rare_suffix;                    //+34
////    uint16_t    auto_pref;                    //+36
////    uint16_t    prefix[3];                    //+38
////    uint16_t    suffix[3];                    //+3E
////    uint8_t    equip_loc;                    //+44
////    /*    emplacement si équipé
////    *    00 = noequip/inBelt
////    *   01 = head
////    *    02 = neck
////    *    03 = tors
////    *    04 = rarm
////    *    05 = larm
////    *    06 = lrin
////    *    07 = rrin
////    *    08 = belt
////    *    09 = feet
////    *    0A = glov
////    *    0B = ralt
////    *    0C = lalt
////*/
////    uint8_t    page;                        //+45
////    /*    page dans laquel se trouve l'item
////    *    FF = mouse/equip/onEarth
////    *    00 = inventory
////    *   01 = cube
////    *    04 = stash
////    */
////    uint8_t    uk4[0x01];                    //+46
////    uint8_t    item_data3;                    //+47 //D2Common10854 D2Common10853
////    uint8_t    p_ear_level;                    //+48
////    uint8_t    var_gfx;                        //+49
////    char    i_name[0x12];                //+4A //inscribed/ear name
////    inventory* inventory;            //+5C
////    unit* pt_prev_item;                    //+60
////    unit* pt_next_item;                    //+64
////    uint8_t    uk8[0x01];                    //+68
////    uint8_t    item_data2;                    //+69
////    uint8_t    uk9[0x0A];                    //+6A
////};
//
//struct net_client {
//    uint32_t    client_id;                    //+00
//    uint8_t    uk1[0x06];                    //+04
//    union {                                //+0A
//        uint16_t    flag;
//        struct {
//            uint16_t f1 : 1;
//            uint16_t f2 : 1;
//            uint16_t is_hard_core_game : 1;
//        };
//    };
//    uint8_t    uk2;                        //+0C
//    char    name[0x10];                    //+0D
//    uint8_t    uk3[0x15F];                    //+1D
//    uint8_t* savefile;                    //+17C
//    uint32_t    final_size;                    //+180
//    uint32_t    counter;                    //+184
//    uint32_t    current_size;                //+188
//    uint8_t    uk4[0x1C];                    //+18C
//    game* game;                        //+1A8
//    //+1A8 is ptGame
//    //+4A8 ptNextClient
///*
//Kingpin: ptPclient
//+16C is either act or unit type
//+170 Active UniqueID for player
//+174 Active ptPlayer on Client
//+1a8 ptGame
//+1b4 Current or next ptRoom
//*/
//};
//
//struct player_data {
//    char        name[0x10];                //+00    Player Name
//    void* pt_quest[3];                //+10    Quest Pointers for each difficulty
//    uint8_t        uk1[0x18];                //+1C        //before : 0x14
//    void* pt_arena_unit;            //+34    ptArena for the Unit
//    uint8_t        uk2[0x4];                //+38        //before : 0x7
//    uint16_t        mp_source_portal_unique_id;    //+3C    Source Portal Unique_ID
//    uint8_t        uk3[0x2];                //+3E
//    uint16_t        mp_dest_portal_unique_id;    //+40    Destination Portal Unique_ID
//    uint8_t        uk4[0x06];                //+42
//    uint8_t        pt_object_un_id;            //+48    Object UniqueID for TownPortals
//    uint8_t        uk5[0x94B];//uk5[0x53];                //+49
//    net_client* net_client;            //+9C    ptClient
//};
//
//struct position {
//    uint32_t x;
//    uint32_t y;
//};
//
//struct room//size=0x80 {
//    //ptRoom +48 0 = spawn new units (monster, objects e.tc), 1 = don't spawn any new units
//    uint32_t    seed1;                        //+00
//    uint32_t    seed2;                        //+04
//    uint8_t    uk8[0x1C];                    //+08
//    room* pt_near_rooms;                //+24
//    uint32_t    nb_near_rooms;                //+28
//    unit* unit;                        //+2C
//    uint8_t    uk5[0x44];                    //+30
//    room* pt_next_room;                    //+74
//    union {
//        uint8_t    flags;                    //+78
//        struct {
//            uint8_t is_generated : 1;
//            uint8_t is_generated2 : 1;//???
//        };
//    };
//};
//
//struct act_map//ptGame+BC size=0x60 {
//    uint32_t    is_not_managed;
//    uint32_t    uk4;
//    uint32_t    uk8;//size = 0x488
//    room* pt_first_room;
//};
//
//struct game {
//    char pad_0000[24]; //0x0000
//    void* critical_section; //0x0018
//    void* memory_pool; //0x001C
//    char pad_0020[74]; //0x0020
//    int8_t game_type; //0x006A
//    char pad_006_b[2]; //0x006B
//    int8_t difficulty_level; //0x006D
//    char pad_006_e[2]; //0x006E
//    int32_t expansion; //0x0070
//    int32_t game_type2; //0x0074
//    int32_t item_format; //0x0078
//    int32_t init_seed; //0x007C
//    int32_t object_seed; //0x0080
//    int32_t init_seed2; //0x0084
//    void* last_client; //0x0088 structure of last player that entered the game
//    int32_t clients_count; //0x008C
//    int32_t unit_counts[6]; //0x0090
//    int32_t game_frame; //0x00A8
//    char pad_00_ac[12]; //0x00AC
//    void* timer_queue; //0x00B8
//    void* drlg_act[5]; //0x00BC
//    int32_t lo_seed; //0x00D0
//    int32_t hi_seed; //0x00D4
//    char pad_00d8[20]; //0x00D8
//    int32_t monster_seed; //0x00EC
//    char pad_244[244];
//    unit* unit_list[5][128];//0x330
//    //void* monster_region[1024]; //0x00F0
//    //void* object_controller; //0x10F0
//    //void* quest_controller; //0x10F4
//    //void* unit_nodes[10]; //0x10F8
//    //unit* unit_list[5][128]; //0x1120
//    //void* tile_list; //0x1B20
//    //int32_t unique_flags[128]; //0x1B24
//    //void* npc_control; //0x1D24
//    //void* arena_control; //0x1D28
//    //void* party_control; //0x1D2C
//    //int8_t boss_flags[64]; //0x1D30
//    //int32_t monster_mode_data[17]; //0x1D70
//    //int32_t monster_mode_data_count; //0x1DB4
//    //char pad_1db8[12]; //0x1DB8
//    //int32_t sync_timer; //0x1DC4
//    //char pad_1dc8[32]; //0x1DC8
//    //int32_t uber_baal; //0x1DE8
//    //int32_t uber_diablo; //0x1DEC
//    //int32_t uber_mephisto; //0x1DF0
//}; //Size: 0x1DF4
////static_assert(sizeof(game) == 0x1DF4);
//
//struct path_t //(8 dword) {                                          //Offset from Code.        Size: 20
//    uint16_t    uk1;                        //+00
//    uint16_t    mapx;                        //+02
//    uint16_t    uk2;                        //+04
//    uint16_t    mapy;                        //+06
//    uint32_t    uk3;                        //+08
//    uint32_t    x;                            //+0C
//    uint32_t    y;                            //+10
//    uint32_t    uk6;                        //+14
//    uint32_t    uk7;                        //+18
//    room* pt_room;                        //+1C
//};
//
//struct inventory {                                       //Offset from Code.        Size: 30 à 40
//    uint32_t    tag;                        //+00    = 0x01020304
//    uint8_t    uk1[0x04];                    //+04    =? 0
//    unit* pt_char;                        //+08
//    unit* pt_first_item;                        //+0C
//    unit* pt_last_item; //+10
//    void* inv_info_ptr; //+14
//    uint32_t inv_info; //+18
//    uint32_t weapon_guid; //+1C
//    unit* inv_owner_item;                    //+20
//    uint8_t    owner_guid;                    //+24
//    uint32_t    current_used_socket;            //+28 //Kingpin : a variable to know how many sockets that have added to item
//    uint32_t    inventory2_c;                //+2C //one value
//    uint32_t    inventory30;                //+30
//    void* pt_corpse;                    //+34
//    uint8_t    uk5[0x04];                    //+38
//    uint32_t    next_corpse_id;                //+3C //max = 15
//    uint8_t    uk6[0x04];                    //+40
//};
//
////https://phrozen-library.fandom.com/wiki/Items_line
////struct items_line {
////    char    szflippyfile[32];    //0x00
////    char    szinvfile[32];        //0x20
////    char    szuniqueinvfile[32];    //0x40
////    char    szsetinvfile[32];    //0x60
////    union {
////        uint32_t    dwcode;        //0x80
////        char    szcode[4];    //0x80
////    };
////    uint32_t    dwnormcode;        //0x84
////    uint32_t    dwubercode;        //0x88
////    uint32_t    dwultracode;        //0x8C
////    uint32_t    dwalternategfx;        //0x90
////    uint32_t    dwpSpell;        //0x94
////    uint16_t    wstate;            //0x98
////    uint16_t    wcstate1;        //0x9A
////    uint16_t    wcstate2;        //0x9C
////    uint16_t    wstat1;            //0x9E
////    uint16_t    wstat2;            //0xA0
////    uint16_t    wstat3;            //0xA2
////    uint32_t    dwcalc1;        //0xA4
////    uint32_t    dwcalc2;        //0xA8
////    uint32_t    dwcalc3;        //0xAC
////    uint32_t    dwLen;            //0xB0
////    uint16_t    bSpellDesc;        //0xB4
////    uint16_t    wSpellDescStr;        //0xB6
////    uint32_t    dwSpellDescCalc;    //0xB8
////    uint32_t    dwBetterGem;        //0xBC
////    uint32_t    dwwclass;        //0xC0
////    uint32_t    dw2handedwclass;    //0xC4
////    uint32_t    dwTMogType;        //0xC8
////    uint32_t    dwminac;        //0xCC
////    uint32_t    dwmaxac;        //0xD0
////    uint32_t    dwgamblecost;        //0xD4
////    uint32_t    dwspeed;        //0xD8
////    uint32_t    dwbitfield1;        //0xDC
////    uint32_t    dwcost;            //0xE0
////    uint32_t    dwminstack;        //0xE4
////    uint32_t    dwmaxstack;        //0xE8
////    uint32_t    dwspawnstack;        //0xEC
////    uint32_t    dwgemoffset;        //0xF0
////    uint16_t    wnamestr;        //0xF4
////    uint16_t    wversion;        //0xF6
////    uint16_t    wautoprefix;        //0xF8
////    uint16_t    wmissiletype;        //0xFA
////    uint8_t    brarity;        //0xFC
////    uint8_t    blevel;            //0xFD
////    uint8_t    bmindam;        //0xFE
////    uint8_t    bmaxdam;        //0xFF
////    uint8_t    bminmisdam;        //0x100
////    uint8_t    bmaxmisdam;        //0x101
////    uint8_t    b2handmindam;        //0x102
////    uint8_t    b2handmaxdam;        //0x103
////    uint16_t    brangeadder;        //0x104
////    uint16_t    wstrbonus;        //0x106
////    uint16_t    wdexbonus;        //0x108
////    uint16_t    wreqstr;        //0x10A
////    uint16_t    wreqdex;        //0x10C
////    uint8_t    babsorbs;        //0x10E
////    uint8_t    binvwidth;        //0x10F
////    uint8_t    binvheight;        //0x110
////    uint8_t    bblock;            //0x111
////    uint8_t    bdurability;        //0x112
////    uint8_t    bnodurability;        //0x113
////    uint8_t    bmissile;        //0x114
////    uint8_t    bcomponent;        //0x115
////    uint8_t    brArm;            //0x116
////    uint8_t    blArm;            //0x117
////    uint8_t    btorso;            //0x118
////    uint8_t    blegs;            //0x119
////    uint8_t    brspad;            //0x11A
////    uint8_t    blspad;            //0x11B
////    uint8_t    b2handed;        //0x11C
////    uint8_t    buseable;        //0x11D
////    uint16_t    wtype;            //0x11E
////    uint16_t    wtype2;            //0x120
////    uint16_t    bsubtype;        //0x122
////    uint16_t    wdropsound;        //0x124
////    uint16_t    wusesound;        //0x126
////    uint8_t    bdropsfxframe;        //0x128
////    uint8_t    bunique;        //0x129
////    uint8_t    bquest;            //0x12A
////    uint8_t    bquestdiffcheck;    //0x12B
////    uint8_t    btransparent;        //0x12C
////    uint8_t    btranstbl;        //0x12D
////    uint8_t     pad0x12E;        //0x12E
////    uint8_t    blightradius;        //0x12F
////    uint8_t    bbelt;            //0x130
////    uint8_t    bautobelt;        //0x131
////    uint8_t    bstackable;        //0x132
////    uint8_t    bspawnable;        //0x133
////    uint8_t    bspellicon;        //0x134
////    uint8_t    bdurwarning;        //0x135
////    uint8_t    bqntwarning;        //0x136
////    uint8_t    bhasinv;        //0x137
////    uint8_t    bgemsockets;        //0x138
////    uint8_t    bTransmogrify;        //0x139
////    uint8_t    bTMogMin;        //0x13A
////    uint8_t    bTMogMax;        //0x13B
////    uint8_t    bhitclass;        //0x13C
////    uint8_t    b1or2handed;        //0x13D
////    uint8_t    bgemapplytype;        //0x13E
////    uint8_t    blevelreq;        //0x13F
////    uint8_t    bmagiclvl;        //0x140
////    uint8_t    bTransform;        //0x141
////    uint8_t    bInvTrans;        //0x142
////    uint8_t    bcompactsave;        //0x143
////    uint8_t    bSkipName;        //0x144
////    uint8_t    bNameable;        //0x145
////    uint8_t    bAkaraMin;        //0x146
////    uint8_t    bGheedMin;        //0x147
////    uint8_t    bCharsiMin;        //0x148
////    uint8_t    bFaraMin;        //0x149
////    uint8_t    bLysanderMin;        //0x14A
////    uint8_t    bDrognanMin;        //0x14B
////    uint8_t    bHraltiMin;        //0x14C
////    uint8_t    bAlkorMin;        //0x14D
////    uint8_t    bOrmusMin;        //0x14E
////    uint8_t    bElzixMin;        //0x14F
////    uint8_t    bAshearaMin;        //0x150
////    uint8_t    bCainMin;        //0x151
////    uint8_t    bHalbuMin;        //0x152
////    uint8_t    bJamellaMin;        //0x153
////    uint8_t    bMalahMin;        //0x154
////    uint8_t    bLarzukMin;        //0x155
////    uint8_t    bDrehyaMin;        //0x156
////    uint8_t    bAkaraMax;        //0x157
////    uint8_t    bGheedMax;        //0x158
////    uint8_t    bCharsiMax;        //0x159
////    uint8_t    bFaraMax;        //0x15A
////    uint8_t    bLysanderMax;        //0x15B
////    uint8_t    bDrognanMax;        //0x15C
////    uint8_t    bHraltiMax;        //0x15D
////    uint8_t    bAlkorMax;        //0x15E
////    uint8_t    bOrmusMax;        //0x15F
////    uint8_t    bElzixMax;        //0x160
////    uint8_t    bAshearaMax;        //0x161
////    uint8_t    bCainMax;        //0x162
////    uint8_t    bHalbuMax;        //0x163
////    uint8_t    bJamellaMax;        //0x164
////    uint8_t    bMalahMax;        //0x165
////    uint8_t    bLarzukMax;        //0x166
////    uint8_t    bDrehyaMax;        //0x167
////    uint8_t    bAkaraMagicMin;        //0x168
////    uint8_t    bGheedMagicMin;        //0x169
////    uint8_t    bCharsiMagicMin;    //0x16A
////    uint8_t    bFaraMagicMin;        //0x16B
////    uint8_t    bLysanderMagicMin;    //0x16C
////    uint8_t    bDrognanMagicMin;    //0x16D
////    uint8_t    bHraltiMagicMin;    //0x16E
////    uint8_t    bAlkorMagicMin;        //0x16F
////    uint8_t    bOrmusMagicMin;        //0x170
////    uint8_t    bElzixMagicMin;        //0x171
////    uint8_t    bAshearaMagicMin;    //0x172
////    uint8_t    bCainMagicMin;        //0x173
////    uint8_t    bHalbuMagicMin;        //0x174
////    uint8_t    bJamellaMagicMin;    //0x175
////    uint8_t    bMalahMagicMin;        //0x176
////    uint8_t    bLarzukMagicMin;    //0x177
////    uint8_t    bDrehyaMagicMin;    //0x178
////    uint8_t    bAkaraMagicMax;        //0x179
////    uint8_t    bGheedMagicMax;        //0x17A
////    uint8_t    bCharsiMagicMax;    //0x17B
////    uint8_t    bFaraMagicMax;        //0x17C
////    uint8_t    bLysanderMagicMax;    //0x17D
////    uint8_t    bDrognanMagicMax;    //0x17E
////    uint8_t    bHraltiMagicMax;    //0x17F
////    uint8_t    bAlkorMagicMax;        //0x180
////    uint8_t    bOrmusMagicMax;        //0x181
////    uint8_t    bElzixMagicMax;        //0x182
////    uint8_t    bAshearaMagicMax;    //0x183
////    uint8_t    bCainMagicMax;        //0x184
////    uint8_t    bHalbuMagicMax;        //0x185
////    uint8_t    bJamellaMagicMax;    //0x186
////    uint8_t    bMalahMagicMax;        //0x187
////    uint8_t    bLarzukMagicMax;    //0x188
////    uint8_t    bDrehyaMagicMax;    //0x189
////    uint8_t    bAkaraMagicLvl;        //0x18A
////    uint8_t    bGheedMagicLvl;        //0x18B
////    uint8_t    bCharsiMagicLvl;    //0x18C
////    uint8_t    bFaraMagicLvl;        //0x18D
////    uint8_t    bLysanderMagicLvl;    //0x18E
////    uint8_t    bDrognanMagicLvl;    //0x18F
////    uint8_t    bHraltiMagicLvl;    //0x190
////    uint8_t    bAlkorMagicLvl;        //0x191
////    uint8_t    bOrmusMagicLvl;        //0x192
////    uint8_t    bElzixMagicLvl;        //0x193
////    uint8_t    bAshearaMagicLvl;    //0x194
////    uint8_t    bCainMagicLvl;        //0x195
////    uint8_t    bHalbuMagicLvl;        //0x196
////    uint8_t    bJamellaMagicLvl;    //0x197
////    uint8_t    bMalahMagicLvl;        //0x198
////    uint8_t    bLarzukMagicLvl;    //0x199
////    uint8_t    bDrehyaMagicLvl;    //0x19A
////    uint8_t    pad0x19B;        //0x19B
////    uint32_t    dwNightmareUpgrade;    //0x19C
////    uint32_t    dwHellUpgrade;        //0x1A0
////    uint8_t    bPermStoreItem;        //0x1A4
////    uint8_t    bmultibuy;        //0x1A5
////    uint16_t    pad0x1A6;        //0x1A6
////};
//
//
//#pragma pack(push, 1)
//namespace d2_protocol {
//    enum message_types_t {
//        MESSAGE_TYPE_ITEM_MOVE,
//        MESSAGE_TYPE_COUNT
//    };
//
//    struct packet_header {
//        uint16_t d2_packet_type;
//        uint8_t message_type;
//
//        packet_header() : d2_packet_type(0xBB), message_type(0) {}
//    };
//
//    struct item_move_cs : packet_header {
//        uint32_t item_guid;
//        uint8_t target_page;
//        uint8_t source_page;
//
//        item_move_cs() : item_guid(0), target_page(0), source_page(0) {
//            message_type = MESSAGE_TYPE_ITEM_MOVE;
//        }
//    };
//
//
//    struct to_belt_cs : packet_header {
//        uint32_t item_guid;
//        to_belt_cs() : item_guid(0) {
//            message_type = MESSAGE_TYPE_ITEM_MOVE;
//        }
//    };
//
//
//    struct item_move_sc : packet_header {
//        uint32_t item_guid;
//        uint32_t tx;
//        uint32_t ty;
//        uint8_t target_page;
//        uint8_t source_page;
//
//        item_move_sc() : item_guid(0), tx(0), ty(0), target_page(0), source_page(0) {
//            message_type = MESSAGE_TYPE_ITEM_MOVE;
//        }
//    };
//}
//#pragma pack(pop, 1)

#endif