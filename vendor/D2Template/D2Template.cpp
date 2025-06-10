
#include <Windows.h>

#define _D2VARS_H

#include "D2Template.h"
#include "D2TemplatePatch.h"

extern DWORD DLLBASE_BNCLIENT = 0;
extern DWORD DLLBASE_D2CLIENT = 0;
extern DWORD DLLBASE_D2CMP = 0;
extern DWORD DLLBASE_D2COMMON = 0;
extern DWORD DLLBASE_D2DDRAW = 0;
extern DWORD DLLBASE_D2DIRECT3D = 0;
extern DWORD DLLBASE_D2GAME = 0;
extern DWORD DLLBASE_D2GDI = 0;
extern DWORD DLLBASE_D2GFX = 0;
extern DWORD DLLBASE_D2GLIDE = 0;
extern DWORD DLLBASE_D2LANG = 0;
extern DWORD DLLBASE_D2LAUNCH = 0;
extern DWORD DLLBASE_D2MCPCLIENT = 0;
extern DWORD DLLBASE_D2MULTI = 0;
extern DWORD DLLBASE_D2NET = 0;
extern DWORD DLLBASE_D2SOUND = 0;
extern DWORD DLLBASE_D2WIN = 0;
extern DWORD DLLBASE_FOG = 0;
extern DWORD DLLBASE_STORM = 0;
extern DWORD DLLBASE_IJL11 = 0;
extern DWORD DLLBASE_BINKW32 = 0;
extern DWORD DLLBASE_SMACKW32 = 0;
extern DWORD DLLBASE_UTILITY = 0;
extern DWORD DLLBASE_PLUGY = 0;
extern DWORD DLLBASE_D2EXPRES = 0;
extern DWORD DLLBASE_SGD2FREERES = 0;

void __fastcall D2TEMPLATE_FatalError(char* szMessage) {
  MessageBoxA(NULL, szMessage, "", MB_OK | MB_ICONERROR);
  TerminateProcess(GetCurrentProcess(), -1);
}

BOOL __fastcall D2TEMPLATE_ApplyPatch(const DLLPatchStrc* hPatch) {
  void* hGame = GetCurrentProcess();

  while (hPatch->nDLL != D2DLL_INVALID) {
    int nReturn = 0;
    int nDLL = hPatch->nDLL;
    if (nDLL < 0 || nDLL >= D2DLL_INVALID) return FALSE;

    DWORD dwAddress = hPatch->dwAddress;
    if (!dwAddress) return FALSE;

    DWORD dwBaseAddress = gptDllFiles[nDLL].dwAddress;
    if (!dwBaseAddress) return FALSE;

    dwAddress += dwBaseAddress;

    DWORD dwData = hPatch->dwData;
    if (hPatch->bRelative) {
      dwData = dwData - (dwAddress + sizeof(dwData));
    }

    void* hAddress = (void*)dwAddress;
    DWORD dwOldPage;

    if (hPatch->nPatchSize > 0) {
      BYTE Buffer[1024] = { 0 };

      for (size_t i = 0; i < hPatch->nPatchSize; i++)
        Buffer[i] = (BYTE)dwData;

      VirtualProtect(hAddress, hPatch->nPatchSize, PAGE_EXECUTE_READWRITE, &dwOldPage);
      nReturn = WriteProcessMemory(hGame, hAddress, &Buffer, hPatch->nPatchSize, 0);
      VirtualProtect(hAddress, hPatch->nPatchSize, dwOldPage, 0);
    }
    else {
      VirtualProtect(hAddress, sizeof(dwData), PAGE_EXECUTE_READWRITE, &dwOldPage);
      nReturn = WriteProcessMemory(hGame, hAddress, &dwData, sizeof(dwData), 0);
      VirtualProtect(hAddress, sizeof(dwData), dwOldPage, 0);
    }

    if (nReturn == 0) return FALSE;

    hPatch++;
  }

  return TRUE;
}

BOOL __fastcall D2TEMPLATE_LoadModules() {
  for (int i = 0; i < D2DLL_INVALID; i++) {
    DLLBaseStrc* hDllFile = &gptDllFiles[i];

    void* hModule = GetModuleHandleA(hDllFile->szName);

    // if (!hModule) {
    //   hModule = LoadLibraryA(hDllFile->szName);
    // }

    hDllFile->dwAddress = (DWORD)hModule;
  }

  DLLBASE_BNCLIENT = (DWORD)GetModuleHandleA("Bnclient.dll");
  DLLBASE_D2CLIENT = (DWORD)GetModuleHandleA("D2Client.dll");
  DLLBASE_D2CMP = (DWORD)GetModuleHandleA("D2CMP.dll");
  DLLBASE_D2COMMON = (DWORD)GetModuleHandleA("D2Common.dll");
  DLLBASE_D2DDRAW = (DWORD)GetModuleHandleA("D2DDraw.dll");
  DLLBASE_D2DIRECT3D = (DWORD)GetModuleHandleA("D2Direct3D.dll");
  DLLBASE_D2GAME = (DWORD)GetModuleHandleA("D2Game.dll");
  DLLBASE_D2GDI = (DWORD)GetModuleHandleA("D2Gdi.dll");
  DLLBASE_D2GFX = (DWORD)GetModuleHandleA("D2Gfx.dll");
  DLLBASE_D2GLIDE = (DWORD)GetModuleHandleA("D2Glide.dll");
  DLLBASE_D2LANG = (DWORD)GetModuleHandleA("D2Lang.dll");
  DLLBASE_D2LAUNCH = (DWORD)GetModuleHandleA("D2Launch.dll");
  DLLBASE_D2MCPCLIENT = (DWORD)GetModuleHandleA("D2MCPClient.dll");
  DLLBASE_D2MULTI = (DWORD)GetModuleHandleA("D2Multi.dll");
  DLLBASE_D2NET = (DWORD)GetModuleHandleA("D2Net.dll");
  DLLBASE_D2SOUND = (DWORD)GetModuleHandleA("D2Sound.dll");
  DLLBASE_D2WIN = (DWORD)GetModuleHandleA("D2Win.dll");
  DLLBASE_FOG = (DWORD)GetModuleHandleA("Fog.dll");
  DLLBASE_STORM = (DWORD)GetModuleHandleA("Storm.dll");
  DLLBASE_IJL11 = (DWORD)GetModuleHandleA("ijl11.dll");
  DLLBASE_BINKW32 = (DWORD)GetModuleHandleA("binkw32.dll");
  DLLBASE_SMACKW32 = (DWORD)GetModuleHandleA("SmackW32.dll");
  DLLBASE_UTILITY = (DWORD)GetModuleHandleA("Utility.dll");
  DLLBASE_PLUGY = (DWORD)GetModuleHandleA("Plugy.dll");
  DLLBASE_D2EXPRES = (DWORD)GetModuleHandleA("d2expres.dll");
  DLLBASE_SGD2FREERES = (DWORD)GetModuleHandleA("SGD2FreeRes.dll");

  return TRUE;
}

int __fastcall D2TEMPLATE_GetDebugPrivilege() {
  void* hToken;
  LUID luid;
  TOKEN_PRIVILEGES tokenPrivileges;

  if (OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken) == 0) {
    D2TEMPLATE_FatalError("OpenProcessToken Failed");
    return 0;
  }

  if (LookupPrivilegeValue(0, SE_DEBUG_NAME, &luid) == 0) {
    D2TEMPLATE_FatalError("LookupPrivilegeValue Failed");
    CloseHandle(hToken);
    return 0;
  }

  tokenPrivileges.PrivilegeCount = 1;
  tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  tokenPrivileges.Privileges[0].Luid = luid;
  if (AdjustTokenPrivileges(hToken, 0, &tokenPrivileges, sizeof(tokenPrivileges), 0, 0) == 0) {
    D2TEMPLATE_FatalError("AdjustTokenPrivileges Failed");
    CloseHandle(hToken);
    return 0;
  }

  CloseHandle(hToken);
  return 1;
}

DWORD __fastcall GetDllOffset(char* ModuleName, DWORD BaseAddress, int Offset) {
  if (!BaseAddress)
    BaseAddress = (DWORD)LoadLibraryA(GetModuleExt(ModuleName));

  if (Offset < 0)
    return (DWORD)GetProcAddress((HMODULE)BaseAddress, (LPCSTR)(-Offset));

  return BaseAddress + Offset;
}

char* __fastcall GetModuleExt(char* ModuleName) {
  char DLLExt[] = ".dll";
  char DLLName[32] = { 0 };
  strcpy(DLLName, ModuleName);
  return strcat(DLLName, DLLExt);
}

int __fastcall D2TEMPLATE_Init() {
  // D2TEMPLATE_GetDebugPrivilege();

  if (!D2TEMPLATE_LoadModules()) {
    D2TEMPLATE_FatalError("Failed to load modules");
    return 0;
  }

  return 1;
}
