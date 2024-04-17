#pragma once

#include <Windows.h>
#include <cstdint>

namespace hooking {
	enum mh_status_t {
		// Unknown error. Should not be returned.
		MH_UNKNOWN = -1,

		// Successful.
		MH_OK = 0,

		// MinHook is already initialized.
		MH_ERROR_ALREADY_INITIALIZED,

		// MinHook is not initialized yet, or already uninitialized.
		MH_ERROR_NOT_INITIALIZED,

		// The hook for the specified target function is already created.
		MH_ERROR_ALREADY_CREATED,

		// The hook for the specified target function is not created yet.
		MH_ERROR_NOT_CREATED,

		// The hook for the specified target function is already enabled.
		MH_ERROR_ENABLED,

		// The hook for the specified target function is not enabled yet, or already
		// disabled.
		MH_ERROR_DISABLED,

		// The specified pointer is invalid. It points the address of non-allocated
		// and/or non-executable region.
		MH_ERROR_NOT_EXECUTABLE,

		// The specified target function cannot be hooked.
		MH_ERROR_UNSUPPORTED_FUNCTION,

		// Failed to allocate memory.
		MH_ERROR_MEMORY_ALLOC,

		// Failed to change the memory protection.
		MH_ERROR_MEMORY_PROTECT,

		// The specified module is not loaded.
		MH_ERROR_MODULE_NOT_FOUND,

		// The specified function is not found.
		MH_ERROR_FUNCTION_NOT_FOUND
	};

	namespace details {
		mh_status_t hook(void* target, void* detour, void** original);
	}

	template<typename TOrig>
	mh_status_t hook(void* target, void* detour, TOrig** original) {
		return details::hook(target, detour, reinterpret_cast<void**>(original));
	}

	template<size_t TOrdinal, typename TOrig>
	mh_status_t hook(void* base, void* detour, TOrig** original) {
		auto fn = GetProcAddress(reinterpret_cast<HMODULE>(base),
								 reinterpret_cast<LPCSTR>(TOrdinal));

		return hook(fn, detour, original);
	}

	intptr_t get_executable_memory(void* origin, size_t size);
	void* set_call(void* address, void* function, size_t stubSize = 7);
	void* set_jmp(void* address, void* function, size_t stubSize = 7);
	void* get_call(void* address);
}
