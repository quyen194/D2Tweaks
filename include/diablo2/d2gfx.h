#pragma once

#include <Windows.h>
#include <cstdint>

namespace diablo2 {
	namespace structures {
		struct gfxdata;
	}

	class d2_gfx {
	public:
		static char* get_base();

		static bool check_perspective_mode();
		static bool check_perspective_coords(int32_t x, int32_t y);
		static int32_t adjust_perspective_coords(int32_t x, int32_t y, int32_t* adjustX, int32_t* adjustY);
		static int32_t get_resolution_mode();
		static void draw_image(structures::gfxdata* data, uint32_t x, uint32_t y, int32_t gamma, int32_t drawType, void* palette);
		static void draw_filled_rect(int left, int top, int right, int bottom, DWORD color, int transTbl);
	};
}