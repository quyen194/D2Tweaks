﻿#include <d2tweaks/client/modules/item_drop_message/item_drop_message_client.h>
#include <d2tweaks/client/client.h>

#include <d2tweaks/common/protocol.h>

#include <common/hooking.h>
#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>
#include <spdlog/spdlog.h>
#include <diablo2/d2client.h>
#include <diablo2/d2win.h>
#include <diablo2/d2gfx.h>
#include <diablo2/d2common.h>
#include <common/autopickup_lootfilter.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>

MODULE_INIT(item_drop_message)

struct message {
	bool active;
	uint64_t start;
	int32_t text_width;
	uint32_t quality;
	wchar_t item_text[256];
	char item_code[4];
	// количество строк, длина каждой строки
	char arr_itemtype_codestr_equivstr[20][5];

	message(bool active, uint32_t hires_posx, uint32_t hires_posy, uint32_t lowres_posx, uint32_t lowres_posy, uint32_t quality)
		: active(active),
		text_width(0), quality(quality) {
		start = 0;//GetTickCount();
		memset(item_text, 0x00, sizeof item_text);
		memset(item_code, 0x00, sizeof item_code);
		memset(arr_itemtype_codestr_equivstr, 0x00, sizeof arr_itemtype_codestr_equivstr);
	}

	message() : active(0), text_width(0), quality(0) {
		start = 0;//GetTickCount();
		memset(item_text, 0x00, sizeof item_text);
		memset(item_code, 0x00, sizeof item_code);
		memset(arr_itemtype_codestr_equivstr, 0x00, sizeof arr_itemtype_codestr_equivstr);
	}
};

static uint32_t m_nMsgCount = 0;
static message m_stMsg[32];

static wchar_t* m_apwcColorStr[17] = { L"ÿc0", L"ÿc1", L"ÿc2", L"ÿc3", L"ÿc4", L"ÿc5", L"ÿc6", L"ÿc7", L"ÿc8", L"ÿc9", L"ÿc:", L"ÿc;", L"ÿc0", L"ÿc0", L"ÿc0", L"ÿc0", L"ÿc0" };
static wchar_t* m_apwcQualityStr[10] = { L"", L"(Cracked)", L"(Normal)", L"(Superior)", L"(Magic)", L"(Set)", L"(Rare)", L"(Unique)", L"(Crafted)", L"(Tempered)" };
static char* m_apcQualityStr[10] = { "", "(Cracked)", "(Normal)", "(Superior)", "(Magic)", "(Set)", "(Rare)", "(Unique)", "(Crafted)", "(Tempered)" };

static uint32_t m_nHookMethod = 1;
static uint32_t m_anQualityColor[10] = { 0 };

static char m_acSecondString[1024] = { 0 };
static wchar_t m_awcSecondString[1024] = { 0 };
static wchar_t m_awcCode[4] = { 0 };

static wchar_t m_awcItemtypeCode[8] = { 0 };
static wchar_t m_aawcItemtypeEquiv[10][8] = { 0 };

static void(__fastcall* fn_GamePacketReceivedIntercept)(d2_tweaks::common::packet_header* packet, size_t size);

class draw_item_menu final : public d2_tweaks::ui::menu {
public:
	draw_item_menu() {
		menu::set_visible(true);
		menu::set_enabled(true);
	}

	void draw() override {
		for (uint32_t i = 0; i < m_nMsgCount; i++) {
			m_stMsg[i].active = false;
		}
	}
};

void d2_tweaks::client::modules::item_drop_message::GamePacketReceivedIntercept(uint8_t* packet, size_t size) {
	if (packet == 0 || size == 0)
		return;

	if ((packet[0] == 0x9C || (packet[0] == 0x9D)) && (packet[1] == 0x0 || packet[1] == 0x2 || packet[1] == 0x3)) {
		static d2_tweaks::common::item_dropped_info_cs info;
		info.item_id = *(WORD*)&packet[4];

		spdlog::debug("[GamePacketReceived] ItemID {0} Pack {1} {2} {3} {4} Message {5} Size {6}", info.item_id, (void*)packet[4], (void*)packet[5], (void*)packet[6], (void*)packet[7], (void*)packet[1], size);

		diablo2::d2_client::send_to_server(&info, sizeof common::item_dropped_info_cs);
	}

	return;
}

__declspec (naked) void d2_tweaks::client::modules::item_drop_message::GamePacketReceivedInterceptASM() {
	__asm
	{
		// call our function (__fastcall)
		pushad;
		pushfd;
		mov edx, ecx;
		mov ecx, ebp;
		call d2_tweaks::client::modules::item_drop_message::GamePacketReceivedIntercept;
		popfd;
		popad;

		jmp[fn_GamePacketReceivedIntercept]
	}
}

void d2_tweaks::client::modules::item_drop_message::init_early() {
}

void d2_tweaks::client::modules::item_drop_message::init() {
	char acPathToIni[MAX_PATH] = { 0 };
	const char* pcIniFile = "\\d2tweaks.ini";

	GetCurrentDirectory(MAX_PATH, acPathToIni);
	lstrcat(acPathToIni, pcIniFile);

	if (GetPrivateProfileInt("modules", "ItemDropMessage", 1, acPathToIni) != FALSE) {
		m_nMsgCount = GetPrivateProfileInt("ItemDropMessage", "MaxNumMessages", 8, acPathToIni);
		m_nHookMethod = GetPrivateProfileInt("ItemDropMessage", "HookMethod", 1, acPathToIni);

		m_anQualityColor[0] = GetPrivateProfileInt("ItemDropMessage", "DefaultColor", 0, acPathToIni);
		m_anQualityColor[1] = GetPrivateProfileInt("ItemDropMessage", "Cracked", 0, acPathToIni);
		m_anQualityColor[2] = GetPrivateProfileInt("ItemDropMessage", "Normal", 0, acPathToIni);
		m_anQualityColor[3] = GetPrivateProfileInt("ItemDropMessage", "Superior", 0, acPathToIni);
		m_anQualityColor[4] = GetPrivateProfileInt("ItemDropMessage", "Magic", 3, acPathToIni);
		m_anQualityColor[5] = GetPrivateProfileInt("ItemDropMessage", "Set", 12, acPathToIni);
		m_anQualityColor[6] = GetPrivateProfileInt("ItemDropMessage", "Rare", 9, acPathToIni);
		m_anQualityColor[7] = GetPrivateProfileInt("ItemDropMessage", "Unique", 7, acPathToIni);
		m_anQualityColor[8] = GetPrivateProfileInt("ItemDropMessage", "Crafted", 8, acPathToIni);
		m_anQualityColor[9] = GetPrivateProfileInt("ItemDropMessage", "Tempered", 10, acPathToIni);

		GetPrivateProfileString("ItemDropMessage", "SecondString", "", m_acSecondString, 1023, acPathToIni);
		mbstowcs(m_awcSecondString, m_acSecondString, 1023);

		// d2hackit hook d2client:$0x15123
		if (m_nHookMethod == 1) {
			hooking::hook(diablo2::d2_client::get_base() + 0x15116, GamePacketReceivedInterceptASM, reinterpret_cast<void**>(&fn_GamePacketReceivedIntercept));
		}

		if (m_nHookMethod == 2) {
			hooking::hook(diablo2::d2_client::get_base() + 0x1511A, GamePacketReceivedInterceptASM, reinterpret_cast<void**>(&fn_GamePacketReceivedIntercept));
		}

		singleton<client>::instance().register_packet_handler(common::message_types_t::MESSAGE_TYPE_ITEM_DROPPED_INFO, this);
		singleton<ui::ui_manager>::instance().add_menu(new draw_item_menu());
	}
}

void d2_tweaks::client::modules::item_drop_message::handle_packet(common::packet_header* packet) {
	const auto info = static_cast<common::item_pickup_info_sc*>(packet);
	const auto item_dropped_packet = static_cast<common::item_dropped_info_sc*>(packet);

	spdlog::debug("[MyPacketReceived] Message {} Size {}", item_dropped_packet->message_type, sizeof item_dropped_packet);

	if (item_dropped_packet->message_type == common::message_types_t::MESSAGE_TYPE_ITEM_DROPPED_INFO) {
		for (uint32_t i = 0; i < m_nMsgCount; i++) {
			if (m_stMsg[i].active == false) {
				static wchar_t buffer[512];
				static char buffermb[512];

				m_stMsg[i].active = true;
				m_stMsg[i].item_code[0] = item_dropped_packet->code[0];
				m_stMsg[i].item_code[1] = item_dropped_packet->code[1];
				m_stMsg[i].item_code[2] = item_dropped_packet->code[2];
				m_stMsg[i].quality = item_dropped_packet->quality;
				m_stMsg[i].start = GetTickCount();

				mbstowcs(buffer, (const char*)item_dropped_packet->namestr, 128);
				memcpy(m_stMsg[i].item_text, buffer, 128);
				memcpy(m_stMsg[i].arr_itemtype_codestr_equivstr, item_dropped_packet->arr_itemtype_codestr_equivstr, sizeof item_dropped_packet->arr_itemtype_codestr_equivstr);

				//сделать в названии предмета замену всех переносов на пробелы
				for (uint32_t c = 0; c <= 128; c++) {
					if (m_stMsg[i].item_text[c] == '\n') {
						m_stMsg[i].item_text[c] = ' ';
					}
				}

				if (lstrlenW(m_awcSecondString) != 0) {
					swprintf_s(buffer, L"%s%s%s", m_stMsg[i].item_text, m_apwcColorStr[m_anQualityColor[0]], m_awcSecondString);
				}
				else {
					swprintf_s(buffer, L"%s", m_stMsg[i].item_text);
				}

				// вывести сообщение о любом предмете
				if (GetKeyState(VK_SCROLL) != 0) {
					mbstowcs(m_awcCode, m_stMsg[i].item_code, 3);
					mbstowcs(m_awcItemtypeCode, m_stMsg[i].arr_itemtype_codestr_equivstr[0], 4);
					mbstowcs(m_aawcItemtypeEquiv[0], m_stMsg[i].arr_itemtype_codestr_equivstr[1], 4);
					mbstowcs(m_aawcItemtypeEquiv[1], m_stMsg[i].arr_itemtype_codestr_equivstr[2], 4);
					mbstowcs(m_aawcItemtypeEquiv[2], m_stMsg[i].arr_itemtype_codestr_equivstr[3], 4);
					mbstowcs(m_aawcItemtypeEquiv[3], m_stMsg[i].arr_itemtype_codestr_equivstr[4], 4);
					mbstowcs(m_aawcItemtypeEquiv[4], m_stMsg[i].arr_itemtype_codestr_equivstr[5], 4);
					mbstowcs(m_aawcItemtypeEquiv[5], m_stMsg[i].arr_itemtype_codestr_equivstr[6], 4);
					mbstowcs(m_aawcItemtypeEquiv[6], m_stMsg[i].arr_itemtype_codestr_equivstr[7], 4);
					mbstowcs(m_aawcItemtypeEquiv[7], m_stMsg[i].arr_itemtype_codestr_equivstr[8], 4);
					mbstowcs(m_aawcItemtypeEquiv[8], m_stMsg[i].arr_itemtype_codestr_equivstr[9], 4);
					mbstowcs(m_aawcItemtypeEquiv[9], m_stMsg[i].arr_itemtype_codestr_equivstr[10], 4);

					swprintf_s(buffer, L"%s  ÿc0Code: %s   Quality: %i %s   Type: %s   Equiv: %s %s %s %s %s %s %s %s %s %s",
						m_stMsg[i].item_text,
						m_awcCode,
						m_stMsg[i].quality,
						m_apwcQualityStr[m_stMsg[i].quality],
						m_awcItemtypeCode,
						m_aawcItemtypeEquiv[0],
						m_aawcItemtypeEquiv[1],
						m_aawcItemtypeEquiv[2],
						m_aawcItemtypeEquiv[3],
						m_aawcItemtypeEquiv[4],
						m_aawcItemtypeEquiv[5],
						m_aawcItemtypeEquiv[6],
						m_aawcItemtypeEquiv[7],
						m_aawcItemtypeEquiv[8],
						m_aawcItemtypeEquiv[9]);
				}

				diablo2::d2_client::print_chat(buffer, (diablo2::ui_color_t)m_anQualityColor[m_stMsg[i].quality]);

				char* nul = "";
				wcstombs(buffermb, m_stMsg[i].item_text, 256);
				spdlog::info("{} {}   ItemList |{}:{}|   ItemTypeCode |{}:{}|   ItemTypeEquiv |{}:{}| |{}:{}| |{}:{}| |{}:{}| |{}:{}| |{}:{}| |{}:{}| |{}:{}| |{}:{}| |{}:{}|",
					buffermb,
					m_apcQualityStr[m_stMsg[i].quality],

					m_stMsg[i].item_code, m_stMsg[i].quality,

					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[0] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[0] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[0] != 0) ? m_stMsg[i].quality : 0,

					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[1] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[1] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[1] != 0) ? m_stMsg[i].quality : 0,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[2] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[2] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[2] != 0) ? m_stMsg[i].quality : 0,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[3] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[3] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[3] != 0) ? m_stMsg[i].quality : 0,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[4] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[4] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[4] != 0) ? m_stMsg[i].quality : 0,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[5] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[5] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[5] != 0) ? m_stMsg[i].quality : 0,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[6] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[6] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[6] != 0) ? m_stMsg[i].quality : 0,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[7] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[7] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[7] != 0) ? m_stMsg[i].quality : 0,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[8] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[8] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[8] != 0) ? m_stMsg[i].quality : 0,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[9] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[9] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[9] != 0) ? m_stMsg[i].quality : 0,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[10] != 0) ? m_stMsg[i].arr_itemtype_codestr_equivstr[10] : nul,
					(*(DWORD*)m_stMsg[i].arr_itemtype_codestr_equivstr[10] != 0) ? m_stMsg[i].quality : 0);
				break;
			}
		}
	}
}