#include <Windows.h>

#include <MinHook.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <memory>
#include <common/ini.h>
#include <d2tweaks/common/common.h>
#include <d2tweaks/server/server.h>
#include <d2tweaks/client/client.h>
#include <DllNotify.h>
#include <D2Template.h>

//#pragma comment(lib, "vcruntime.lib")
#pragma comment(lib, "mincore_downlevel.lib")

void init_log() {
	const auto console_err = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
	const auto logPath = "d2tweaks.log";
	const auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath);

	// ReSharper disable once CppSmartPointerVsMakeFunction
	auto logger = std::shared_ptr<spdlog::logger>(new spdlog::logger("d2tweaks", { file, console_err }));

	logger->flush_on(spdlog::level::trace);

	set_default_logger(logger);

#ifndef NDEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	spdlog::info("Log system initialized");
}

void initialize(uint32_t param) {
#ifndef NDEBUG
	AllocConsole();
	// ReSharper disable once CppDeprecatedEntity
	freopen("CONOUT$", "w", stdout);
	// ReSharper disable once CppDeprecatedEntity
	freopen("CONOUT$", "w", stderr);
#endif

	init_log();
	D2TEMPLATE_Init();

	if (MH_Initialize() != MH_OK) {
		MessageBox(nullptr, "Cannot initialize hook system!", "Error", MB_OK | MB_ICONSTOP);
		exit(0);
	}

	singleton<d2_tweaks::common::common>::instance().init();
	singleton<d2_tweaks::server::server>::instance().init();
	singleton<d2_tweaks::client::client>::instance().init();

	MH_EnableHook(nullptr);
}

//extern "C" {
//#pragma comment(linker, "/EXPORT:_Init@0=_init_plugy@0")
//	__declspec(dllexport) void __stdcall init_plugy() {
//		initialize();
//	}
//
//#pragma comment(linker, "/EXPORT:_Init@4=_init_d2_mod@4")
//	__declspec(dllexport) void* __stdcall init_d2_mod(const char* a1) {
//		initialize();
//		return nullptr;
//	}
//}

HANDLE g_hThread = 0;
// ReSharper disable once CppInconsistentNaming
BOOL APIENTRY DllMain(HMODULE moduleHandle,
	DWORD reason,
	LPVOID reserved) {
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch (reason) {
	case DLL_PROCESS_ATTACH:
	{
		//dllnotify::DllNotify::Init_Dllnotify();
		DisableThreadLibraryCalls(moduleHandle);
		initialize(0);
		break;
	}

	case DLL_PROCESS_DETACH:
	{
		if (g_hThread != NULL) {
#ifndef NDEBUG
			FreeConsole();
#endif
			CloseHandle(g_hThread);
			MH_Uninitialize();
			//dllnotify::DllNotify::Uninit_Dllnotify();
		}
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	default:
		break;
	}
	return true;
}
