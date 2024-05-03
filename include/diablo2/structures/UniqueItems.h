#pragma once

#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct D2UniqueItemsTxt
		{
			uint16_t unk0x00;						//0x00
			char szName[34];					//0x02
			uint32_t dwVersion;					//0x24
			union
			{
				uint32_t dwBaseItemCode;			//0x28
				char szBaseItemCode[4];			//0x28
			};
			uint32_t dwUniqueItemFlags;			//0x2C
			uint32_t dwRarity;						//0x30
			uint16_t wLvl;							//0x34
			uint16_t wLvlReq;						//0x36
			uint8_t nChrTransform;					//0x38
			uint8_t nInvTransform;					//0x39
			char szFlippyFile[32];				//0x3A
			char szInvFile[34];					//0x5A
			uint32_t dwCostMult;					//0x7C
			uint32_t dwCostAdd;					//0x80
			uint16_t wDropSound;					//0x84
			uint16_t wUseSound;						//0x86
			uint32_t dwDropSfxFrame;				//0x88
			uint32_t dwProp1;						//0x8C
			uint32_t dwPar1;						//0x90
			uint32_t dwMin1;						//0x94
			uint32_t dwMax1;						//0x98
			uint32_t dwProp2;						//0x9C
			uint32_t dwPar2;						//0xA0
			uint32_t dwMin2;						//0xA4
			uint32_t dwMax2;						//0xA8
			uint32_t dwProp3;						//0xAC
			uint32_t dwPar3;						//0xB0
			uint32_t dwMin3;						//0xB4
			uint32_t dwMax3;						//0xB8
			uint32_t dwProp4;						//0xBC
			uint32_t dwPar4;						//0xC0
			uint32_t dwMin4;						//0xC4
			uint32_t dwMax4;						//0xC8
			uint32_t dwProp5;						//0xCC
			uint32_t dwPar5;						//0xD0
			uint32_t dwMin5;						//0xD4
			uint32_t dwMax5;						//0xD8
			uint32_t dwProp6;						//0xDC
			uint32_t dwPar6;						//0xE0
			uint32_t dwMin6;						//0xE4
			uint32_t dwMax6;						//0xE8
			uint32_t dwProp7;						//0xEC
			uint32_t dwPar7;						//0xF0
			uint32_t dwMin7;						//0xF4
			uint32_t dwMax7;						//0xF8
			uint32_t dwProp8;						//0xFC
			uint32_t dwPar8;						//0x100
			uint32_t dwMin8;						//0x104
			uint32_t dwMax8;						//0x10
			uint32_t dwProp9;						//0x10C
			uint32_t dwPar9;						//0x110
			uint32_t dwMin9;						//0x114
			uint32_t dwMax9;						//0x118
			uint32_t dwProp10;						//0x11C
			uint32_t dwPar10;						//0x120
			uint32_t dwMin10;						//0x124
			uint32_t dwMax10;						//0x128
			uint32_t dwProp11;						//0x12C
			uint32_t dwPar11;						//0x130
			uint32_t dwMin11;						//0x134
			uint32_t dwMax11;						//0x138
			uint32_t dwProp12;						//0x13C
			uint32_t dwPar12;						//0x140
			uint32_t dwMin12;						//0x144
			uint32_t dwMax12;						//0x148
		};
	}
}