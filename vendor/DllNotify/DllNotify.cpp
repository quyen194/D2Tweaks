#include "DllNotify.h"
#include "D2Template.h"

using namespace dllnotify;

DllNotify::DllNotify() 
{
}

static bool m_d2expres_loaded;
static bool m_sgd2freeres_loaded;

bool DllNotify::is_d2expres() {
	return m_d2expres_loaded;
}

bool DllNotify::is_sgd2freeres() {
	return m_sgd2freeres_loaded;
}

VOID CALLBACK DllNotify::LdrDllNotification(
	_In_      ULONG NotificationReason,
	_In_      PCLDR_DLL_NOTIFICATION_DATA NotificationData,
	_In_opt_  PVOID Context
)
{
	if (NotificationReason == LDR_DLL_NOTIFICATION_REASON_LOADED)
	{
		// D2Expres.dll want load?
		if (lstrcmpiW(NotificationData->Loaded.BaseDllName->Buffer, L"d2expres.dll") == 0) {
			DLLBASE_D2EXPRES = (DWORD)NotificationData->Loaded.DllBase;
			m_d2expres_loaded = true;
		}

		// SGD2FreeRes.dll want load?
		if (lstrcmpiW(NotificationData->Loaded.BaseDllName->Buffer, L"SGD2FreeRes.dll") == 0) {
			DLLBASE_SGD2FREERES = (DWORD)NotificationData->Loaded.DllBase;
			m_sgd2freeres_loaded = true;
		}
	}
}

static PVOID cookie;
static DllNotify::PLDR_REGISTER_DLL_NOTIFICATION    p_LdrRegisterDllNotification;
static DllNotify::PLDR_UNREGISTER_DLL_NOTIFICATION  p_LdrUnRegisterDllNotification;

BOOL DllNotify::Init_Dllnotify()
{
	NTSTATUS status = 1;
	HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");

	p_LdrRegisterDllNotification = (PLDR_REGISTER_DLL_NOTIFICATION)GetProcAddress(ntdll, "LdrRegisterDllNotification");
	p_LdrUnRegisterDllNotification = (PLDR_UNREGISTER_DLL_NOTIFICATION)GetProcAddress(ntdll, "LdrUnRegisterDllNotification");

	if (p_LdrRegisterDllNotification)
	{
		status = p_LdrRegisterDllNotification(
			0, // must be zero
			LdrDllNotification,
			0, // context,
			&cookie
		);
	}

	return status == 0;
}


BOOL DllNotify::Uninit_Dllnotify()
{
	NTSTATUS status = 1;

	if (p_LdrUnRegisterDllNotification)
	{
		status = p_LdrUnRegisterDllNotification(cookie);
		cookie = 0;
	}

	return status == 0;
}
