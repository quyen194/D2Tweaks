#pragma once

#include <cstdint>
#include <functional>
#include <diablo2/structures/unit.h>

namespace diablo2 {
	namespace structures {
		struct game_server;
		struct net_client;
		struct game;
		struct unit;
	}

	class d2_game {
	public:
		static char* get_base();

		static void enqueue_packet(structures::net_client* client, void* packet, size_t size);

		static uint32_t* get_game_id_array_begin();
		static uint32_t* get_game_id_array_end();

		static structures::game_server* get_game_server();
		static structures::game* get_game(structures::game_server* gs, uint32_t gameId);

		static structures::game* get_game_from_client_id(int32_t id);
		static structures::net_client* get_net_client_from_id(structures::game* game, int32_t id);
		static structures::net_client* diablo2::d2_game::get_net_client_from_id_2(structures::game* game, int32_t id);

		static structures::unit* get_player_pet(structures::game* game, structures::unit* unit, uint32_t type, uint32_t index);

		static structures::unit* get_server_unit(structures::game* game, diablo2::structures::unit_type_t type, uint32_t uniqueid);
		static structures::npc_record* diablo2::d2_game::get_npc_record(structures::game* game, structures::unit* npc, structures::unit** ptnpc);
		static void diablo2::d2_game::free_gamble(structures::game* game, structures::unit* player, structures::unit* npc, structures::npc_record* npcrecord);
		static void diablo2::d2_game::fill_gamble(structures::game* game, structures::unit* player, structures::unit* npc, structures::npc_record* npcrecord);
		static void diablo2::d2_game::create_vendor_cache1(structures::game* game, structures::unit* player, structures::unit* npc, uint32_t param, bool bGamble);
		static void diablo2::d2_game::create_vendor_cache2(structures::game* game, structures::unit* player, structures::unit* npc, uint32_t param, bool bGamble);

		static int32_t identify_item(structures::game* game, structures::unit* player, structures::unit* item);
		static int32_t pickup_gold_pile(structures::game* game, structures::unit* unit, structures::unit* item);
		//static int32_t pickup_item(structures::game* game, structures::unit* unit, structures::unit* item);
		static bool __fastcall pickup_item(structures::game* game, structures::unit* player, uint32_t guid, uint32_t* ptrItemCarried);
		static structures::unit* get_unit_owner(structures::game* game, structures::unit* unit);
		static void* iterate_unit_pets(structures::game* game, structures::unit* unit,
			const std::function<void(structures::game*, structures::unit*, structures::unit*)>& cb);

		static void update_inventory_items(structures::game* game, structures::unit* player);
		static uint32_t __fastcall diablo2::d2_game::transmogrify(diablo2::structures::game* game, diablo2::structures::unit* player);

		//D2Game.0x6FC4ED80
		structures::unit* __fastcall D2GAME_CreateItemEx_6FC4ED80(structures::game* pGame, structures::D2ItemDropStrc* pItemDrop, int32_t a3);
	};
}
