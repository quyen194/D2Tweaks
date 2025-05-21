#include <d2tweaks/server/modules/auto_gold_pickup/auto_gold_pickup_server.h>
#include <d2tweaks/server/server.h>

#include <spdlog/spdlog.h>
#include <diablo2/d2game.h>
#include <diablo2/d2common.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/room.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/player_data.h>
#include <diablo2/structures/unit.h>

MODULE_INIT(auto_gold_pickup)

void d2_tweaks::server::modules::auto_gold_pickup::init() {
	char acPathToIni[MAX_PATH] = { 0 };
	const char* pcIniFile = "\\d2tweaks.ini";

	GetCurrentDirectory(MAX_PATH, acPathToIni);
	lstrcat(acPathToIni, pcIniFile);

	if (GetPrivateProfileInt("modules", "AutoGoldPickup", 1, acPathToIni) != FALSE) {
		singleton<server>::instance().register_packet_handler(common::MESSAGE_TYPE_GOLD_PICKUP_INFO, this);
		//singleton<server>::instance().register_tick_handler(this);
	}
}

bool d2_tweaks::server::modules::auto_gold_pickup::handle_packet(diablo2::structures::game* game, diablo2::structures::unit* player, common::packet_header* packet) {
	const auto income_packet_cs = static_cast<common::gold_pickup_info_cs*>(packet);
	static auto& instance = singleton<server>::instance();
	const auto item = instance.get_server_unit(game, income_packet_cs->item_guid, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM); //0x4 = item

	if (item == nullptr)
		return true; //block further packet processing

	d2_tweaks::server::modules::auto_gold_pickup::au_pickup_gold(game, player, item);

	return true;
}

bool d2_tweaks::server::modules::auto_gold_pickup::au_pickup_gold(diablo2::structures::game* game, diablo2::structures::unit* unit, diablo2::structures::unit* item)
{
	static common::gold_pickup_info_sc packet;

	const auto currentGold = diablo2::d2_common::get_stat(unit, diablo2::UNIT_STAT_GOLD, 0);
	const auto goldToPickup = diablo2::d2_common::get_stat(item, diablo2::UNIT_STAT_GOLD, 0);
	const auto maxGold = diablo2::d2_common::get_maximum_character_gold(unit);

	if (static_cast<unsigned int>(currentGold + goldToPickup) > maxGold)
		return false;

	diablo2::d2_game::pickup_gold_pile(game, unit, item);
	packet.gold = goldToPickup;

	singleton<server>::instance().send_packet(unit->player_data->net_client, &packet, sizeof packet);
	return true;
}

void d2_tweaks::server::modules::auto_gold_pickup::tick(diablo2::structures::game* game,
	diablo2::structures::unit* unit) {
	//static common::gold_pickup_info_sc packet;
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

	//	auto record = diablo2::d2_common::get_item_record(item->data_record_index);

	//	if (!record)
	//		continue;

	//	const auto distance = diablo2::d2_common::get_distance_between_units(unit, item);
	//	if (distance > g_distance)
	//		continue;

	//	// cannot pick up more than one item per tick, otherwise the item will disappear
	//	if (record->string_code[0] == 'g' &&
	//		record->string_code[1] == 'l' &&
	//		record->string_code[2] == 'd')
	//	{
	//		d2_tweaks::server::modules::auto_gold_pickup::au_pickup_gold(game, unit, item);
	//		break;
	//	}

	//	continue;
	//}
}