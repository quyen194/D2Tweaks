#include <d2tweaks/server/modules/auto_item_pickup/auto_item_pickup_server.h>
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

MODULE_INIT(auto_item_pickup)

void d2_tweaks::server::modules::auto_item_pickup::init() {
	char acPathToIni[MAX_PATH] = { 0 };
	const char* pcIniFile = "\\d2tweaks.ini";

	GetCurrentDirectory(MAX_PATH, acPathToIni);
	lstrcat(acPathToIni, pcIniFile);

	if (GetPrivateProfileInt("modules", "AutoItemPickup", 1, acPathToIni) != FALSE) {
		singleton<server>::instance().register_packet_handler(common::MESSAGE_TYPE_ITEM_PICKUP_INFO, this);
		singleton<server>::instance().register_tick_handler(this);
	}
}

void d2_tweaks::server::modules::auto_item_pickup::tick(diablo2::structures::game* game,
	diablo2::structures::unit* unit) {
	//static common::item_pickup_info_sc packet;
	//static auto& instance = singleton<d2_tweaks::server::server>::instance();
	//if (!game || !unit)
	//	return;

	//if (unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER)
	//	return;

	//const auto room = diablo2::d2_common::get_room_from_unit(unit);

	//if (!room)
	//	return;

	//for (auto item = room->unit; item; item = item->prev_unit_in_room) {
	//	if (!item)
	//		continue;

	//	if (item->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM)
	//		continue;

	//	const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

	//	if (!record)
	//		continue;

	//	const auto distance = diablo2::d2_common::get_distance_between_units(unit, item);
	//	if (distance > g_distance)
	//		continue;

	//	const auto itemtype_record = diablo2::d2_common::get_item_type_record(record->type);
	//	auto itemtype_record_equiv1 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv1);
	//	auto itemtype_record_equiv2 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv2);

	//	uint32_t quality = diablo2::d2_common::get_item_quality(item);

	//	// clear the array
	//	char arr_itemtype_codestr_equivstr[20][5] = { 0 };

	//	// itemtype code is always the first element in the array
	//	*(DWORD*)arr_itemtype_codestr_equivstr[0] = *(DWORD*)itemtype_record->code;
	//	// index second code in array
	//	uint32_t index_arr_itemtype = 1;

	//	if (itemtype_record_equiv1) {
	//		if (*(DWORD*)itemtype_record_equiv1->code != 0x20202020) {
	//			// save the first previously obtained equiv1
	//			*(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv1->code;
	//			index_arr_itemtype++;
	//			// expand all equiv1 into the array
	//			for (index_arr_itemtype; itemtype_record_equiv1->equiv1 != 0; index_arr_itemtype++) {
	//				// get the next one
	//				itemtype_record_equiv1 = diablo2::d2_common::get_item_type_record(itemtype_record_equiv1->equiv1);
	//				if (*(DWORD*)itemtype_record_equiv1->code != 0x20202020) {
	//					*(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv1->code;
	//				}
	//				else break;
	//			}
	//		}
	//	}

	//	if (itemtype_record_equiv2) {
	//		if (*(DWORD*)itemtype_record_equiv2->code != 0x20202020) {
	//			// save the first previously obtained equiv2
	//			*(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv2->code;
	//			index_arr_itemtype++;
	//			// expand all equiv2 into the array
	//			for (index_arr_itemtype; itemtype_record_equiv2->equiv2 != 0; index_arr_itemtype++) {
	//				// get the next one
	//				itemtype_record_equiv2 = diablo2::d2_common::get_item_type_record(itemtype_record_equiv2->equiv2);
	//				if (*(DWORD*)itemtype_record_equiv2->code != 0x20202020) {
	//					*(DWORD*)arr_itemtype_codestr_equivstr[index_arr_itemtype] = *(DWORD*)itemtype_record_equiv2->code;
	//				}
	//				else break;
	//			}
	//		}
	//	}

	//	for (uint32_t i = 0; i < g_pickup_count_itemtype_code; i++)
	//	{
	//		for (uint32_t count = 0; count < index_arr_itemtype; count++)
	//		{
	//			if (*(DWORD*)arr_itemtype_codestr_equivstr[count] == (DWORD)g_pickup_itemtype_code_st[i].dwtype)
	//			{
	//				if (g_pickup_itemtype_code_st[i].qualityinclude[quality] == TRUE)
	//				{
	//					d2_tweaks::server::modules::auto_item_pickup::au_pickup_item(game, unit, item);
	//					goto L1;
	//				}
	//			}
	//		}
	//	}

	//	for (uint32_t i = 0; i < g_pickup_count_all_items; i++)
	//	{
	//		if (record->string_code[0] == g_pickup_itemcode_st[i].code0 &&
	//			record->string_code[1] == g_pickup_itemcode_st[i].code1 &&
	//			record->string_code[2] == g_pickup_itemcode_st[i].code2)
	//		{
	//			if (g_pickup_itemcode_st[i].qualityinclude[quality] == TRUE)
	//			{
	//				d2_tweaks::server::modules::auto_item_pickup::au_pickup_item(game, unit, item);
	//			}
	//		}
	//	}

	//	L1:
	//	continue;
	//}

	//g_tick_between_item_pickup++;
	////spdlog::info("current {0}", g_tick_between_item_pickup);
}

bool d2_tweaks::server::modules::auto_item_pickup::au_pickup_item(diablo2::structures::game* game, diablo2::structures::unit* unit, uint32_t guid)
{
	static common::item_pickup_info_sc packet;

	uint32_t ptrNull = 0;
	//if (g_tick_between_item_pickup >= 25) {
		// true - if item picked up, false - if inventory full
	if (diablo2::d2_game::pickup_item(game, unit, guid, &ptrNull) != true)
	{
		//packet.inventory_full = true;
		//singleton<server>::instance().send_packet(unit->player_data->net_client, &packet, sizeof packet);
	}
	//}

	return true;
}

bool d2_tweaks::server::modules::auto_item_pickup::handle_packet(diablo2::structures::game* game, diablo2::structures::unit* player, common::packet_header* packet) {
	const auto income_packet_cs = static_cast<common::item_pickup_info_cs*>(packet);
	static auto& instance = singleton<server>::instance();

	//const auto item = instance.get_server_unit(game, income_packet_cs->item_guid, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM); //0x4 = item
	//if (item == nullptr)
	//	return true; //block further packet processing

	d2_tweaks::server::modules::auto_item_pickup::au_pickup_item(game, player, income_packet_cs->item_guid);

	return true;
}