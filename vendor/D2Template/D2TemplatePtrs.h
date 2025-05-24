#pragma once

#ifndef _D2PTRS_H
#define _D2PTRS_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  These are the macros used by the template core to declare                                                                                                                                                                   ///
//  pointers. Do not touch unless you know what you're doing                                                                                                                                                                    ///
//                                                                                                                                                                                                                              ///
//                                                                                                                                                                                                                              ///
//#ifdef _MSC_VER // MS Compiler                                                                                                                                                                                                  ///
//#define D2FUNC(DLL, NAME, RETURN, CONV, ARGS, OFFSET) typedef RETURN (CONV* DLL##_##NAME##_t) ARGS; __declspec(selectany) extern DLL##_##NAME##_t DLL##_##NAME = (DLL##_##NAME##_t)GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);   ///
//#define D2VAR(DLL, NAME, TYPE, OFFSET) typedef TYPE DLL##_##NAME##_vt; __declspec(selectany) extern DLL##_##NAME##_vt * DLL##_##NAME = (DLL##_##NAME##_vt *)GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);                          ///
//#define D2PTR(DLL, NAME, OFFSET) __declspec(selectany) extern DWORD NAME = GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);                                                                                                           ///
//#else // GCC Compiler                                                                                                                                                                                                           ///
//#define D2FUNC(DLL, NAME, RETURN, CONV, ARGS, OFFSET) typedef RETURN (CONV* DLL##_##NAME##_t) ARGS; DLL##_##NAME##_t DLL##_##NAME __attribute__((weak)) = (DLL##_##NAME##_t)GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);          ///
//#define D2VAR(DLL, NAME, TYPE, OFFSET) typedef TYPE DLL##_##NAME##_vt; DLL##_##NAME##_vt * DLL##_##NAME __attribute__((weak)) = (DLL##_##NAME##_vt *)GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);                                 ///
//#define D2PTR(DLL, NAME, OFFSET) DWORD NAME __attribute__((weak)) = GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);                                                                                                                  ///
//#endif

#define D2FUNC(DLL, NAME, RETURN, CONV, ARGS, OFFSET) typedef RETURN (CONV##* DLL##_##NAME##_t) ARGS; static DLL##_##NAME##_t DLL##_##NAME = (OFFSET < 0) ? ((DLL##_##NAME##_t)GetProcAddress((HMODULE)DLLBASE_##DLL, (LPCSTR)-(OFFSET))) : (DLL##_##NAME##_t)(DLLBASE_##DLL + OFFSET);        ///
#define D2VAR(DLL, NAME, TYPE, OFFSET) typedef TYPE DLL##_##NAME##_vt; static DLL##_##NAME##_vt * DLL##_##NAME = (DLL##_##NAME##_vt *)(DLLBASE_##DLL + OFFSET);                                 ///
#define D2PTR(DLL, NAME, OFFSET) static DWORD NAME = (DLLBASE_##DLL + OFFSET);

extern DWORD __fastcall GetDllOffset(char* ModuleName, DWORD BaseAddress, int Offset);                                                                                                                                          ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//D2FUNC(D2NET, SendPacket, void, __stdcall, (DWORD arg1, BYTE* buf, size_t len), -10005)
//
//D2FUNC(D2GFX, D2GFX_10055, void, __stdcall, (int nXStart, int nYStart, int nXEnd, int nYEnd, DWORD dwColor, int nDrawMode), -10055)
//D2FUNC(D2CLIENT, CheckGameType, int, __fastcall, (), 0xBFF0) // 0 = Single Player 6 = Unknown 8 = Multiplayer
//D2VAR(D2CLIENT, ScreenHeight, int, 0xD40F0)
//D2VAR(D2CLIENT, ScreenWidth, int, 0xD40F4)
//D2VAR(D2CLIENT, MouseX, int, 0x121AE4)
//D2VAR(D2CLIENT, MouseY, int, 0x6FBC1AE8 - 0x6FAA0000)
//D2VAR(D2CLIENT, GetUIWindowState_base, BOOL*, 0x11A6A8)
//
////D2FUNC(D2CLIENT, GetPlayerUnit, UnitAny*, __stdcall, (), 0x883D0)
//D2FUNC(D2CLIENT, LoadCellFile, CellFile*, __fastcall, (const char* szFilename, int nCellFileType), 0x1000)
//
//D2FUNC(D2COMMON, GetStat, DWORD, __stdcall, (UnitAny* Unit, DWORD StatID, DWORD index), -10519)
//D2FUNC(D2COMMON, GetStatSigned, DWORD, __stdcall, (UnitAny* Unit, DWORD StatID, DWORD index), -10520)
//D2FUNC(D2COMMON, GetBaseStat, DWORD, __stdcall, (UnitAny* Unit, DWORD StatID, DWORD index), -10521)
//D2FUNC(D2COMMON, GetModStat, DWORD, __stdcall, (UnitAny* Unit, DWORD StatID, DWORD index), -10522)
//D2FUNC(D2COMMON, GetUnitState, DWORD, __stdcall, (UnitAny* Unit, DWORD State), -10487)
//D2FUNC(D2COMMON, IsInTown, bool, __stdcall, (Room * ptRoom), -10082);// #10082
//D2FUNC(D2COMMON, IsMonsterNPCInTown, bool, __fastcall, (UnitAny* pMonster), 0x656C0)
//D2FUNC(D2COMMON, GetMonsterColorIndex, int, __stdcall, (UnitAny* mon, int no), -11017)
//D2FUNC(D2COMMON, GetUnitStat, int, __stdcall, (UnitAny* unit, DWORD statno, DWORD unk), -10519)
//
//D2VAR(D2GFX, pfnDriverCallback, fnRendererCallbacks*, 0x6FA8D258 - 0x6FA70000) //:$1D258
//
////D2FUNC(D2GFX, GetHwnd, HWND, __stdcall, (), -10027)
//D2FUNC(D2GFX, DrawShiftedImage, void, __stdcall, (CellContext* data, DWORD nXpos, DWORD nYpos, int nGamma, int nDrawMode, int nGlobalShift), -10073)
//D2FUNC(D2GFX, DrawVerticalCropImage, void, __stdcall, (CellContext* data, DWORD x, DWORD y, int nSkipLines, int nDrawLines, int nDrawMode), -10074)
//D2FUNC(D2GFX, DrawCellContext, void, __stdcall, (CellContext *context, int nXpos, int nYpos, DWORD dwGamma, int nDrawMode, BYTE* pPalette), -10072)
//D2FUNC(D2GFX, DrawFilledRect, void, __stdcall, (int nXposStart, int nYposStart, int nXposEnd, int nYposEnd, int nColor, int nTransparency), -10055)
//D2FUNC(D2GFX, DrawFilledRect2, void, __stdcall, (int nXposStart, int nYposStart, int nXposEnd, int nYposEnd, int nColor, int nTransparency), -10056)
//
//D2FUNC(D2COMMON, GetUnitBaseStat, DWORD, __stdcall, (UnitAny* Unit, DWORD Stat, DWORD Stat2), -10521)
//D2FUNC(D2COMMON, GetLevelThreshold, DWORD, __stdcall, (int nClass, DWORD dwLevel), -10628)
//D2FUNC(D2LANG, GetStringFromIndex, wchar_t*, __fastcall, (short nTblIndex_name_str), -10004)
//
//D2FUNC(D2COMMON, GetMaxLevel, int, __stdcall, (int nClass), -10629)
//D2FUNC(D2GFX, DrawLine, void, __stdcall, (int nXStart, int nYStart, int nXEnd, int nYEnd, BYTE nColor, BYTE nAlpha), -10057)
//D2FUNC(D2WIN, DrawFramedText, void, __fastcall, (wchar_t * wStr, int X, int Y, int nColor, int Centered), -10129)
//
//D2FUNC(D2COMMON, GetRoomFromUnit, room3*, __stdcall, (Unit* pPlayer), -10342)
//D2FUNC(D2COMMON, GetItemRecord, items_line*, __stdcall, (uint32_t guid), -10600)
//D2FUNC(D2COMMON, GetItemUniqueIndex, uint32_t, __stdcall, (unit3* item), -10732)
//D2FUNC(D2CLIENT, GetUnit, UnitAny *, __fastcall, (DWORD id, DWORD type), 0x869F0);
//D2FUNC(D2GAME, GetUnit, unit*, __fastcall, (Game* pGame, DWORD type, DWORD uniqueid), 0x8BB00);
//
//D2FUNC(D2GAME, GetGameFromClientId, Game*, __fastcall, (int32_t id), 0x94E0); //id 0 - current single player
//
//D2FUNC(D2CLIENT, ScmdItemUnitEx, UnitAny*, __fastcall, (void* aPacket), 0x14420);
//
//D2FUNC(D2WIN, DisplayBlendedText, int, __fastcall, (wchar_t* pString, int nXpos, int nYpos, int nColour, int bCentered, int BlendMode), -10118)
//D2FUNC(D2GFX, GetHwnd, HWND, __stdcall, (), -10027)
//D2FUNC(D2WIN, PrintString, void, __fastcall, (LPWSTR s, DWORD x, DWORD y, DWORD color, DWORD bfalse), -10117)
//D2FUNC(D2WIN, SetFont, DWORD, __fastcall, (DWORD fontID), -10127)
//D2FUNC(D2WIN, PrintPopup, void, __fastcall, (LPWSTR s, DWORD x, DWORD y, DWORD color, DWORD center), -10129)
//D2FUNC(D2GFX, GetResolutionMode, int, __stdcall, (), -10005)
//
//D2FUNC(D2CLIENT, PrintGameStringAtTopLeft, void, __fastcall, (wchar_t* text, int nColor), 0x7C600)
//D2FUNC(D2CLIENT, PrintGameStringAtBottomLeft, void, __fastcall, (wchar_t* text, int nColor), 0x7C950)
////D2VARPTR(D2CLIENT, nQuestPage, int, DLLBASE_D2CLIENT+0x116F15)
//D2FUNC(D2CLIENT, GetSelectedUnit, UnitAny *, __stdcall, (), 0x15A20)
////D2VARPTR(D2CLIENT, pInSocketablePatch, UnitAny *, DLLBASE_D2CLIENT+0x1158F0)
////D2VARPTR(D2CLIENT, pMonsterNameLifePatch1, UnitAny, DLLBASE_D2CLIENT+0xD9290)
////D2VARPTR(D2CLIENT, pMonsterNameLifePatch2, UnitAny, DLLBASE_D2CLIENT+0xD9298)
//D2FUNC(D2CLIENT, TestPvpFlag, DWORD, __fastcall, (DWORD planum1, DWORD planum2, DWORD flagmask), 0xAB900)
//D2FUNC(D2CLIENT, GetMonsterOwner, DWORD, __fastcall, (DWORD monnum), 0x11B00)
//D2FUNC(D2CLIENT, GetMonsterTxt, MonsterTxt*, __fastcall, (DWORD monno), 0xE570)
//
////F7(FAST, D2Win, 10046,10046,10046,10061,10075,10015,10022,10051,    void,        D2PrintLineOnTextBox,(void* screen, char* s, DWORD color) );
////F7(FAST, D2Win, 10117,10117,10117,10020,10064,10001,10150,10076,    void,        D2PrintString,(LPWSTR s, DWORD x, DWORD y, DWORD color, DWORD bfalse) );
////F7(FAST, D2Win, 10121,10121,10121,10034,10128,10132,10028,10150,    DWORD,        D2GetPixelLen,(LPWSTR s) );
////F7(FAST, D2Win, 10127,10127,10127,10141,10170,10010,10184,10047,    DWORD,        D2SetFont,(DWORD fontID) );
////F7(FAST, D2Win, 10129,10129,10129,10118,10039,10031,10085,10137,    void,        D2PrintPopup,(LPWSTR s, DWORD x, DWORD y, DWORD color, DWORD center) );
////F7(FAST, D2Win, 10131,10131,10131,00000,00000,00000,00000,00000,    void,        D2GetPixelRect,(LPWSTR s, DWORD* x, DWORD* y) );//6F8AB260
////F7(FAST, D2Win, 10132,10132,10132,00000,00000,00000,00000,00000,    DWORD,        D2PrintTextPopup,(LPWSTR s, DWORD x, DWORD y, DWORD uk, DWORD type, DWORD color) );//6F8AB080
////F7(STD,  D2Win, 10017,10017,10017,10147,10113,10098,10098,10164,    void*,        D2CreateTextBox,(DWORD* data) );
//
////FAST, D2Game,00000,00000,3F220,4ABE0,EC7E0,40B90,24950,CDE20,    DWORD,        D2SpawnMonster, (Game* ptGame, Room* ptRoom, DWORD zero1, DWORD x, DWORD y, DWORD minusOne, DWORD superuniqueID, DWORD zero2));//wrong param
//D2FUNC(D2GAME, D2SpawnMonster, DWORD, __fastcall, (Game* ptGame, Room* ptRoom, DWORD zero1, DWORD x, DWORD y, DWORD minusOne, DWORD superuniqueID, DWORD zero2), 0x3F220)
////D2FUNC(D2GAME, SpawnItem, int, __fastcall, (Game* pGame, Unit* pPlayer, DWORD dwCode, DWORD nIlvl, DWORD nQuality, DWORD dwOne), 0x65DF0)
////D2FUNC(D2CLIENT, GetPtPlayer, Unit*, __stdcall, (), 0x883D0)
//D2FUNC(D2COMMON, SetItemPage, int, __stdcall, (Unit* pItem, DWORD page), -10720)
////D2FUNC(D2CLIENT, GetPtRoom, Room*, __stdcall, (), 0x89370)
//D2FUNC(D2GAME, SpawnRandomMonsterPack, int, __fastcall, (Game* pGame, Room* ptRoom), 0x3AA70)
//
////D2FUNC(D2CLIENT, PrintChat, void, __fastcall, (LPWSTR s, DWORD n), 0x7C600)
//
//D2VAR(D2CLIENT, GetLocalPlayerName, const char*, 0x107810)
//D2FUNC(FOG, GetSavePath, void, __fastcall, (char* buffer, size_t bufferSize), -10115)
//D2FUNC(D2LAUNCH, DeleteSaveFile, HANDLE, __fastcall, (char* name, char* a2), 0x17C00)
//HANDLE __fastcall HookDeleteSaveFile(char* name, char* a2);
//
//D2FUNC(D2GAME, ServerCreate, void, __fastcall, (Game* pGame, Unit* pPlayer, DWORD dwGUID), 0x4B550)
//void __fastcall HookServerCreate(Game* pGame, Unit* pPlayer, DWORD dwGUID);
//
//D2FUNC(D2GAME, ServerDestroy, void, __fastcall, (Game* pGame, Unit* pPlayer), 0x4B630)
//void __fastcall HookServerDestroy(Game* pGame, Unit* pPlayer);
//
//void GameLoopPatch();
//void GameEndPatch();
//
//void __fastcall HookD2ClientTimerUpdate();
//VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
//int __stdcall handleServerUpdate (Unit* ptChar, WORD param);
//__inline void updateServer3(WORD p);
//__inline void updateServer7(BYTE type, DWORD num);
//__inline void updateServer9(BYTE type, DWORD num, DWORD command0x13);
//void BackToTown();
//
//
//// Convertion to 1.09
//struct s_shifting {
//    DWORD ptInventory;
//    DWORD ptSpecificData;
//    DWORD ptPYPlayerData;
//    DWORD ptGame;
//    DWORD ptClientGame;
//    DWORD ptSkills;
//    DWORD ptImage;
//    DWORD ptFrame;
//};
//extern s_shifting shifting;
//
////#ifdef MSVC
//#define FASTCALL __fastcall
////#else
////#define FASTCALL __msfastcall
////#endif
//#define STDCALL        __stdcall
//#define FCT_ASM(N) __declspec(naked) void N() {__asm{
//#define RANDOM(V) ((int)(rand()/(RAND_MAX+1.0)*(V)))
////#define RANDOM(V) (rand()%(V))
//
//#define INST_NOP 0x90
//#define INST_CALL 0xe8
//#define INST_JMP 0xe9
//#define INST_JMPR 0xeb
//#define INST_RET 0xc3
//
////shifting.ptPYPlayerData = *(DWORD*)((DWORD)D2InitPlayerData + V8(D2Common, 5D, 5D, 5D, 49, 49, 49, 49, 49, 48));
////shifting.ptSpecificData = 0x14;
////shifting.ptGame = 0x80;
////shifting.ptClientGame = 0x1A8;
////shifting.ptInventory = 0x60;
////shifting.ptSkills = 0xA8;
////shifting.ptImage = 0x34;
////shifting.ptFrame = 0x0;
//
////#define PY(C) (*(PYPlayerData**)((BYTE*)(ptChar)+shifting.ptPYPlayerData))
//#define PCPlayerData (*(PlayerData**)((DWORD)(ptChar)+shifting.ptSpecificData)) //->ptPlayerData
//#define PCGame (*(Game**)((DWORD)(ptChar)+shifting.ptGame)) //->ptGame
//#define PClientGame (*(Game**)((DWORD)(ptClient)+shifting.ptClientGame)) //ptClient->ptGame
//#define PCInventory (*(Inventory**)((DWORD)(ptChar)+shifting.ptInventory)) //->ptInventory
////#define PIItemData (*(ItemData**)((DWORD)(ptItem)+shifting.ptSpecificData)) //->ptItemData
////#define PCPY (*(PYPlayerData**)((DWORD)(ptChar)+shifting.ptPYPlayerData)) //->ptPYPlayerData
//#define PCPY ((PYPlayerData*)((DWORD)PCPlayerData+shifting.ptPYPlayerData)) //->ptPYPlayerData
//#define PCSkills (*(Skills**)((DWORD)(ptChar)+shifting.ptSkills)) //->ptSkills
//
//#define R8(Z,A,B,C,D,E,F,G,H,I) (offset_##Z + (version_##Z == V114d? 0x##I : (version_##Z == V113d? 0x##H : (version_##Z == V113c? 0x##G : (version_##Z == V112? 0x##F : (version_##Z == V111b? 0x##E : (version_##Z == V111? 0x##D : (version_##Z == V110? 0x##C : (version_##Z == V109d? 0x##B : 0x##A)))))))))
//#define V8(Z,A,B,C,D,E,F,G,H,I) (version_##Z == V114d? 0x##I : (version_##Z == V113d? 0x##H : (version_##Z == V113c? 0x##G : (version_##Z == V112? 0x##F : (version_##Z == V111b? 0x##E : (version_##Z == V111? 0x##D : (version_##Z == V110? 0x##C : (version_##Z == V109d? 0x##B : 0x##A))))))))
//
//
//
//
////D2FUNC(D2GFX, GetHwnd, HWND, __stdcall, (), -10027)
////D2FUNC(D2WIN, DisplayBlendedText, int, __fastcall, (wchar_t* pString, int nXpos, int nYpos, int nColour, int bCentered, int BlendMode), -10118)
////D2FUNC(D2WIN, PrintString, void, __fastcall, (LPWSTR s, DWORD x, DWORD y, DWORD color, DWORD bfalse), -10117)
////D2FUNC(D2WIN, SetFont, DWORD, __fastcall, (DWORD fontID), -10127)
////D2FUNC(D2WIN, PrintPopup, void, __fastcall, (LPWSTR s, DWORD x, DWORD y, DWORD color, DWORD center), -10129)
//
////F7(FAST, D2Win, 10046,10046,10046,10061,10075,10015,10022,10051,    void,        D2PrintLineOnTextBox,(void* screen, char* s, DWORD color) );
////F7(FAST, D2Win, 10117,10117,10117,10020,10064,10001,10150,10076,    void,        D2PrintString,(LPWSTR s, DWORD x, DWORD y, DWORD color, DWORD bfalse) );
////F7(FAST, D2Win, 10121,10121,10121,10034,10128,10132,10028,10150,    DWORD,        D2GetPixelLen,(LPWSTR s) );
////F7(FAST, D2Win, 10127,10127,10127,10141,10170,10010,10184,10047,    DWORD,        D2SetFont,(DWORD fontID) );
////F7(FAST, D2Win, 10129,10129,10129,10118,10039,10031,10085,10137,    void,        D2PrintPopup,(LPWSTR s, DWORD x, DWORD y, DWORD color, DWORD center) );
////F7(FAST, D2Win, 10131,10131,10131,00000,00000,00000,00000,00000,    void,        D2GetPixelRect,(LPWSTR s, DWORD* x, DWORD* y) );//6F8AB260
////F7(FAST, D2Win, 10132,10132,10132,00000,00000,00000,00000,00000,    DWORD,        D2PrintTextPopup,(LPWSTR s, DWORD x, DWORD y, DWORD uk, DWORD type, DWORD color) );//6F8AB080
////F7(STD,  D2Win, 10017,10017,10017,10147,10113,10098,10098,10164,    void*,        D2CreateTextBox,(DWORD* data) );
//
////(FAST, D2Client,     D210, D200, D990,00000,00000,00000,00000,00000,    void,        D2SendToServer3, (BYTE type, WORD p))
//
//D2FUNC(D2CLIENT, PrintChat, void, __fastcall, (LPWSTR s, DWORD n), 0x7C600)
//D2FUNC(D2CLIENT, GetPtPlayer, Unit*, __stdcall, (), 0x883D0)
//D2FUNC(D2CLIENT, GetPlayerUnit, UnitAny *, __stdcall, (), 0x883D0)
//D2FUNC(D2CLIENT, GetPtRoom, Room*, __stdcall, (), 0x89370)
//D2FUNC(D2CLIENT, SendToServer3, void, __fastcall, (BYTE type, WORD p), 0xD990)
//
////d2client Send transmute cube packet
////6FAECD28 | 53                   | push ebx                                                                                       | ebx == 0
////6FAECD29 | 53                   | push ebx                                                                                       | ebx == 0
////6FAECD2A | BA 18000000          | mov edx, 18                                                                                    |
////6FAECD2F | B1 4F                | mov cl, 4F                                                                                     |
////6FAECD31 | 891D 505BBB6F        | mov dword ptr ds:[6FBB5B50], ebx                                                               | ebx == 0
////6FAECD37 | E8 A40CFCFF          | call d2client.6FAAD9E0                                                                         |
//D2FUNC(D2CLIENT, SendToServer7, void, __fastcall, (BYTE type, DWORD num, DWORD unk1, DWORD unk2), 0xD9E0)
//D2FUNC(D2CLIENT, SendToServer9, void, __fastcall, (BYTE type, DWORD num, DWORD unk1), 0xDA40) // Interact
//
//D2FUNC(D2CLIENT, SendToServer13, void, __fastcall, (BYTE type, DWORD num, DWORD unk1, DWORD unk2, DWORD unk3), 0xDA70)
//D2FUNC(D2CLIENT, D2SendNPCPacket, void, __fastcall, (void* pPacket, size_t nSize), 0xD850)
//
//D2FUNC(D2CLIENT, RevealAutomapRoom, void, __fastcall, (DrlgRoom1 *room1, DWORD clipflag, AutomapLayer* layer), 0x2D180)
//
////6FACD180  0002D180    83EC 0C  sub esp,0C  ; D2Client.6FACD180(guessed Arg1)
//
//D2FUNC(D2GAME, SpawnItem, int, __fastcall, (Game* pGame, Unit* pPlayer, DWORD dwCode, DWORD nIlvl, DWORD nQuality, DWORD dwOne), 0x65DF0)
////D2FUNC(D2GAME, SpawnRandomMonsterPack, int, __fastcall, (Game* pGame, Room* pRoom), 0x3AA70)
////D2FUNC(D2GAME, PopulateRoom, int, __fastcall, (Game* pGame, Room* pRoom), 0x
//
////A7(FAST, D2Game, C380, C650, C710,41420,A0D50,7D220,8A3E0,DB780,    DWORD,        D2SendPacket, (void* ptNetClient, LPVOID pData, DWORD size));//EAX=ptNetClient [ESP]=pData
////A7(FAST, D2Game, D650, D920, DB50,44D00,A3F20,802E0,8D5F0,DD4F0,    void,        D2SetSkillBaseLevelOnClient, (void* ptClient, Unit* ptChar, DWORD skillID, DWORD sLvl, DWORD bRemove));//by EAX,ESI,EBX
//D2FUNC(D2GAME, SendPacket, DWORD, __fastcall, (void* ptNetClient, LPVOID pData, DWORD size), 0xC710)
//D2FUNC(D2GAME, SetSkillBaseLevelOnClient, void, __fastcall, (void* ptClient, Unit* ptChar, DWORD skillID, DWORD sLvl, DWORD bRemove), 0xDB50)
//D2FUNC(D2GAME, DispatchClientPacket, int, __fastcall, (Game* pGame, Unit* pPlayer, DWORD pPacket, int nSize), 0x59320)
//
//
////D2CreateItemUnit D2Game6FC501A0
////
////arg1 = ptPlayer
////arg2 = itemCode
////arg3 = ptGame
////arg4 = unknown (usually use 4 here)
////arg5 = quality
////arg6 = unkown (usually use 1 here)
////arg7 = unkown (usually use 1 here)
////arg8 = itemLevel
////arg9 = unknown (usually use 0 here)
////arg10 = unknown (usually use -1 here)
////arg11 = unknown (usually use 1 here)
//
////D2FUNC(D2GAME, CreateItemUnit, int, __fastcall, (Unit* pPlayer, DWORD itemCode, Game* ptGame, DWORD unknown4, DWORD quality, DWORD unkown6, DWORD unkown7, DWORD itemLevel, DWORD unknown9, DWORD unknown10, DWORD unknown11), 0x201A0)
//
////D2SpawnSuperUnique D2Game6FC6F690
////arg1: ptGame
////arg2: ptRoom
////arg3: pointX
////arg4: pointY
////arg5: hcIdx (from superuniques.txt)
////10F0F900     //0x1463      0x1413        03 rakanishu
//D2FUNC(D2GAME, SpawnSuperUnique, int, __fastcall, (Game* pGame, Room* pRoom, int pointX, int pointY, int hcIdx), 0x3F690)
//
////A7(FAST, D2Game,    7C2C0,7C7B0,8C2E0,00000,00000,00000,00000,00000,    NetClient*,    D2GetClient, (Unit* ptUnit, char* lpszErrFile, DWORD ErrLine));//6FCBC2E0
//
//D2FUNC(D2GAME, GetClient, NetClient*, __fastcall, (Unit* ptUnit, char* lpszErrFile, DWORD ErrLine), 0x8C2E0)
//
////F7(STD,  D2Common,10968,10968,10968,10700,10109,10904,10306,10007,    DWORD,        D2GetSkillLevel, (Unit* ptChar, SkillData* ptSkill, DWORD includingBonus));
////F7(STD,  D2Common,10953,10953,10953,10099,10255,10210,10302,10335,    void,        D2SetSkillBaseLevel,(Unit* ptChar, DWORD skillID, DWORD slvl, DWORD bRemove, char*, DWORD));
////F7(STD,  D2Common,11276,11276,11276,10254,10074,10111,10435,11081,    DWORD,        D2GetSkillCost, (Unit* ptChar, int skpoints, DWORD skillID, DWORD curSkillLevel));//not 10447
////F7(STD,  D2Common,10521,10521,10521,10733,10550,10494,10587,10216,    int,        D2GetPlayerBaseStat, (Unit* ptChar, DWORD statID, DWORD index));
//
//D2FUNC(D2COMMON, GetSkillLevel, DWORD, __stdcall, (Unit* ptChar, SkillData* ptSkill, DWORD includingBonus), -10968)
//D2FUNC(D2COMMON, SetSkillBaseLevel,    void, __stdcall, (Unit* ptChar, DWORD skillID, DWORD slvl, DWORD bRemove, char*, DWORD), -10953)
//D2FUNC(D2COMMON, GetSkillCost,    DWORD, __stdcall, (Unit* ptChar, int skpoints, DWORD skillID, DWORD curSkillLevel), -11276)
////D2FUNC(D2COMMON, SetItemPage, int, __stdcall, (Unit* pItem, DWORD page), -10720)
//D2FUNC(D2COMMON, GetCurrentRoom, Room*, __fastcall, (void* ptAct), -10056)
//D2FUNC(D2COMMON, GetXPosFromPath, int, __stdcall, (Path* ptPath), -10162)
//D2FUNC(D2COMMON, GetYPosFromPath, int,__stdcall, (Path* ptPath), -10163)
//D2FUNC(D2COMMON, GetMaxGoldBank, DWORD, __stdcall, (Unit* ptUnit), -10339)
//D2FUNC(D2COMMON, GetMaxGold, DWORD, __stdcall, (Unit* ptUnit), -10439)
//D2FUNC(D2COMMON, AddPlayerStat, void, __stdcall, (Unit* ptChar, DWORD statID, int amount, DWORD index), -10518)
//D2FUNC(D2COMMON, GetPlayerStat, int, __stdcall, (Unit* ptChar, DWORD statID, DWORD index), -10519)
//D2FUNC(D2COMMON, SetPlayerStat, void, __stdcall, (Unit* ptChar, DWORD statID, int amount, DWORD index), -10517)
//D2FUNC(D2COMMON, GetPlayerBaseStat, int, __stdcall, (Unit* ptChar, DWORD statID, DWORD index), -10521)
////D2FUNC(PLUGY, handleServerUpdate, int, __stdcall, (Unit* ptChar, WORD param), 0x1F490)
//D2FUNC(PLUGY, handleServerUpdate, int, __stdcall, (Unit* ptChar, WORD param), 0x1E250)
//
//
//// automap
//// 6FAC57B0 | A1 40BCBB6F | mov eax, dword ptr ds:[D2Client.6FBBBC40] //  1.09
//// 6FACBAF0 | A1 E419BB6F | mov eax, dword ptr ds:[D2Client.6FBB19E4] //  1.10
//
////020D90    6F620D90    10010    DRLG_GetLevelDefsRecord    int nLevelNo
////Unique    fastcall    (int nLevelNo)    __fastcall DRLG_GetLevelDefsRecord(int nLevelNo)
////6F620D90    DRLG_GetLevelDefsRecord    6F620D90    __fastcall DRLG_GetLevelDefsRecord(int nLevelNo)
//
////6FAC5884 $+0x25884 | E8 C3910900          | call <JMP.&D2Common#10010>  // 1.09    InitAutomapLayer
////6FACBBC4 $+0x2BBC4 | E8 CDF20900          | call <jmp.&D2Common#10010>  // 1.10    InitAutomapLayer
//
////6FAC594F $+0x2594F | 8B4424 14     mov eax,dword ptr ss:[esp+14]  // 1.09    InitAutomapLayer_END
////6FACBC8A $+0x2BC8A | 8B4424 14     mov eax,dword ptr ss:[esp+14]  // 1.10    InitAutomapLayer_END
//
////D2PTR(D2CLIENT, InitAutomapLayer, 0x6FAC5884)
////D2PTR(D2CLIENT, InitAutomapLayer_END, 0x6FAC594F)
//
////D2PTR(D2CLIENT, InitAutomapLayer, 0x2BBC4)
////D2PTR(D2CLIENT, InitAutomapLayer_END, 0x2BC8A)
//
//
//
////6FACDCE9  |.  3935 A419BB6F cmp dword ptr ds:[D2Client.6FBB19A4],esi
////6FAA8023  |.  A1 8479BA6F   mov eax,dword ptr ds:[D2Client.6FBA7984]
////1.09                                                        1.10    6FBBC200
////D2VARPTR(D2CLIENT, pPlayerUnit, UnitAny *, 0x6FBC63F8) == D2FUNC(D2CLIENT, GetPtPlayer, Unit*, __stdcall, (), 0x883D0)
////enum {DLLNO_D2CLIENT, DLLNO_D2COMMON, DLLNO_D2GFX, DLLNO_D2WIN, DLLNO_D2LANG, DLLNO_D2CMP};
////#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|(( ((b1)<0)?(b1):(b1)-DLLBASE_##a1 )<<8))
//////NOTE :- reference vars buggy
////#define D2FUNCPTR(d1,v1,t1,t2,o1) typedef t1 d1##_##v1##_t t2; d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
////#define D2VARPTR(d1,v1,t1,o1)     typedef t1 d1##_##v1##_t;    d1##_##v1##_t *p_##d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
//#define D2ASMPTR(DLL,NAME,OFFSET)        DWORD DLL##_##NAME  = GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);
//#define D2FUNCPTR(DLL,NAME,t1,t2,OFFSET) typedef t1 DLL##_##NAME##_t t2; DLL##_##NAME##_t *DLL##_##NAME = (DLL##_##NAME##_t *)GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);
//#define D2VARPTR(DLL,NAME,t1,OFFSET)     typedef t1 DLL##_##NAME##_t;    DLL##_##NAME##_t *p_##DLL##_##NAME = (DLL##_##NAME##_t *)GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);
////#define D2ASMPTR(d1,v1,o1)        DWORD d1##_##v1 = DLLOFFSET(d1,o1);
////#define D2PTR(DLL, NAME, OFFSET) DWORD NAME __attribute__((weak)) = GetDllOffset(#DLL, DLLBASE_##DLL, OFFSET);
////D2VAR(D2CLIENT, pDrlgAct, DrlgAct *, 0x6FBB0BE4)
////D2VAR(D2CLIENT, pPlayerUnit, UnitAny *, 0x6FBC63F8)
////
////D2VAR(D2CLIENT, pAutomapLayer, AutomapLayer *, 0x6FBBBC00)
////D2VAR(D2CLIENT, fAutomapSize, DWORD, 0x6FBBBBEC)
////D2VAR(D2CLIENT, fAutomapOn, DWORD, 0x6FBC48DC)
////D2VAR(D2CLIENT, xAutomap, int, 0x6FBBBC24)
////D2VAR(D2CLIENT, yAutomap, int, 0x6FBBBC28)
//
//////D2VAR(D2CLIENT, xMapShake, int, 0x6FBB9B0C)
//////D2VAR(D2CLIENT, yMapShake, int, 0x6FBB6454)
//////D2VAR(D2COMMON, nWeaponsTxt, int, 0x6FDE1D4C)
//////D2VAR(D2COMMON, nArmorTxt, int, 0x6FDE1D54)
//////D2VAR(D2CLIENT, nQuestPage, int, 0x6FBC11DD)
//////D2VAR(D2CLIENT, nDifficulty, BYTE, 0x6FBB0BBC)
//////D2VAR(D2COMMON, aTreasureClasses, TreasureClass *, 0x6FDE1C14)
//
////6FAB22A0  /.  A1 8479BA6F   mov eax,dword ptr ds:[D2Client.6FBA7984] //1.10
////6FAB2310  /.  A1 8479BA6F   mov eax,dword ptr ds:[D2Client.6FBA7984] //1.10
////D2FUNC(D2CLIENT, RecvCommand07, void, __fastcall, (BYTE* cmdbuf), 0x6FAB1C80) //1.09
////D2FUNC(D2CLIENT, RecvCommand08, void, __fastcall, (BYTE* cmdbuf), 0x6FAB1CF0) //1.09
//
//D2FUNC(D2CLIENT, RecvCommand07, void, __fastcall, (BYTE* cmdbuf), 0x122A0) //1.10
//D2FUNC(D2CLIENT, RecvCommand08, void, __fastcall, (BYTE* cmdbuf), 0x12310) //1.10
//
////034C10    6F634C10    10006    DRLG_InitLevel            DRLGLevel* pLevel
////usually called if pLevel->pRoomFirst returned by #10005 is null
////Unique    stdcall    (DRLGLevel* pLevel)    __stdcall DRLG_InitLevel(DRLGLevel* pLevel)
////6F634C10    DRLG_InitLevel    6F634C10    __stdcall DRLG_InitLevel(DRLGLevel* pLevel)
//D2FUNC(D2COMMON, InitDrlgLevel, void, __stdcall, (DrlgLevel* drlglevel), -10006)
//
////0349A0    6F6349A0    10005    DRLG_Get/InitLevelStructure            DRLGMisc* pMisc    int nLevel
////Unique    stdcall    (DRLGMisc* pMisc, int nLevel)
////__stdcall DRLG_Get/InitLevelStructure(DRLGMisc* pMisc, int nLevel)
////6F6349A0    DRLG_Get/InitLevelStructure    6F6349A0
////__stdcall DRLG_Get/InitLevelStructure(DRLGMisc* pMisc, int nLevel)
//D2FUNC(D2COMMON, GetDrlgLevel, DrlgLevel*, __stdcall, (DrlgMisc *drlgmisc, DWORD levelno), -10005)
//
////0318F0    6F6318F0    10626    DATA_TBL_GetObjectRecord            DWORD nObject
////Unique    stdcall    (DWORD nObject)    __stdcall DATA_TBL_GetObjectRecord(DWORD nObject)
////6F6318F0    DATA_TBL_GetObjectRecord    6F6318F0    __stdcall DATA_TBL_GetObjectRecord(DWORD nObject)
//D2FUNC(D2COMMON, GetObjectTxt, ObjectTxt*, __stdcall, (DWORD objno), -10626)
//
////D2FUNC(D2CLIENT, NewAutomapCell, AutomapCell*, __fastcall, (), 0x6FAC5700) // 1.09
////6FACBA40  /$  8B0D 9C19BB6F mov ecx,dword ptr ds:[D2Client.6FBB199C] // 1.10
//D2FUNC(D2CLIENT, NewAutomapCell, AutomapCell*, __fastcall, (), 0x2BA40) // 1.10
//
////D2FUNC(D2CLIENT, AddAutomapCell, void, __fastcall, (AutomapCell *cell, AutomapCell **node), 0x6FAC6A40) //1.09
////6FACCD50  /$  83EC 08       sub esp,8 // 1.10
//D2FUNC(D2CLIENT, AddAutomapCell, void, __fastcall, (AutomapCell *cell, AutomapCell **node), 0x2CD50) //1.10
//
//
//D2ASMPTR(D2CLIENT, InitAutomapLayerStart, 0x2BBC4)
//D2ASMPTR(D2CLIENT, InitAutomapLayerEnd, 0x2BC8A)
//
//D2FUNC(D2CLIENT, SetAutomapParty, void, __fastcall, (DWORD flag), 0x2B9F0)
//D2FUNC(D2CLIENT, SetAutomapNames, void, __fastcall, (DWORD flag), 0x2BA20)
//
////D2ASMPTR(D2CLIENT, InitAutomapLayer, DLLBASE_D2CLIENT+0x2BBC4)
////D2ASMPTR(D2CLIENT, InitAutomapLayer_END, DLLBASE_D2CLIENT+0x2BC8A)
//
////void __declspec(naked) __fastcall InitAutomapLayer(DWORD levelno)
////{
////    __asm {
////        //ecx = level no
////        lea eax, [eax]
////        mov ecx, eax
////        mov eax, D2CLIENT_InitAutomapLayer_END
////        mov byte ptr [eax], 0xC3
////        pushad
////        xor ebp, ebp
////        call D2CLIENT_InitAutomapLayer
////        popad
////        mov byte ptr [eax],0x8b
////        ret
////    }
////}
//
////#define D2CLIENT_pDrlgAct (*p_D2CLIENT_pDrlgAct)
////#define D2CLIENT_pPlayerUnit (*p_D2CLIENT_pPlayerUnit)
////
////#define D2CLIENT_pAutomapLayer (*p_D2CLIENT_pAutomapLayer)
////#define D2CLIENT_fAutomapSize (*p_D2CLIENT_fAutomapSize)
////#define D2CLIENT_fAutomapOn (*p_D2CLIENT_fAutomapOn)
////#define D2CLIENT_xAutomap (*p_D2CLIENT_xAutomap)
////#define D2CLIENT_yAutomap (*p_D2CLIENT_yAutomap)
//
////#define D2CLIENT_xMapShake (*p_D2CLIENT_xMapShake)
////#define D2CLIENT_yMapShake (*p_D2CLIENT_yMapShake)
////#define D2COMMON_nWeaponsTxt (*p_D2COMMON_nWeaponsTxt)
////#define D2COMMON_nArmorTxt (*p_D2COMMON_nArmorTxt)
////#define D2CLIENT_nQuestPage (*p_D2CLIENT_nQuestPage)
////#define D2CLIENT_nDifficulty (*p_D2CLIENT_nDifficulty)
////#define D2COMMON_aTreasureClasses (*p_D2COMMON_aTreasureClasses)
//
//
////D2VAR(D2CLIENT, pDrlgAct, DrlgAct*, 0x107984) //  0x6FBA7984
////D2VAR(D2CLIENT, pPlayerUnit, UnitAny*, 0x11C200) //  0x6FBBC200
////D2VAR(D2CLIENT, pAutomapLayer, AutomapLayer*, 0x1119A4)  //0x6FBB19A4
////D2VAR(D2CLIENT, fAutomapSize, DWORD, 0x111990)    //0x6FBB1990
////D2VAR(D2CLIENT, fAutomapOn, DWORD, 0x11A6D0) //  0x6FBBA6D0
////D2VAR(D2CLIENT, xAutomap, int, 0x1119C8)  //   0x6FBB19C8
////D2VAR(D2CLIENT, yAutomap, int, 0x1119CC)  //  0x6FBB19CC
//
////D2VAR(D2CLIENT, pAutomapCellsHeader, AutomapCellNode *, 0x1119A0)
////D2VAR(D2CLIENT, pDrlgAct, DrlgAct, 0x107984) //  0x6FBA7984
////D2VAR(D2CLIENT, pPlayerUnit, UnitAny, 0x11C200) //  0x6FBBC200
////D2VAR(D2CLIENT, pAutomapLayer, AutomapLayer, 0x1119A4)  //0x6FBB19A4  AutomapLayer*
////D2VAR(D2CLIENT, fAutomapSize, DWORD, 0x111990)    //0x6FBB1990
////D2VAR(D2CLIENT, fAutomapOn, DWORD, 0x11A6D0) //  0x6FBBA6D0
////D2VAR(D2CLIENT, xAutomap, int, 0x1119C8)  //   0x6FBB19C8
////D2VAR(D2CLIENT, yAutomap, int, 0x1119CC)  //  0x6FBB19CC
//
////D2VAR(D2CLIENT, pAutomapCellsHeader, AutomapCellNode *, 0x1119A0)
////D2VAR(D2CLIENT, pDrlgAct, DrlgAct, 0x107984) //  0x6FBA7984
////D2VAR(D2CLIENT, pPlayerUnit, UnitAny, 0x11C200) //  0x6FBBC200
////D2VAR(D2CLIENT, pAutomapLayer, AutomapLayer, 0x1119A4)  //0x6FBB19A4  AutomapLayer*
////D2VAR(D2CLIENT, fAutomapSize, DWORD, 0x111990)    //0x6FBB1990
////D2VAR(D2CLIENT, fAutomapOn, DWORD, 0x11A6D0) //  0x6FBBA6D0
////D2VAR(D2CLIENT, xAutomap, int, 0x1119C8)  //   0x6FBB19C8
////D2VAR(D2CLIENT, yAutomap, int, 0x1119CC)  //  0x6FBB19CC
//D2VARPTR(D2CLIENT, pDrlgAct, DrlgAct *, 0x107984)
//D2VARPTR(D2CLIENT, pAutomapCellsHeader, AutomapCellNode *, 0x1119A0)
//D2VARPTR(D2CLIENT, pAutomapLayer, AutomapLayer *, 0x1119A4)
//D2VARPTR(D2CLIENT, fAutomapOn, DWORD, 0x11A6D0)
//D2VARPTR(D2CLIENT, ptAutomap, POINT, 0x1119C8)
//D2VARPTR(D2CLIENT, ptOffset, POINT, 0x1119D8) // need to confirm
////D2VARPTR(D2CLIENT, nPtDivisor, int, DLLBASE_D2CLIENT+0x000D7BC0)
////D2VARPTR(D2COMMON, nWeaponsTxt, int, DLLBASE_D2COMMON+0x000AA2EC)
////D2VARPTR(D2COMMON, nArmorTxt, int, DLLBASE_D2COMMON+0x000AA2F4)
////D2VARPTR(D2COMMON, nUnknownVar_1, int, DLLBASE_D2COMMON+0x000AA2FC)
////D2VARPTR(D2CLIENT, xMapShake, int, DLLBASE_D2CLIENT+0x10F8C8)
////D2VARPTR(D2CLIENT, yMapShake, int, DLLBASE_D2CLIENT+0x10C20C)
////D2VARPTR(D2CLIENT, yPosition, int, DLLBASE_D2CLIENT+0x1119FC)
//
//#define D2CLIENT_pDrlgAct (*p_D2CLIENT_pDrlgAct)
//#define D2CLIENT_pAutomapLayer (*p_D2CLIENT_pAutomapLayer)
//#define D2CLIENT_pAutomapCellsHeader (*p_D2CLIENT_pAutomapCellsHeader)
//#define D2CLIENT_fAutomapOn (*p_D2CLIENT_fAutomapOn)
//#define D2CLIENT_ptAutomap (*p_D2CLIENT_ptAutomap)
//#define D2CLIENT_ptOffset (*p_D2CLIENT_ptOffset)
////#define D2CLIENT_xMapShake (*p_D2CLIENT_xMapShake)
////#define D2CLIENT_yMapShake (*p_D2CLIENT_yMapShake)
//
//D2FUNCPTR(D2GFX, DrawAutomapCell, void __stdcall, (CellContext *context, DWORD xpos, DWORD ypos, RECT *cliprect, DWORD bright), -0x275D)
//D2FUNCPTR(D2GFX, DrawAutomapCell2, void __stdcall, (CellContext *context, DWORD xpos, DWORD ypos, DWORD bright2, DWORD bright, BYTE *coltab), -0x2758)
//D2FUNCPTR(D2CMP, InitCellFile, void __stdcall, (void *cellfile, CellFile **outptr, char *srcfile, DWORD lineno, DWORD filever, char *filename), -0x2728)
//D2FUNCPTR(D2CMP, DeleteCellFile, void __stdcall, (CellFile *cellfile), -0x2730)
//D2FUNCPTR(D2WIN, SetTextSize, DWORD __fastcall, (DWORD size), -0x278F)
//D2FUNCPTR(D2WIN, GetTextWidthFileNo, DWORD __fastcall, (wchar_t *str, DWORD* width, DWORD* fileno), -0x2793)
//D2FUNCPTR(D2WIN, DrawText, void __fastcall, (wchar_t *str, int xpos, int ypos, DWORD col, DWORD unknown), -0x2785)
////D2FUNCPTR(D2GFX, DrawLine, void __stdcall, (int x1, int y1, int x2, int y2, DWORD col, DWORD unknown), -0x2749)
//D2FUNCPTR(D2CLIENT, SetExitApp, void __fastcall, (DWORD flag), 0x2040)
//D2FUNCPTR(D2GFX, GetScreenSize, DWORD __stdcall, (), -0x2715)
//D2ASMPTR(D2CLIENT, OverrideShrinePatch_ORIG, 0x10F990)
//D2ASMPTR(D2COMMON, GetLevelIdFromRoom, -0x2749)
//D2FUNCPTR(D2COMMON, GetLevelTxt, LevelTxt * __stdcall, (DWORD levelno), -0x2987)
//D2FUNCPTR(D2CLIENT, GetAutomapSize, DWORD __stdcall, (), 0x2F820)
//
//D2FUNCPTR(D2CLIENT, Interact, void __fastcall, (UINT8 byte1, UINT8 byte2, UINT8 byte3), 0xDA40) // 0x13 0x2 0x13 stash // Interact
//
//D2FUNC(D2GAME, SetInteractionInfo_8D840, void, __stdcall, (Unit* ptPlayer, int nInteractUnitType, DWORD dwIneractUnitGUID), 0x8D840)
//D2FUNC(D2GAME, FuncStash_8C2E0, DWORD, __fastcall, (Unit* ptPlayer, char* file, DWORD cons_0x3C2), 0x8C2E0)
//D2FUNC(D2GAME, OpenStash, void, __fastcall, (DWORD xz, UINT8 cons_0x10), 0xE0B0)
//
//D2FUNC(D2GAME, UpdateTraderItems, int, __fastcall, (DWORD p1, DWORD p2, DWORD p3, DWORD p4, DWORD p5), 0x99F40)

//eclass 98
//0x39F10
// end of file -------------------------
// end of file -----------------------------------------------------------------
#endif