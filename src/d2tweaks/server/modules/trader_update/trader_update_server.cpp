#include <d2tweaks/server/modules/trader_update/trader_update_server.h>
#include <d2tweaks/server/server.h>
#include <d2tweaks/common/protocol.h>
#include <spdlog/spdlog.h>
#include <diablo2/d2game.h>
#include <diablo2/d2common.h>
#include <diablo2/d2lang.h>
#include <diablo2/d2net.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/room.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>
#include <diablo2/structures/player_data.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/net_client.h>
#include <diablo2/structures/path.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/player_data.h>
#include <diablo2/structures/npc_record.h>
#include <common/autopickup_lootfilter.h>
#include <common/ini.h>
#include <common/hooking.h>
#include <DllNotify.h>
#include <D2Template.h>

MODULE_INIT(trader_update)

enum trader_command {
	COMMAND_NULL,
	COMMAND_FREE_NPC_INVENTORY,
	COMMAND_FREE_NPC_GAMBLE,
	COMMAND_FILL_NPC_INVENTORY,
	COMMAND_FILL_NPC_GAMBLE,
	COMMAND_FINISHED,
	COMMAND_ABORT
};

static uint32_t m_nNpcId;
static bool m_bGamble;
static uint32_t m_nParam4 = 1;

//void* g_click_trade_menu;
//void* g_click_gamble_menu;
//void* g_ret;
//uint32_t gamble_or_trade; //gamble == 2, trade == 1

//__declspec (naked) void click_menu() {
//__asm {
//		pushad;
//		pushfd;
//		mov eax, [esp + 0x2C];//+0x8
//		mov [m_nNpcId], eax;
//		mov eax, DLLBASE_D2GAME;
//		add eax, 0x99F4A;
//		mov [g_ret], eax;
//		popfd;
//		popad;
//
//		mov eax, dword ptr ss : [esp + 0x8]
//		sub esp, 0x10C
//		jmp[g_ret];
//	}
//}
//
//static const DLLPatchStrc gpt_click_menu[] =
//{
//	// d2game:$0x99F40
//	{D2DLL_D2GAME, 0x99F40 + 0, PATCH_JMP, FALSE, 0x1},
//	{D2DLL_D2GAME, 0x99F40 + 1, (DWORD)click_menu, TRUE, 0x0},
//	{D2DLL_D2GAME, 0x99F40 + 5, (DWORD)PATCH_NOPBLOCK, FALSE, 0x5},
//	{D2DLL_INVALID}
//};
//
//
//__declspec (naked) void click_trade_menu() {
//	__asm {
//		pushad; //+0x24 esp
//		pushfd; //
//		//mov eax, [esp + 0x28];//+0x4
//		//mov [m_nNpcId], eax;
//		mov eax, [esp + 0x34];//+0x10
//		mov [m_nParam4], eax;
//		mov [m_bGamble], FALSE
//		mov eax, DLLBASE_D2GAME;
//		add eax, 0x9A086;
//		mov [g_ret], eax;
//		popfd;
//		popad;
//		// original instructions
//		mov edx, dword ptr [esp + 0x10];
//		mov ecx, edi;
//		jmp [g_ret];
//	}
//}
//
//__declspec (naked) void click_gamble_menu() {
//	__asm {
//		pushad; //+0x24 esp
//		pushfd; //
//		//mov eax, [esp + 0x28];//+0x4
//		//mov [m_nNpcId], eax;
//		mov eax, [esp + 0x34];//+0x10
//		mov [m_nParam4], eax;
//		mov [m_bGamble], TRUE
//		mov eax, DLLBASE_D2GAME;
//		add eax, 0x9A0F0;
//		mov [g_ret], eax;
//		popfd;
//		popad;
//		mov eax, dword ptr [esp + 0x10] ;
//		mov edx, esi;
//		jmp[g_ret];
//	}
//}
//
//
//static const DLLPatchStrc gpt_click_trade_menu[] =
//{
//	{D2DLL_D2GAME, 0x9A080 + 0, PATCH_JMP, FALSE, 0x1},
//	{D2DLL_D2GAME, 0x9A080 + 1, (DWORD)click_trade_menu, TRUE, 0x0},
//	{D2DLL_D2GAME, 0x9A080 + 5, (DWORD)PATCH_NOPBLOCK, FALSE, 0x1},
//	{D2DLL_INVALID}
//};
//
//static const DLLPatchStrc gpt_click_gamble_menu[] =
//{
//	{D2DLL_D2GAME, 0x9A0EA + 0, PATCH_JMP, FALSE, 0x1},
//	{D2DLL_D2GAME, 0x9A0EA + 1, (DWORD)click_gamble_menu, TRUE, 0x0},
//	{D2DLL_D2GAME, 0x9A0EA + 5, (DWORD)PATCH_NOPBLOCK, FALSE, 0x1},
//	{D2DLL_INVALID}
//};

void d2_tweaks::server::modules::trader_update::init() {
	char szDir[MAX_PATH];
	char szPath[MAX_PATH];
	const char szConfig[] = "d2tweaks.ini";

	GetCurrentDirectory(MAX_PATH, szDir);
	snprintf(szPath, MAX_PATH, "%s\\%s", szDir, szConfig);
	CIni config(szPath);

	if (config.GetInt("modules", "ReloadTradeGamble", 1) != FALSE) {
		singleton<server>::instance().register_packet_handler(common::MESSAGE_TYPE_TRADER_UPDATE, this);
		//D2TEMPLATE_ApplyPatch(gpt_click_trade_menu);
		//D2TEMPLATE_ApplyPatch(gpt_click_gamble_menu);
		//D2TEMPLATE_ApplyPatch(gpt_click_menu);
	}
}

void d2_tweaks::server::modules::trader_update::tick(diablo2::structures::game* game, diablo2::structures::unit* unit) {
	return;
}

struct ClientFromNumber {
	uint8_t padding[0x174];
	diablo2::structures::net_client* net_cleint;
};

bool d2_tweaks::server::modules::trader_update::handle_packet(diablo2::structures::game* game, diablo2::structures::unit* player, common::packet_header* packet) {
	const auto income_packet_cs = static_cast<common::trader_update_cs*>(packet);
	static common::trader_update_sc response_packet_sc;

	diablo2::structures::unit* temp_ptNPC = diablo2::d2_game::get_server_unit(game, diablo2::structures::unit_type_t::UNIT_TYPE_MONSTER, income_packet_cs->npc_id);
	diablo2::structures::npc_record* npcrecord = diablo2::d2_game::get_npc_record(game, temp_ptNPC, &temp_ptNPC);
	diablo2::structures::unit* ptNPC = diablo2::d2_game::get_server_unit(game, diablo2::structures::unit_type_t::UNIT_TYPE_MONSTER, income_packet_cs->npc_id);

	if (!ptNPC)
		return true;

	// button press, clear the merchant's inventory on the server
	if (income_packet_cs->command == COMMAND_FREE_NPC_INVENTORY) {
		// send update packet to all connected clients
		// net client id - 1 is always the host, then every odd number: 3, 5, 7, 9, 11, 13, 15
		for (uint32_t i = 0; i < 16; i++) {
			diablo2::structures::net_client* netclient = diablo2::d2_game::get_net_client_from_id(game, i);
			if (netclient != 0) {
				response_packet_sc.command = COMMAND_FREE_NPC_INVENTORY;
				response_packet_sc.npc_id = income_packet_cs->npc_id;
				response_packet_sc.is_gamble_menu_open = income_packet_cs->is_gamble_menu_open;
				singleton<server>::instance().send_packet(netclient, &response_packet_sc, sizeof response_packet_sc);
			}
		}
	}

	if (income_packet_cs->command == COMMAND_FREE_NPC_GAMBLE) {
		npcrecord->pGamble->pInventory = 0;
		npcrecord->pGamble->dwGUID = 0;
		npcrecord->pGamble = 0;
		response_packet_sc.command = COMMAND_FREE_NPC_GAMBLE;
		response_packet_sc.npc_id = income_packet_cs->npc_id;
		response_packet_sc.is_gamble_menu_open = income_packet_cs->is_gamble_menu_open;
		singleton<server>::instance().send_packet(player->player_data->net_client, &response_packet_sc, sizeof response_packet_sc);
	}

	if (income_packet_cs->command == COMMAND_FILL_NPC_INVENTORY) {
		// fill the merchant's inventory
		npcrecord->bRefreshInventory = true;
		diablo2::d2_game::create_vendor_cache1(game, player, ptNPC, 1, false);
		npcrecord->bRefreshInventory = false;

		// send an update packet to all connected clients
		// net client id - 1 is always the host, then every odd number: 3, 5, 7, 9, 11, 13, 15
		for (uint32_t i = 0; i < 16; i++) {
			diablo2::structures::net_client* netclient = diablo2::d2_game::get_net_client_from_id(game, i);
			if (netclient != 0) {
				response_packet_sc.command = COMMAND_FILL_NPC_INVENTORY;
				response_packet_sc.npc_id = income_packet_cs->npc_id;
				response_packet_sc.is_gamble_menu_open = income_packet_cs->is_gamble_menu_open;
				singleton<server>::instance().send_packet(netclient, &response_packet_sc, sizeof response_packet_sc);
			}
		}
	}

	if (income_packet_cs->command == COMMAND_FILL_NPC_GAMBLE) {
		diablo2::d2_game::create_vendor_cache1(game, player, ptNPC, 1, true);
		response_packet_sc.command = COMMAND_FILL_NPC_GAMBLE;
		response_packet_sc.npc_id = income_packet_cs->npc_id;
		response_packet_sc.is_gamble_menu_open = income_packet_cs->is_gamble_menu_open;
		singleton<server>::instance().send_packet(player->player_data->net_client, &response_packet_sc, sizeof response_packet_sc);
	}

	return true;
}