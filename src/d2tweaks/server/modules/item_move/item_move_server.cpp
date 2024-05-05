#include <d2tweaks/server/modules/item_move/item_move_server.h>

#include <d2tweaks/server/server.h>

#include <diablo2/d2net.h>
#include <diablo2/d2game.h>
#include <diablo2/d2common.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/net_client.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/player_data.h>

#include <spdlog/spdlog.h>

#include <fstream>
#include <iostream>
#include <diablo2/d2client.h>

#include <iomanip> // For std::setw
#include <sstream>
#include <string>
#include <stdexcept> // For std::invalid_argument

#include <windows.h> // Include Windows API header for MessageBox

#include <iomanip> // For std::setw

enum D2C_ItemQualities
{
	ITEMQUAL_INFERIOR = 0x01, 	//0x01 Inferior
	ITEMQUAL_NORMAL = 0x02, 	//0x02 Normal
	ITEMQUAL_SUPERIOR = 0x03, 	//0x03 Superior
	ITEMQUAL_MAGIC = 0x04, 		//0x04 Magic
	ITEMQUAL_SET = 0x05, 		//0x05 Set
	ITEMQUAL_RARE = 0x06, 		//0x06 Rare
	ITEMQUAL_UNIQUE = 0x07, 	//0x07 Unique
	ITEMQUAL_CRAFT = 0x08, 		//0x08 Crafted
	ITEMQUAL_TEMPERED = 0x09	//0x09 Tempered
};


void serialize_item(const std::string& itemcode, const diablo2::structures::unit& item, std::ofstream& file) {
	// Write item code
	file << itemcode << ":";

	// Write serialized data
	for (size_t i = 0; i < sizeof(item); ++i) {
		file << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(reinterpret_cast<const char*>(&item)[i]);
	}

	// Add a newline after each item
	file << std::endl;
}

MODULE_INIT(item_move)

void d2_tweaks::server::modules::item_move::init() {
	char acPathToIni[MAX_PATH] = { 0 };
	const char* pcIniFile = "\\d2tweaks.ini";

	GetCurrentDirectory(MAX_PATH, acPathToIni);
	lstrcat(acPathToIni, pcIniFile);

	if (GetPrivateProfileInt("modules", "ItemMover", 1, acPathToIni) != FALSE) {
		singleton<server>::instance().register_packet_handler(common::MESSAGE_TYPE_ITEM_MOVE, this);
	}
}

// handle packet coming from the client
bool d2_tweaks::server::modules::item_move::handle_packet(diablo2::structures::game* game,
	diablo2::structures::unit* player, common::packet_header* packet) {
	static common::item_move_sc resp;
	static auto& instance = singleton<server>::instance();

	const auto itemMove = static_cast<common::item_move_cs*>(packet);
	const auto key = static_cast<common::item_move_cs*>(packet)->item_code;

	// Display key in a message box
	//MessageBox(NULL, key, "Item code", MB_OK | MB_ICONINFORMATION);

	const auto item = instance.get_server_unit(game, itemMove->item_guid, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM); //0x4 = item

	const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

	const char* itemcode = itemMove->item_code;
	const auto bag = instance.get_server_unit(game, itemMove->bag_guid, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM); //0x4 = item

	if (itemMove->tmog == 1) {
		// here we need to add item to inventory

		const auto item = instance.get_server_unit(game, itemMove->item_guid, diablo2::structures::unit_type_t::UNIT_TYPE_ITEM); //0x4 = item

		diablo2::d2_game::D2GAME_Transmogrify_6FC4A660(game, player, item);

		//auto gemCode = diablo2::d2_common::get_item_id_from_item_code(' vfg');
		//diablo2::structures::D2ItemDropStrc itemDrop = {};
		//diablo2::structures::D2CoordStrc pReturnCoords = {};
		//auto player = diablo2::d2_client::get_local_player();
		//itemDrop.pSeed = nullptr;
		//itemDrop.nX = pReturnCoords.nX;
		//itemDrop.nY = pReturnCoords.nY;
		//itemDrop.wItemFormat = game->item_format;
		//itemDrop.pRoom = 0;
		//itemDrop.nQuality = ITEMQUAL_NORMAL;
		//itemDrop.pUnit = player;
		//itemDrop.pGame = game;
		//itemDrop.nId = gemCode;
		//itemDrop.nSpawnType = 4;
		//itemDrop.wUnitInitFlags = 1;
		//itemDrop.nItemLvl = 1;
		//diablo2::structures::unit* nItem = diablo2::d2_game::D2GAME_CreateItemEx_6FC4ED80(game, &itemDrop, 0);

		//Display nITem->guid in a messagebox
		//std::string guid = std::to_string(nItem->guid);
		//MessageBox(NULL, guid.c_str(), "Item GUID", MB_OK | MB_ICONINFORMATION);

		//const auto inventoryIndex = diablo2::d2_common::get_inventory_index(player, 0, game->item_format == 101);
		//uint32_t tx, ty;

		//if (!find_free_space(player->inventory, nItem, inventoryIndex, 0, tx, ty))
		//	return true; //block further packet processing
		//
		//nItem->item_data->page = 0;

		//diablo2::d2_common::inv_add_item(player->inventory, nItem, tx, ty, inventoryIndex, false, nItem->item_data->page);
		//diablo2::d2_common::inv_update_item(player->inventory, nItem, false);

		//diablo2::d2_game::update_inventory_items(game, player);

		////send update packet
		//resp.item_guid = nItem->guid;
		//resp.target_page = 0;
		//resp.tx = tx;
		//resp.ty = ty;

		//const auto client = player->player_data->net_client;

		//diablo2::d2_net::send_to_client(1, client->client_id, &resp, sizeof resp);

	}
	else {
		D2PropertyStrc itemProperty = {};
		itemProperty.nProperty = itemMove->prop;
		itemProperty.nLayer = 0;
		itemProperty.nMin = itemMove->val;
		itemProperty.nMax = itemMove->val;
		diablo2::d2_common::add_property(bag, &itemProperty, 1);

		if (item == nullptr)
			return true; //block further packet processing

		const auto inventoryIndex = diablo2::d2_common::get_inventory_index(player, itemMove->target_page, game->item_format == 101);

		uint32_t tx, ty;

		if (!find_free_space(player->inventory, item, inventoryIndex, itemMove->target_page, tx, ty))
			return true; //block further packet processing

		//diablo2::d2_common::set_unit_mode(item, 0); // mode 4 - предмет нельзя взять мышкой из инвентаря, mode 0 - обычный режим

		item->item_data->page = itemMove->target_page;

		diablo2::d2_common::inv_add_item(player->inventory, item, tx, ty, inventoryIndex, false, item->item_data->page);
		diablo2::d2_common::inv_update_item(player->inventory, item, false);

		diablo2::d2_game::update_inventory_items(game, player);

		//send update packet
		resp.item_guid = itemMove->item_guid;
		resp.target_page = itemMove->target_page;
		resp.tx = tx;
		resp.ty = ty;

		const auto client = player->player_data->net_client;

		diablo2::d2_net::send_to_client(1, client->client_id, &resp, sizeof resp);

		return true;
	}
}

bool d2_tweaks::server::modules::item_move::find_free_space(diablo2::structures::inventory* inv,
	diablo2::structures::unit* item, int32_t inventoryIndex, char page, uint32_t& x, uint32_t& y) {
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