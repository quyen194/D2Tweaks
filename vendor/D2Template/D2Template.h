/****************************************************************************
*                                                                           *
*   DLLmain.h                                                               *
*                                                                           *
*   Licensed under the Apache License, Version 2.0 (the "License");         *
*   you may not use this file except in compliance with the License.        *
*   You may obtain a copy of the License at                                 *
*                                                                           *
*   http://www.apache.org/licenses/LICENSE-2.0                              *
*                                                                           *
*   Unless required by applicable law or agreed to in writing, software     *
*   distributed under the License is distributed on an "AS IS" BASIS,       *
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
*   See the License for the specific language governing permissions and     *
*   limitations under the License.                                          *
*                                                                           *
*---------------------------------------------------------------------------*
*                                                                           *
*   https://github.com/olivier-verville/D2Template                          *
*                                                                           *
*   D2Template core file, do not modify unless you know what you're doing   *
*                                                                           *
*****************************************************************************/
#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

extern DWORD DLLBASE_BNCLIENT;
extern DWORD DLLBASE_D2CLIENT;
extern DWORD DLLBASE_D2CMP;
extern DWORD DLLBASE_D2COMMON;
extern DWORD DLLBASE_D2DDRAW;
extern DWORD DLLBASE_D2DIRECT3D;
extern DWORD DLLBASE_D2GAME;
extern DWORD DLLBASE_D2GDI;
extern DWORD DLLBASE_D2GFX;
extern DWORD DLLBASE_D2GLIDE;
extern DWORD DLLBASE_D2LANG;
extern DWORD DLLBASE_D2LAUNCH;
extern DWORD DLLBASE_D2MCPCLIENT;
extern DWORD DLLBASE_D2MULTI;
extern DWORD DLLBASE_D2NET;
extern DWORD DLLBASE_D2SOUND;
extern DWORD DLLBASE_D2WIN;
extern DWORD DLLBASE_FOG;
extern DWORD DLLBASE_STORM;
extern DWORD DLLBASE_IJL11;
extern DWORD DLLBASE_BINKW32;
extern DWORD DLLBASE_SMACKW32;
extern DWORD DLLBASE_UTILITY;
extern DWORD DLLBASE_PLUGY;
extern DWORD DLLBASE_D2EXPRES;
extern DWORD DLLBASE_SGD2FREERES;

#include "D2TemplateConstants.h"
#include "D2TemplateStructs.h"
#include "D2TemplatePtrs.h"
#include "D2TemplateVars.h"

struct DLLBaseStrc
{
    char* szName;
    DWORD dwAddress;
};

struct DLLPatchStrc
{
    int nDLL;
    DWORD dwAddress;
    DWORD dwData;
    BOOL bRelative;
    size_t nPatchSize;
};

enum D2TEMPLATE_DLL_FILES
{
    D2DLL_BINKW32,
    D2DLL_BNCLIENT,
    D2DLL_D2CLIENT,
    D2DLL_D2CMP,
    D2DLL_D2COMMON,
    D2DLL_D2DDRAW,
    D2DLL_D2DIRECT3D,
    D2DLL_D2GAME,
    D2DLL_D2GDI,
    D2DLL_D2GFX,
    D2DLL_D2GLIDE,
    D2DLL_D2LANG,
    D2DLL_D2LAUNCH,
    D2DLL_D2MCPCLIENT,
    D2DLL_D2MULTI,
    D2DLL_D2NET,
    D2DLL_D2SOUND,
    D2DLL_D2WIN,
    D2DLL_FOG,
    D2DLL_IJL11,
    D2DLL_SMACKW32,
    D2DLL_STORM,
	D2DLL_UTILITY,
	D2DLL_PLUGY,
	D2DLL_D2EXPRES,
	D2DLL_SGD2FREERES,
    D2DLL_INVALID
};


static DLLBaseStrc gptDllFiles[] =
{
	{"Binkw32.dll",         NULL},
	{"BnClient.dll",        NULL},
	{"D2Client.dll",        NULL},
	{"D2CMP.dll",           NULL},
	{"D2Common.dll",        NULL},
	{"D2DDraw.dll",         NULL},
	{"D2Direct3D.dll",      NULL},
	{"D2Game.dll",          NULL},
	{"D2Gdi.dll",           NULL},
	{"D2Gfx.dll",           NULL},
	{"D2Glide.dll",         NULL},
	{"D2Lang.dll",          NULL},
	{"D2Launch.dll",        NULL},
	{"D2MCPClient.dll",     NULL},
	{"D2Multi.dll",         NULL},
	{"D2Net.dll",           NULL},
	{"D2Sound.dll",         NULL},
	{"D2Win.dll",           NULL},
	{"Fog.dll",             NULL},
	{"Ijl11.dll",           NULL},
	{"SmackW32.dll",        NULL},
	{"Storm.dll",           NULL},
	{"Utility.dll",         NULL},
	{"Plugy.dll",           NULL},
	{"d2expres.dll",        NULL},
	{"SGD2FreeRes.dll",     NULL},
};

#define PATCH_JMP               0x000000E9
#define PATCH_CALL              0x000000E8
#define PATCH_RETN              0x000000C3
#define PATCH_RETN4             0x000004C2
#define PATCH_RETN8             0x000008C2
#define PATCH_RETN0C            0x00000CC2
#define PATCH_RETN10            0x000010C2
#define PATCH_RETN14            0x000014C2
#define PATCH_RETN18            0x000018C2
#define PATCH_RETN1C            0x00001CC2
#define PATCH_NOPBLOCK          0x90909090

#define FCT_ASM(N) __declspec(naked) void N() {__asm{
#include "D2TemplatePatch.h"

void __fastcall D2TEMPLATE_FatalError(char* szMessage);
int __fastcall D2TEMPLATE_Init();
BOOL __fastcall D2TEMPLATE_ApplyPatch(const DLLPatchStrc* hPatch);
BOOL __fastcall D2TEMPLATE_LoadModules();
int __fastcall D2TEMPLATE_GetDebugPrivilege();
DWORD __fastcall GetDllOffset(char* ModuleName, DWORD BaseAddress, int Offset);
char* __fastcall GetModuleExt(char* ModuleName);

static void* g_ret = 0;

#define JMP_TO_RVA(BASE, RVA) __asm		\
{										\
__asm push eax							\
__asm pushfd							\
__asm mov eax, BASE						\
__asm add eax, RVA						\
__asm mov [g_ret], eax					\
__asm popfd								\
__asm pop eax							\
__asm jmp [g_ret]						\
}

#define RET_TO_RVA(BASE, RVA) __asm		\
{										\
__asm push BASE							\
__asm pushfd							\
__asm add dword ptr [esp+4], RVA		\
__asm popfd								\
__asm retn								\
}