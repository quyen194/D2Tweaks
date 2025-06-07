#pragma once

#include <cstdint>

namespace diablo2 {
namespace structures {
struct Inventory;

struct npc_gamble {       // sizeof 0xC
  Inventory* pInventory;  // +00
  uint32_t dwGUID;        // +04
  // npc_gamble*  pNext;     // +08
};

struct npc_record {                 // sizeof 0x44
  int nNPC;                         // +00
  Inventory* pInventory;            // +04
  npc_gamble* pGamble;              // +08
  bool bGambleInit;                 // +0C
  uint32_t* pMercData;              // +10 //D2MercDataStrc*
  uint32_t* pEvent;                 // +14 //D2NPCEventStrc*
  uint32_t* pVendorChain;           // +18  //D2VendorChainStrc*
  bool bTrading;                    // +1C
  union {
    struct {
      union {
        bool bFlags[8];             // +20
        struct {
          bool bVendorInit;         // +20
          bool bHireInit;           // +21
          uint8_t nAct;             // +22
          bool bTrader;             // +23
          bool bLevelRefresh;       // +24
          bool bInited;             // +25
          bool bForceVendor;        // +26
          bool bRefreshInventory;   // +27
        };
      };

      uint32_t dwTicks;             // +28
      uint32_t pProxy[4];           // +2C //D2UnitProxyStrc
      uint32_t dwUnk;               // +3C
      uint32_t dwNPCGUID;           // +40
    };

    uint32_t pTrade;  // +20 //D2NPCTradeStrc
  };
};
}  // namespace structures
}  // namespace diablo2
