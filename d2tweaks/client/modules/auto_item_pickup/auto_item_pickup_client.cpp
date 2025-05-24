#include <d2tweaks/client/modules/auto_item_pickup/auto_item_pickup_client.h>
#include <d2tweaks/client/client.h>

#include <common/hooking.h>
#include <d2tweaks/common/common.h>
#include <d2tweaks/common/protocol.h>
#include <d2tweaks/common/asset_manager.h>

#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>
#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/controls/button.h>

#include <diablo2/d2client.h>
#include <diablo2/d2win.h>
#include <diablo2/d2gfx.h>
#include <diablo2/d2common.h>
#include <diablo2/d2lang.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/room.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>
#include <diablo2/structures/player_data.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>

#include <common/autopickup_lootfilter.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <common/ini.h>
#include <DllNotify.h>
#include <D2Template.h>

MODULE_INIT(auto_item_pickup)

static int32_t m_iDistance = 0;
static uint32_t m_bAutoItemPickupEnabled = 0;
static uint32_t m_nTick = 0;

static bool m_bToggleAutoItemPickup = false;
static bool m_bInventoryFull = false;

static item_code* m_stItemList;
static item_type* m_stItemTypes;

static uint32_t m_nCountItemListAll = 0;
static uint32_t m_nCountItemTypesAll = 0;

static const uint32_t m_nCountItemListKeys = 30;
static const uint32_t m_nCountItemTypesKeys = 10;

static char* m_apcItemList[m_nCountItemListKeys] = { 0 };
static char* m_apcItemTypes[m_nCountItemTypesKeys] = { 0 };

static char* m_pcItemListAll;
static char* m_pcItemListAllTemp;

static char* m_pcItemTypesAll;
static char* m_pcItemTypesAllTemp;

static char m_acBuffer[1024] = { 0 };
static char m_acPathToIni[MAX_PATH] = { 0 };
static const char* m_pcIniFile = "\\d2tweaks.ini";

void d2_tweaks::client::modules::auto_item_pickup::init_early() {
}

void ReloadFilters(char* szPathToIni) {
	uint32_t dwLenght = 0;

	m_nCountItemListAll = 0;
	m_nCountItemTypesAll = 0;

	free(m_stItemList);
	free(m_stItemTypes);

	free(m_pcItemListAllTemp);
	free(m_pcItemListAll);
	free(m_pcItemTypesAll);
	free(m_pcItemTypesAllTemp);

	m_pcItemListAll = (char*)malloc(MAX_STRING_LENGHT * m_nCountItemListKeys);
	m_pcItemListAllTemp = (char*)malloc(MAX_STRING_LENGHT * m_nCountItemListKeys);
	m_pcItemTypesAll = (char*)malloc(MAX_STRING_LENGHT * m_nCountItemTypesKeys);
	m_pcItemTypesAllTemp = (char*)malloc(MAX_STRING_LENGHT * m_nCountItemTypesKeys);

	memset(m_pcItemListAll, 0, MAX_STRING_LENGHT * m_nCountItemListKeys);
	memset(m_pcItemListAllTemp, 0, MAX_STRING_LENGHT * m_nCountItemListKeys);
	memset(m_pcItemTypesAll, 0, MAX_STRING_LENGHT * m_nCountItemTypesKeys);
	memset(m_pcItemTypesAllTemp, 0, MAX_STRING_LENGHT * m_nCountItemTypesKeys);

	for (uint32_t i = 0; i < m_nCountItemListKeys; i++) {
		free(m_apcItemList[i]);
		m_apcItemList[i] = (char*)malloc(MAX_STRING_LENGHT);
		memset(m_apcItemList[i], 0, MAX_STRING_LENGHT);

		sprintf_s(m_acBuffer, sizeof(m_acBuffer), "ItemList%d", i + 1);
		dwLenght = GetPrivateProfileString("AutoItemPickup", m_acBuffer, 0, m_apcItemList[i], MAX_STRING_LENGHT - 1, szPathToIni);
		if (dwLenght != 0) {
			lstrcat(m_pcItemListAll, m_apcItemList[i]);
			lstrcat(m_pcItemListAll, "|");
		}
	}

	for (uint32_t i = 0; i < m_nCountItemTypesKeys; i++) {
		free(m_apcItemTypes[i]);
		m_apcItemTypes[i] = (char*)malloc(MAX_STRING_LENGHT);
		memset(m_apcItemTypes[i], 0, MAX_STRING_LENGHT);

		sprintf_s(m_acBuffer, sizeof(m_acBuffer), "ItemTypeList%d", i + 1);
		dwLenght = GetPrivateProfileString("AutoItemPickup", m_acBuffer, 0, m_apcItemTypes[i], MAX_STRING_LENGHT - 1, szPathToIni);
		if (dwLenght != 0) {
			lstrcat(m_pcItemTypesAll, m_apcItemTypes[i]);
			lstrcat(m_pcItemTypesAll, "|");
		}
	}

	/// Parse ItemCode
	dwLenght = lstrlen(m_pcItemListAll);
	memcpy(m_pcItemListAllTemp, m_pcItemListAll, dwLenght + 1);
	// Count the total number of all items
	char* token_string_itemcode = strtok(m_pcItemListAllTemp, " ,|");
	while (token_string_itemcode)
	{
		m_nCountItemListAll++;
		token_string_itemcode = strtok(NULL, " ,|");
	}

	// Create an array of structures equal to the number of items
	m_stItemList = (item_code*)malloc(m_nCountItemListAll * sizeof(item_code));
	memset(m_stItemList, 0, m_nCountItemListAll * sizeof(item_code));

	// Parse each item
	memcpy(m_pcItemListAllTemp, m_pcItemListAll, dwLenght + 1);
	token_string_itemcode = strtok(m_pcItemListAllTemp, " ,|");

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

	/// Parse ItemType Code
	dwLenght = lstrlen(m_pcItemTypesAll);
	memcpy(m_pcItemTypesAllTemp, m_pcItemTypesAll, dwLenght + 1);
	char* token_string_itemtype_code = strtok(m_pcItemTypesAllTemp, ",|");
	while (token_string_itemtype_code)
	{
		m_nCountItemTypesAll++;
		token_string_itemtype_code = strtok(NULL, ",|");
	}

	m_stItemTypes = (item_type*)malloc(m_nCountItemTypesAll * sizeof(item_type));
	memset(m_stItemTypes, 0, m_nCountItemTypesAll * sizeof(item_type));

	memcpy(m_pcItemTypesAllTemp, m_pcItemTypesAll, dwLenght + 1);
	token_string_itemtype_code = strtok(m_pcItemTypesAllTemp, ",|");
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
}

class auto_item_pickup_menu : public d2_tweaks::ui::menu {
	d2_tweaks::common::asset* m_buttons_img;
	d2_tweaks::ui::controls::button* m_auto_pickup_btn;

public:
	auto_item_pickup_menu() {
		menu::set_enabled(true);
		menu::set_visible(true);

		//load_xml("d2tweaks\\interfaces\\autopickup.xml");
		if (DLLBASE_D2EXPRES != 0)
			load_xml("d2tweaks\\interface_d2expres\\autopickup.xml");
		if (DLLBASE_SGD2FREERES != 0)
			load_xml("d2tweaks\\interface_sgd2freeres\\autopickup.xml");
		if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
			load_xml("d2tweaks\\interface_vanilla\\autopickup.xml");

		m_buttons_img = singleton<d2_tweaks::common::asset_manager>::instance().get_mpq_file("d2tweaks\\assets\\buttons", d2_tweaks::common::MPQ_FILE_TYPE_DC6);
		m_auto_pickup_btn = get_button("m_auto_pickup_btn", std::bind(&auto_item_pickup_menu::auto_item_pickup_click, this));
	}

	void draw() override {
		if (m_bToggleAutoItemPickup) {
			//diablo2::d2_client::print_chat(L"AUTO TRANSMUTE ON", 1);
			m_auto_pickup_btn->set_current_frame(8); // index of frame in buttons.dc6
		}
		else if (!m_bToggleAutoItemPickup)
		{
			//diablo2::d2_client::print_chat(L"AUTO TRANSMUTE OFF", 2);
			m_auto_pickup_btn->set_current_frame(6);
		}

		if (m_bToggleAutoItemPickup && m_bInventoryFull) {
			m_auto_pickup_btn->set_current_frame(7);
		}

		//if (diablo2::d2_gfx::get_resolution_mode() == 0) {
		//	m_auto_pickup_btn->set_x(g_btn_hires_posx);
		//	m_auto_pickup_btn->set_y(g_btn_hires_posy);
		//}
		//else {
		//	m_auto_pickup_btn->set_x(g_btn_lowres_posx);
		//	m_auto_pickup_btn->set_y(g_btn_lowres_posy);
		//}

		if (!should_draw()) {
			m_auto_pickup_btn->set_enabled(false);
			m_auto_pickup_btn->set_visible(false);
			return;
		}

		m_auto_pickup_btn->set_enabled(true);
		m_auto_pickup_btn->set_visible(true);

		menu::draw();
	}
private:
	static bool should_draw() {
		return diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INTERFACE);
	}

	d2_tweaks::ui::controls::button* get_button(const std::string& name, const std::function<void()>& onClick) {
		auto result = static_cast<d2_tweaks::ui::controls::button*>(get_control(name));

		result->set_on_click(onClick);

		result->set_enabled(false);
		result->set_visible(false);

		return result;
	}

	void auto_item_pickup_click() {
		m_bToggleAutoItemPickup ^= true;

		if (m_bToggleAutoItemPickup) {
			ReloadFilters(m_acPathToIni);
			diablo2::d2_client::print_chat(L"AUTO PICKUP ON", 1);
		}
		else
		{
			diablo2::d2_client::print_chat(L"AUTO PICKUP OFF", 2);
		}
	}
};

void d2_tweaks::client::modules::auto_item_pickup::init() {
	GetCurrentDirectory(MAX_PATH, m_acPathToIni);
	lstrcat(m_acPathToIni, m_pcIniFile);

	if (GetPrivateProfileInt("modules", "AutoItemPickup", 1, m_acPathToIni) != FALSE) {
		m_iDistance = GetPrivateProfileInt("AutoItemPickup", "PickupDistance", 4, m_acPathToIni);

		//ReloadFilters(m_acPathToIni);

		singleton<ui::ui_manager>::instance().add_menu(new auto_item_pickup_menu());
		singleton<client>::instance().register_tick_handler(this);
		singleton<client>::instance().register_packet_handler(common::message_types_t::MESSAGE_TYPE_ITEM_PICKUP_INFO, this);
	}
}

bool find_free_space(diablo2::structures::inventory* inv, diablo2::structures::unit* item, int32_t inventoryIndex, char page, uint32_t& x, uint32_t& y) {
	char data[0x18];

	diablo2::d2_common::get_inventory_data(inventoryIndex, 0, data);

	const auto mx = static_cast<uint32_t>(data[0]);
	const auto my = static_cast<uint32_t>(data[1]);

	for (x = 0; x < mx; x++) {
		for (y = 0; y < my; y++) {
			diablo2::structures::unit* blockingUnit = nullptr;
			uint32_t blockingUnitIndex = 0;

			if (diablo2::d2_common::can_put_into_slot(inv, item, x, y, inventoryIndex, &blockingUnit, &blockingUnitIndex, page))
				return true;
		}
	}
	return false;
}

void d2_tweaks::client::modules::auto_item_pickup::tick() {
	static common::item_pickup_info_sc packet;
	const auto unit = diablo2::d2_client::get_local_player();

	//spdlog::debug("g_tick_between_item_pickup {0}", g_tick_between_item_pickup);
	m_nTick++;

	if (!m_bToggleAutoItemPickup)
		return;

	if (!unit)
		return;

	if (unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER)
		return;

	const auto room = diablo2::d2_common::get_room_from_unit(unit);

	if (!room)
		return;

	for (auto item = room->unit; item; item = item->prev_unit_in_room) {
		if (!item)
			continue;

		if (item->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM)
			continue;

		const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

		if (!record)
			continue;

		const auto distance = diablo2::d2_common::get_distance_between_units(unit, item);
		if (distance > m_iDistance)
			continue;

		//// Only one item can be picked up per frame; picking more will cause items to disappear.
		//if (record->string_code[0] == 'g' &&
		//	record->string_code[1] == 'l' &&
		//	record->string_code[2] == 'd')
		//{
		//	static d2_tweaks::common::gold_pickup_info_cs request_packet_cs;
		//	request_packet_cs.item_guid = item->guid;
		//	diablo2::d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
		//	continue;
		//}

		const auto itemtype_record = diablo2::d2_common::get_item_type_record(record->type);
		auto itemtype_record_equiv1 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv1);
		auto itemtype_record_equiv2 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv2);

		uint32_t quality = diablo2::d2_common::get_item_quality(item);

		// Clear the array
		char arr_itemtype_codestr_equivstr[20][5] = { 0 };

		// Item type code is always the first element in the array
		*(DWORD*)arr_itemtype_codestr_equivstr[0] = *(DWORD*)itemtype_record->code;
		// index second code in array
		uint32_t index_arr_itemtype = 1;

		if (itemtype_record_equiv1) {
			if (*(DWORD*)itemtype_record_equiv1->code != 0x20202020) {
				// Save the first previously obtained equiv1
				*(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv1->code;
				index_arr_itemtype++;
				// Unpack all equiv1 into an array
				for (index_arr_itemtype; itemtype_record_equiv1->equiv1 != 0; index_arr_itemtype++) {
					// Get the next one
					itemtype_record_equiv1 = diablo2::d2_common::get_item_type_record(itemtype_record_equiv1->equiv1);
					if (*(DWORD*)itemtype_record_equiv1->code != 0x20202020) {
						*(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv1->code;
					}
					else break;
				}
			}
		}

		if (itemtype_record_equiv2) {
			if (*(DWORD*)itemtype_record_equiv2->code != 0x20202020) {
				// Save the first previously obtained equiv1
				*(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv2->code;
				index_arr_itemtype++;
				// Expand all equiv1 into an array
				for (index_arr_itemtype; itemtype_record_equiv2->equiv2 != 0; index_arr_itemtype++) {
					// Get the next one
					itemtype_record_equiv2 = diablo2::d2_common::get_item_type_record(itemtype_record_equiv2->equiv2);
					if (*(DWORD*)itemtype_record_equiv2->code != 0x20202020) {
						*(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv2->code;
					}
					else break;
				}
			}
		}

		static d2_tweaks::common::item_pickup_info_cs request_packet_cs;
		request_packet_cs.item_guid = 0;

		for (uint32_t i = 0; i < m_nCountItemTypesAll; i++)
		{
			for (uint32_t count = 0; count < index_arr_itemtype; count++)
			{
				if (*(DWORD*)arr_itemtype_codestr_equivstr[count] == (DWORD)m_stItemTypes[i].dwtype)
				{
					if (m_stItemTypes[i].qualityinclude[quality] == TRUE)
					{
						const auto inventoryIndex = diablo2::d2_common::get_inventory_index(unit, 0, 0x65);
						uint32_t tx = 0, ty = 0;
						if (!find_free_space(unit->inventory, item, inventoryIndex, 0, tx, ty)) {
							m_bInventoryFull = true;
							if (m_nTick >= 500) {
								diablo2::d2_client::play_sound(0xB76, unit, 0, 0, 0);
								m_nTick = 0;
							}
							goto L1;
						}
						else {
							m_bInventoryFull = false;
						}

						request_packet_cs.item_guid = item->guid;
						diablo2::d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
						goto L1;
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
					const auto inventoryIndex = diablo2::d2_common::get_inventory_index(unit, 0, 0x65);
					uint32_t tx = 0, ty = 0;
					if (!find_free_space(unit->inventory, item, inventoryIndex, 0, tx, ty)) {
						m_bInventoryFull = true;
						if (m_nTick >= 500) {
							diablo2::d2_client::play_sound(0xB76, unit, 0, 0, 0);
							m_nTick = 0;
						}
						goto L1;
					}
					else {
						m_bInventoryFull = false;
					}

					request_packet_cs.item_guid = item->guid;
					diablo2::d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
					goto L1;
				}
			}
		}
	L1:;
	}
}

void d2_tweaks::client::modules::auto_item_pickup::handle_packet(common::packet_header* packet) {
	const auto info = static_cast<common::item_pickup_info_sc*>(packet);

	//if (info->inventory_full == true) {
	//	m_nTick = 0;
	//}
	//else {
	//	g_tick_between_item_pickup = g_delay_between_item_pickup;
	//}
	//g_value += info->gold;
}
