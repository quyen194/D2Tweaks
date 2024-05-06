#pragma once

#include <diablo2/structures/item_data.h>

#include <cstring>

namespace d2_tweaks {
	namespace client {
		namespace modules {
			struct loot_filter_settings {
				size_t size; //struct size

				bool alt_only;
				bool show_gold;
				bool show_runes;
				bool show_gems;

				bool m_show_r01;
				bool m_show_r02;
				bool m_show_r03;
				bool m_show_r04;
				bool m_show_r05;
				bool m_show_r06;
				bool m_show_r07;
				bool m_show_r08;
				bool m_show_r09;
				bool m_show_r10;
				bool m_show_r11;
				bool m_show_r12;
				bool m_show_r13;
				bool m_show_r14;
				bool m_show_r15;
				bool m_show_r16;
				bool m_show_r17;
				bool m_show_r18;
				bool m_show_r19;
				bool m_show_r20;
				bool m_show_r21;
				bool m_show_r22;
				bool m_show_r23;
				bool m_show_r24;
				bool m_show_r25;
				bool m_show_r26;
				bool m_show_r27;
				bool m_show_r28;
				bool m_show_r29;
				bool m_show_r30;
				bool m_show_r31;
				bool m_show_r32;
				bool m_show_r33;


				bool quality_settings[static_cast<size_t>(diablo2::structures::item_quality_t::ITEM_QUALITY_COUNT)];

				char reserved[1004];

				static loot_filter_settings& get();

				static void save(const char* name);
				static void load(const char* name);
				static void remove(const char* name);

			private:
				loot_filter_settings() : size(sizeof(loot_filter_settings)),
					alt_only(false), show_gold(true), show_runes(true), show_gems(true), reserved{}
				{
					memset(quality_settings, 0x1, sizeof quality_settings);
				}
			};
		}
	}
}
