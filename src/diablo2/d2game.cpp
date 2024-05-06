#include <diablo2/d2game.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/npc_record.h>
#include <common/ptr_wrapper.h>

char* diablo2::d2_game::get_base() {
	static auto base = reinterpret_cast<char*>(GetModuleHandle("d2game.dll"));
	return base;
}

void diablo2::d2_game::enqueue_packet(structures::net_client* client, void* packet, size_t size) {
	static wrap_func_fast<void(structures::net_client*, void*, size_t)> enqueue_packet(0xC710, get_base());
	enqueue_packet(client, packet, size);
}

diablo2::structures::npc_record* diablo2::d2_game::get_npc_record(structures::game* game, structures::unit* npc, structures::unit** ptnpc) {
	static wrap_func_fast<structures::npc_record* (structures::game*, structures::unit*, structures::unit**)>get_npc_record(0x9B910, get_base());
	return get_npc_record(game, npc, ptnpc);
}

void diablo2::d2_game::free_gamble(structures::game* game, structures::unit* player, structures::unit* npc, structures::npc_record* npcrecord) {
	static wrap_func_fast<void(structures::game* game, structures::unit* player, structures::unit* npc, structures::npc_record* npcrecord)>free_gamble(0x9D190, get_base());
	free_gamble(game, player, npc, npcrecord);
}

void diablo2::d2_game::fill_gamble(structures::game* game, structures::unit* player, structures::unit* npc, structures::npc_record* npcrecord) {
	static wrap_func_fast<void(structures::game* game, structures::unit* player, structures::unit* npc, structures::npc_record* npcrecord)>fill_gamble(0x9A9F0, get_base());
	fill_gamble(game, player, npc, npcrecord);
}

void diablo2::d2_game::create_vendor_cache1(structures::game* game, structures::unit* player, structures::unit* npc, uint32_t param, bool bGamble) {
	static wrap_func_fast<void(structures::game* game, structures::unit* player, structures::unit* npc, uint32_t param, bool bGamble)>create_vendor_cache1(0x974F0, get_base());
	create_vendor_cache1(game, player, npc, param, bGamble);
}

void diablo2::d2_game::create_vendor_cache2(structures::game* game, structures::unit* player, structures::unit* npc, uint32_t param, bool bGamble) {
	static wrap_func_fast<void(structures::game* game, structures::unit* player, structures::unit* npc, uint32_t param, bool bGamble)>create_vendor_cache2(0x9AE20, get_base());
	create_vendor_cache2(game, player, npc, param, bGamble);
}

diablo2::structures::unit* diablo2::d2_game::get_server_unit(structures::game* game, structures::unit_type_t type, uint32_t uniqueid) {
	static wrap_func_fast<structures::unit* (structures::game*, structures::unit_type_t, uint32_t)>get_server_unit(0x8BB00, get_base());
	return get_server_unit(game, type, uniqueid);
}

diablo2::structures::game* diablo2::d2_game::get_game_from_client_id(int32_t id) {
	static wrap_func_fast<structures::game* (int32_t)> get_game_from_client_id(0x94E0, get_base());
	return get_game_from_client_id(id);
}

// __fastcall D2CLIENTS_GetClientByClientNumber(pGame, int nNumber)
diablo2::structures::net_client* diablo2::d2_game::get_net_client_from_id(structures::game* game, int32_t id) {
	static wrap_func_fast<structures::net_client* (structures::game*, int32_t)>get_net_client_from_id(0x1DE0, get_base());
	return get_net_client_from_id(game, id);
}

diablo2::structures::net_client* diablo2::d2_game::get_net_client_from_id_2(structures::game* game, int32_t id) {
	static wrap_func_fast<structures::net_client* (structures::game*, int32_t)>get_net_client_from_id_2(0x37B0, get_base());
	return get_net_client_from_id_2(game, id);
}

diablo2::structures::unit* diablo2::d2_game::get_player_pet(structures::game* game, structures::unit* unit,
	uint32_t type, uint32_t index) {
	static wrap_func_fast<structures::unit* (structures::game*, structures::unit*, uint32_t, uint32_t)>get_player_pet(0x4E8B0, get_base());
	return get_player_pet(game, unit, type, index);
}

int32_t diablo2::d2_game::identify_item(structures::game* game, structures::unit* player, structures::unit* item) {
	static wrap_func_fast<int32_t(structures::game*, structures::unit*, structures::unit*)> identify_item(0x19670, get_base());
	return identify_item(game, player, item);
}

int32_t diablo2::d2_game::pickup_gold_pile(structures::game* game, structures::unit* unit, structures::unit* item) {
	static wrap_func_fast<int32_t(structures::game*, structures::unit*, structures::unit*)> pickup_gold_pile(0x12DD0, get_base());
	return pickup_gold_pile(game, unit, item);
}

bool __fastcall diablo2::d2_game::pickup_item(diablo2::structures::game* game, diablo2::structures::unit* player, uint32_t guid, uint32_t* ptrNull) {
	static wrap_func_fast<bool(diablo2::structures::game*, diablo2::structures::unit*, uint32_t, uint32_t*)>pickup_item(0x13340, get_base());
	return pickup_item(game, player, guid, ptrNull);
}

void diablo2::d2_game::update_inventory_items(structures::game* game, structures::unit* player) {
	static wrap_func_fast<void(structures::game*, structures::unit*, uint32_t)>update_inventory_items(0x14A90, get_base());
	update_inventory_items(game, player, 0);
}

uint32_t* diablo2::d2_game::get_game_id_array_begin() {
	static wrap_value<uint32_t> game_id_array_begin(0x1147F8, get_base());
	return game_id_array_begin;
}

uint32_t* diablo2::d2_game::get_game_id_array_end() {
	static wrap_value<uint32_t> game_id_array_end(0x1157F8, get_base());
	return game_id_array_end;
}

diablo2::structures::game_server* diablo2::d2_game::get_game_server() {
	static wrap_value<structures::game_server*> game_server(0x115818, get_base());
	return *game_server;
}

static diablo2::structures::game* (__thiscall* g_get_game)(diablo2::structures::game_server* gs, uint32_t gameId, void*)
= decltype(g_get_game)(0xB6A0 + diablo2::d2_game::get_base());

diablo2::structures::game* diablo2::d2_game::get_game(structures::game_server* gs, uint32_t gameId) {
	static wrap_func_std<structures::game* (uint32_t, void*)> get_game(0xB6A0, get_base());
	return g_get_game(gs, gameId, reinterpret_cast<char*>(gs) + 0x44);
}

diablo2::structures::unit* diablo2::d2_game::get_unit_owner(structures::game* game, structures::unit* unit) {
	static wrap_func_fast<structures::unit* (structures::game*, structures::unit*)> get_unit_owner(0x8BB70, get_base());
	return get_unit_owner(game, unit);
}

static void __fastcall unit_pet_iterator(diablo2::structures::game* game, diablo2::structures::unit* owner,
	diablo2::structures::unit* unit, void* arg) {
	const std::function<void(diablo2::structures::game*, diablo2::structures::unit*, diablo2::structures::unit*)>* cb =
		reinterpret_cast<decltype(cb)>(arg);
	cb->operator()(game, owner, unit);
}

void* diablo2::d2_game::iterate_unit_pets(structures::game* game, structures::unit* unit,
	const std::function<void(structures::game*, structures::unit*, diablo2::structures::unit*)>& cb) {
	static wrap_func_fast<void* (structures::game*, structures::unit*,
		void(__fastcall*)(structures::game*, structures::unit*, diablo2::structures::unit*, void*), void*)> iterate_unit_pets(0x4E7C0, get_base());
	const auto cbref = &cb;
	// ReSharper disable once CppCStyleCast
	return iterate_unit_pets(game, unit, unit_pet_iterator, (void*)cbref);
}

uint32_t __fastcall diablo2::d2_game::transmogrify(diablo2::structures::game* game, diablo2::structures::unit* player) {
	static wrap_func_fast<uint32_t(diablo2::structures::game*, diablo2::structures::unit*)>transmogrify(0x62130, get_base());
	return transmogrify(game, player);
}

// d2game:$0x60010
// int __fastcall CRAFT_Transmogrify(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2CubemainTXT* pCubeTxt, void* pUnknown)

// Add a wrapper for the following function:
// structures::unit* __fastcall D2GAME_CreateItemEx_6FC4ED80(structures::game* pGame, structures::D2ItemDropStrc* pItemDrop, int32_t a3);
diablo2::structures::unit* __fastcall diablo2::d2_game::D2GAME_CreateItemEx_6FC4ED80(diablo2::structures::game* pGame, diablo2::structures::D2ItemDropStrc* pItemDrop, int32_t a3) {
	static wrap_func_fast< diablo2::structures::unit* (diablo2::structures::game*, diablo2::structures::D2ItemDropStrc*, int32_t)> D2GAME_CreateItemEx_6FC4ED80(0x1ed80, get_base());
	return D2GAME_CreateItemEx_6FC4ED80(pGame, pItemDrop, a3);
}

// Add a wrapper for the following function:
//D2Game.0x6FC4A660
//int32_t __fastcall D2GAME_Transmogrify_6FC4A660(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
int32_t __fastcall diablo2::d2_game::D2GAME_Transmogrify_6FC4A660(diablo2::structures::game* pGame, diablo2::structures::unit* pPlayer, diablo2::structures::unit* pItem) {
	static wrap_func_fast<int32_t(diablo2::structures::game*, diablo2::structures::unit*, diablo2::structures::unit*)> D2GAME_Transmogrify_6FC4A660(0x1a660, get_base());
	return D2GAME_Transmogrify_6FC4A660(pGame, pPlayer, pItem);
}

//D2Game.0x6FC95DF0
//D2UnitStrc* __fastcall QUESTS_CreateItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint32_t dwCode, int32_t nLevel, uint8_t nQuality, int32_t bDroppable)
diablo2::structures::unit* __fastcall diablo2::d2_game::QUESTS_CreateItem(diablo2::structures::game* pGame, diablo2::structures::unit* pPlayer, uint32_t dwCode, int32_t nLevel, uint8_t nQuality, int32_t bDroppable) {
	static wrap_func_fast< diablo2::structures::unit* (diablo2::structures::game*, diablo2::structures::unit*, uint32_t, int32_t, uint8_t, int32_t)>QUESTS_CreateItem(0x65DF0, get_base());
	return QUESTS_CreateItem(pGame, pPlayer, dwCode, nLevel, nQuality, bDroppable);
}


