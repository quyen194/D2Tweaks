#include <d2tweaks/server/modules/identify_on_pickup/identify_on_pickup_server.h>
#include <d2tweaks/server/server.h>

#include <common/hooking.h>

#include <diablo2/d2game.h>
#include <diablo2/d2common.h>
#include <diablo2/structures/unit.h>

MODULE_INIT(identify_on_pickup)

static unsigned int g_item_Normal = 0;
static unsigned int g_item_Superior = 0;
static unsigned int g_item_Magic = 0;
static unsigned int g_item_Rare = 0;
static unsigned int g_item_Set = 0;
static unsigned int g_item_Unique = 0;
static unsigned int g_item_Crafted = 0;
static unsigned int g_item_Tempered = 0;

static uint32_t(__fastcall* g_pickup_item_original)(diablo2::structures::game*,
	diablo2::structures::unit*,
	uint32_t, uint32_t);
static uint32_t __fastcall pickup_item(diablo2::structures::game* game,
	diablo2::structures::unit* player,
	uint32_t guid,
	uint32_t a4) {
	static auto& instance = singleton<d2_tweaks::server::server>::instance();

	if (!game || !player || player->type != diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER)
		return g_pickup_item_original(game, player, guid, a4);

	const auto item = instance.get_server_unit(game, guid, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM);

	if (!item)
		return g_pickup_item_original(game, player, guid, a4);

	auto quality = diablo2::d2_common::get_item_quality(item);

	if (quality == 2 && g_item_Normal != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 3 && g_item_Superior != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 4 && g_item_Magic != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 5 && g_item_Set != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 6 && g_item_Rare != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 7 && g_item_Unique != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 8 && g_item_Crafted != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 9 && g_item_Tempered != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}

	return g_pickup_item_original(game, player, guid, a4);
}

static uint32_t(__fastcall* g_pickup_item_cursor_original)(diablo2::structures::game*,
	diablo2::structures::unit*,
	uint32_t, uint32_t);
static uint32_t __fastcall pickup_item_cursor(diablo2::structures::game* game,
	diablo2::structures::unit* player,
	uint32_t guid,
	uint32_t a4) {
	static auto& instance = singleton<d2_tweaks::server::server>::instance();

	if (!game || !player || player->type != diablo2::structures::unit_type_t::UNIT_TYPE_PLAYER)
		return g_pickup_item_cursor_original(game, player, guid, a4);

	const auto item = instance.get_server_unit(game, guid, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM);

	if (!item)
		return g_pickup_item_cursor_original(game, player, guid, a4);

	auto quality = diablo2::d2_common::get_item_quality(item);

	if (quality == 2 && g_item_Normal != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 3 && g_item_Superior != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 4 && g_item_Magic != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 5 && g_item_Set != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 6 && g_item_Rare != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 7 && g_item_Unique != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 8 && g_item_Crafted != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}
	if (quality == 9 && g_item_Tempered != 0) {
		diablo2::d2_game::identify_item(game, player, item);
	}

	return g_pickup_item_cursor_original(game, player, guid, a4);
}

void d2_tweaks::server::modules::identify_on_pickup::init() {
	char acPathToIni[MAX_PATH] = { 0 };
	const char* pcIniFile = "\\d2tweaks.ini";

	GetCurrentDirectory(MAX_PATH, acPathToIni);
	lstrcat(acPathToIni, pcIniFile);

	if (GetPrivateProfileInt("modules", "IdentifyOnPickup", 1, acPathToIni) != FALSE) {
		hooking::hook(diablo2::d2_game::get_base() + 0x13340, pickup_item, &g_pickup_item_original);
		hooking::hook(diablo2::d2_game::get_base() + 0x12B80, pickup_item_cursor, &g_pickup_item_cursor_original);

		g_item_Normal = GetPrivateProfileInt("IdentifyOnPickup", "Normal", 1, acPathToIni);
		g_item_Superior = GetPrivateProfileInt("IdentifyOnPickup", "Superior", 1, acPathToIni);
		g_item_Magic = GetPrivateProfileInt("IdentifyOnPickup", "Magic", 1, acPathToIni);
		g_item_Rare = GetPrivateProfileInt("IdentifyOnPickup", "Rare", 1, acPathToIni);
		g_item_Set = GetPrivateProfileInt("IdentifyOnPickup", "Set", 1, acPathToIni);
		g_item_Unique = GetPrivateProfileInt("IdentifyOnPickup", "Unique", 1, acPathToIni);
		g_item_Crafted = GetPrivateProfileInt("IdentifyOnPickup", "Crafted", 1, acPathToIni);
		g_item_Tempered = GetPrivateProfileInt("IdentifyOnPickup", "Tempered", 1, acPathToIni);
	}
}