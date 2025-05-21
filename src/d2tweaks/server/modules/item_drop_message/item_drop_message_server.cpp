#include <d2tweaks/server/modules/item_drop_message/item_drop_message_server.h>
#include <d2tweaks/server/server.h>

#include <spdlog/spdlog.h>
#include <diablo2/d2game.h>
#include <diablo2/d2common.h>
#include <diablo2/d2lang.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/room.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>
#include <diablo2/structures/player_data.h>
#include <diablo2/structures/unit.h>

#include <common/autopickup_lootfilter.h>

MODULE_INIT(item_drop_message)

static const uint32_t m_nCountItemListKeys = 30;
static const uint32_t m_nCountItemTypesKeys = 10;

static uint32_t m_nCountItemListAll = 0;
static char m_aacItemList[m_nCountItemListKeys][MAX_STRING_LENGHT] = { 0 };
static char m_acItemListAll[MAX_STRING_LENGHT * m_nCountItemListKeys] = { 0 };
static char m_acItemListAllTemp[MAX_STRING_LENGHT * m_nCountItemListKeys] = { 0 };

static uint32_t m_nCountItemTypesAll = 0;
static char m_aacItemTypes[m_nCountItemTypesKeys][MAX_STRING_LENGHT] = { 0 };
static char m_acItemTypesAll[MAX_STRING_LENGHT * m_nCountItemTypesKeys] = { 0 };
static char m_acItemTypesAllTemp[MAX_STRING_LENGHT * m_nCountItemTypesKeys] = { 0 };

static char m_acBuffer[1024] = { 0 };

static item_code* m_stItemList;
static item_type* m_stItemTypes;

void d2_tweaks::server::modules::item_drop_message::init() {
	uint32_t dwLenght = 0;
	char acPathToIni[MAX_PATH] = { 0 };
	const char* pcIniFile = "\\d2tweaks.ini";

	GetCurrentDirectory(MAX_PATH, acPathToIni);
	lstrcat(acPathToIni, pcIniFile);

	if (GetPrivateProfileInt("modules", "ItemDropMessage", 1, acPathToIni) != FALSE) {
		for (uint32_t i = 0; i < m_nCountItemListKeys; i++) {
			sprintf_s(m_acBuffer, sizeof(m_acBuffer), "ItemList%d", i + 1);
			dwLenght = GetPrivateProfileString("ItemDropMessage", m_acBuffer, 0, m_aacItemList[i], MAX_STRING_LENGHT - 1, acPathToIni);
			if (dwLenght != 0) {
				lstrcat(m_acItemListAll, m_aacItemList[i]);
				lstrcat(m_acItemListAll, "|");
			}
		}

		for (uint32_t i = 0; i < m_nCountItemTypesKeys; i++) {
			sprintf_s(m_acBuffer, sizeof(m_acBuffer), "ItemTypeList%d", i + 1);
			dwLenght = GetPrivateProfileString("ItemDropMessage", m_acBuffer, 0, m_aacItemList[i], MAX_STRING_LENGHT - 1, acPathToIni);
			if (dwLenght != 0) {
				lstrcat(m_acItemTypesAll, m_aacItemTypes[i]);
				lstrcat(m_acItemTypesAll, "|");
			}
		}

		/////// Parse ItemCode
		dwLenght = lstrlen(m_acItemListAll);
		memcpy(m_acItemListAllTemp, m_acItemListAll, dwLenght + 1);
		// count the total number of items
		char* token_string_itemcode = strtok(m_acItemListAllTemp, " ,|");
		while (token_string_itemcode)
		{
			m_nCountItemListAll++;
			token_string_itemcode = strtok(NULL, " ,|");
		}

		// Create an array of structures equal to the number of items
		m_stItemList = (item_code*)malloc(m_nCountItemListAll * sizeof(item_code));
		memset(m_stItemList, 0, m_nCountItemListAll * sizeof(item_code));

		// Parse each item
		memcpy(m_acItemListAllTemp, m_acItemListAll, dwLenght + 1);
		token_string_itemcode = strtok(m_acItemListAllTemp, " ,|");

		for (uint32_t i = 0; token_string_itemcode != 0; i++)
		{
			uint32_t cur_string_length = lstrlen(token_string_itemcode);
			m_stItemList[i].code0 = token_string_itemcode[0];
			m_stItemList[i].code1 = token_string_itemcode[1];
			m_stItemList[i].code2 = token_string_itemcode[2];

			if (token_string_itemcode[3] == ':') {
				// first index quality = 1
				for (uint32_t q = 1; q <= 9; q++) {
					m_stItemList[i].qualityinclude[q] = FALSE;
				}

				if (cur_string_length <= 13) { //for example jew:123456789
					// p = 4 --> this is first digit after ':'
					for (uint32_t p = 4; p <= cur_string_length; p++) {
						if (token_string_itemcode[p] == 0) {
							break;
						}
						if (token_string_itemcode[p] >= '0' && token_string_itemcode[p] <= '9') {
							m_stItemList[i].qualityinclude[token_string_itemcode[p] - 0x30] = TRUE;
						}
					}
				}
			}

			if (token_string_itemcode[3] == '-') {
				// first index quality = 1
				for (uint32_t q = 1; q <= 9; q++) {
					m_stItemList[i].qualityinclude[q] = TRUE;
				}

				if (cur_string_length <= 13) { //for example jew:123456789
					// p = 4 --> this is first digit after '-'
					for (uint32_t p = 4; p <= cur_string_length; p++) {
						if (token_string_itemcode[p] == 0) {
							break;
						}
						if (token_string_itemcode[p] >= '0' && token_string_itemcode[p] <= '9') {
							m_stItemList[i].qualityinclude[token_string_itemcode[p] - 0x30] = FALSE;
						}
					}
				}
			}

			if (token_string_itemcode[3] != '-' && token_string_itemcode[3] != ':')
			{
				// first index quality = 1
				for (uint32_t q = 1; q <= 9; q++) {
					m_stItemList[i].qualityinclude[q] = TRUE;
				}
			}

			token_string_itemcode = strtok(NULL, " ,|");
		}

		/// Parse ItemType
		dwLenght = lstrlen(m_acItemTypesAll);
		memcpy(m_acItemTypesAllTemp, m_acItemTypesAll, dwLenght + 1);
		char* token_string_itemtype_code = strtok(m_acItemTypesAllTemp, ",|");
		while (token_string_itemtype_code)
		{
			m_nCountItemTypesAll++;
			token_string_itemtype_code = strtok(NULL, ",|");
		}

		m_stItemTypes = (item_type*)malloc(m_nCountItemTypesAll * sizeof(item_type));
		memset(m_stItemTypes, 0, m_nCountItemTypesAll * sizeof(item_type));

		memcpy(m_acItemTypesAllTemp, m_acItemTypesAll, dwLenght + 1);
		token_string_itemtype_code = strtok(m_acItemTypesAllTemp, ",|");
		for (uint32_t i = 0; token_string_itemtype_code != 0; i++)
		{
			uint32_t cur_itemtypes_string_length = lstrlen(token_string_itemtype_code);
			//m_stItemTypes[i].type0 = token_string_itemtype_code[0];
			//m_stItemTypes[i].type1 = token_string_itemtype_code[1];
			//m_stItemTypes[i].type2 = token_string_itemtype_code[2];
			//m_stItemTypes[i].type3 = token_string_itemtype_code[3];

			m_stItemTypes[i].dwtype = *(DWORD*)token_string_itemtype_code;

			if (token_string_itemtype_code[4] == ':') {
				// first index quality = 1
				for (uint32_t q = 1; q <= 9; q++) {
					m_stItemTypes[i].qualityinclude[q] = FALSE;
				}

				if (cur_itemtypes_string_length <= 14) { //for example tors:123456789
					// p = 5 --> this is first digit after ':'
					for (uint32_t p = 5; p <= cur_itemtypes_string_length; p++) {
						if (token_string_itemtype_code[p] == 0) {
							break;
						}
						if (token_string_itemtype_code[p] >= '0' && token_string_itemtype_code[p] <= '9') {
							m_stItemTypes[i].qualityinclude[token_string_itemtype_code[p] - 0x30] = TRUE;
						}
					}
				}
			}

			if (token_string_itemtype_code[4] == '-') {
				// first index quality = 1
				for (uint32_t q = 1; q <= 9; q++) {
					m_stItemTypes[i].qualityinclude[q] = TRUE;
				}

				if (cur_itemtypes_string_length <= 14) { //for example armo:123456789
					// p = 5 --> this is first digit after '-'
					for (uint32_t p = 5; p <= cur_itemtypes_string_length; p++) {
						if (token_string_itemtype_code[p] == 0) {
							break;
						}
						if (token_string_itemtype_code[p] >= '0' && token_string_itemtype_code[p] <= '9') {
							m_stItemTypes[i].qualityinclude[token_string_itemtype_code[p] - 0x30] = FALSE;
						}
					}
				}
			}

			if (token_string_itemtype_code[4] != '-' && token_string_itemtype_code[4] != ':')
			{
				// first index quality = 1
				for (uint32_t q = 1; q <= 9; q++) {
					m_stItemTypes[i].qualityinclude[q] = TRUE;
				}
			}

			token_string_itemtype_code = strtok(NULL, ",|");
		}

		//singleton<server>::instance().register_tick_handler(this);
		singleton<server>::instance().register_packet_handler(common::MESSAGE_TYPE_ITEM_DROPPED_INFO, this);
	}
}

void d2_tweaks::server::modules::item_drop_message::tick(diablo2::structures::game* game,
	diablo2::structures::unit* unit) {
	static common::item_pickup_info_sc packet;
	static auto& instance = singleton<d2_tweaks::server::server>::instance();
	if (!game || !unit)
		return;

	if (unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER)
		return;
}

bool d2_tweaks::server::modules::item_drop_message::handle_packet(diablo2::structures::game* game, diablo2::structures::unit* player, common::packet_header* packet) {
	const auto income_item_dropped_packet = static_cast<common::item_dropped_info_cs*>(packet);
	static common::item_dropped_info_sc response_item_dropped_packet;

	switch (income_item_dropped_packet->message_type)
	{
	case common::message_types_t::MESSAGE_TYPE_ITEM_DROPPED_INFO:
		auto current_unit = diablo2::d2_game::get_server_unit(game, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM, income_item_dropped_packet->item_id);
		if (current_unit != 0) {
			if (current_unit->type == diablo2::structures::unit_type_t::UNIT_TYPE_ITEM) {
				if (current_unit->item_data != 0) {
					auto record = diablo2::d2_common::get_item_record(current_unit->data_record_index);
					uint32_t quality = diablo2::d2_common::get_item_quality(current_unit);
					wchar_t* string_wc = diablo2::d2_lang::get_string_from_index(record->name_str);
					static char string_mb[260] = { 0 };

					const auto itemtype_record = diablo2::d2_common::get_item_type_record(record->type);
					auto itemtype_record_equiv1 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv1);
					auto itemtype_record_equiv2 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv2);

					wcstombs(string_mb, string_wc, 256);

					memset(response_item_dropped_packet.arr_itemtype_codestr_equivstr, 0, sizeof response_item_dropped_packet.arr_itemtype_codestr_equivstr);
					// Item type code is always the first element in the array
					*(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[0] = *(DWORD*)itemtype_record->code;
					// Index of the second code in the array
					uint32_t index_arr_itemtype = 1;

					if (itemtype_record_equiv1) {
						//
						if (*(DWORD*)itemtype_record_equiv1->code != 0x20202020) {
							// Save the first previously obtained equiv1
							*(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv1->code;
							index_arr_itemtype++;
							// Expand all equiv1 into the array
							for (index_arr_itemtype; itemtype_record_equiv1->equiv1 != 0; index_arr_itemtype++) {
								// Get the next one
								itemtype_record_equiv1 = diablo2::d2_common::get_item_type_record(itemtype_record_equiv1->equiv1);
								if (*(DWORD*)itemtype_record_equiv1->code != 0x20202020) {
									*(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv1->code;
								}
								else break;
							}
						}
					}

					if (itemtype_record_equiv2) {
						if (*(DWORD*)itemtype_record_equiv2->code != 0x20202020) {
							// Save the first previously obtained equiv2
							*(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv2->code;
							index_arr_itemtype++;
							// Expand all equiv2 into the array
							for (index_arr_itemtype; itemtype_record_equiv2->equiv2 != 0; index_arr_itemtype++) {
								// Get the next equiv2
								itemtype_record_equiv2 = diablo2::d2_common::get_item_type_record(itemtype_record_equiv2->equiv2);
								if (*(DWORD*)itemtype_record_equiv2->code != 0x20202020) {
									*(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv2->code;
								}
								else break;
							}
						}
					}

					if (GetKeyState(VK_SCROLL) != 0) {
						response_item_dropped_packet.item = income_item_dropped_packet->item_id;
						response_item_dropped_packet.code[0] = record->string_code[0];
						response_item_dropped_packet.code[1] = record->string_code[1];
						response_item_dropped_packet.code[2] = record->string_code[2];

						// total number of entries in the itemtypes array
						response_item_dropped_packet.index_arr_itemtype = index_arr_itemtype;

						response_item_dropped_packet.quality = quality;
						response_item_dropped_packet.showthis = TRUE;

						memcpy(response_item_dropped_packet.namestr, string_mb, 128);
						//spdlog::debug("[item_drop_message_s] Code={} Type.Code={} Type.Equiv1={} Type.Equiv2={}", response_item_dropped_packet.code, response_item_dropped_packet.itemtype_code, response_item_dropped_packet.itemtype_equiv1, response_item_dropped_packet.itemtype_equiv2);
						singleton<server>::instance().send_packet(player->player_data->net_client, &response_item_dropped_packet, sizeof response_item_dropped_packet);
						break;
					}

					for (uint32_t i = 0; i < m_nCountItemTypesAll; i++)
					{
						for (uint32_t count = 0; count < index_arr_itemtype; count++)
						{
							if (*(DWORD*)response_item_dropped_packet.arr_itemtype_codestr_equivstr[count] == (DWORD)m_stItemTypes[i].dwtype)
							{
								if (m_stItemTypes[i].qualityinclude[quality] == TRUE)
								{
									response_item_dropped_packet.item = income_item_dropped_packet->item_id;
									response_item_dropped_packet.code[0] = record->string_code[0];
									response_item_dropped_packet.code[1] = record->string_code[1];
									response_item_dropped_packet.code[2] = record->string_code[2];

									response_item_dropped_packet.index_arr_itemtype = index_arr_itemtype;
									response_item_dropped_packet.quality = quality;
									response_item_dropped_packet.showthis = TRUE;

									memcpy(response_item_dropped_packet.namestr, string_mb, 128);
									//spdlog::debug("[item_drop_message_s_itemtypes] Code={} Type.Code={} Type.Equiv1={} Type.Equiv2={}", response_item_dropped_packet.code, response_item_dropped_packet.itemtype_code, response_item_dropped_packet.itemtype_equiv1, response_item_dropped_packet.itemtype_equiv2);
									singleton<server>::instance().send_packet(player->player_data->net_client, &response_item_dropped_packet, sizeof response_item_dropped_packet);
									goto L1;
								}
								else {
									response_item_dropped_packet.showthis = FALSE;
								}
							}
						}
					}

					for (uint32_t i = 0; i < m_nCountItemListAll; i++)
					{
						if (record->string_code[0] == m_stItemList[i].code0 &&
							record->string_code[1] == m_stItemList[i].code1 &&
							record->string_code[2] == m_stItemList[i].code2)
						{
							if (m_stItemList[i].qualityinclude[quality] == TRUE)
							{
								response_item_dropped_packet.item = income_item_dropped_packet->item_id;
								response_item_dropped_packet.code[0] = record->string_code[0];
								response_item_dropped_packet.code[1] = record->string_code[1];
								response_item_dropped_packet.code[2] = record->string_code[2];

								response_item_dropped_packet.index_arr_itemtype = index_arr_itemtype;
								response_item_dropped_packet.quality = quality;
								response_item_dropped_packet.showthis = TRUE;

								memcpy(response_item_dropped_packet.namestr, string_mb, 128);
								//spdlog::debug("[item_drop_message_s] Code={} Type.Code={} Type.Equiv1={} Type.Equiv2={}", response_item_dropped_packet.code, response_item_dropped_packet.itemtype_code, response_item_dropped_packet.itemtype_equiv1, response_item_dropped_packet.itemtype_equiv2);
								singleton<server>::instance().send_packet(player->player_data->net_client, &response_item_dropped_packet, sizeof response_item_dropped_packet);
							}
						}
						else {
							response_item_dropped_packet.showthis = FALSE;
						}
					}

				L1:;
				}
			}
		}
		break;
	}

	return true;
}
