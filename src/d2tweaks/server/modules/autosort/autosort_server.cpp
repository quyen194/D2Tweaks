#include <d2tweaks/server/modules/autosort/autosort_server.h>

#include <d2tweaks/server/server.h>

#include <vector>
#include <spdlog/spdlog.h>

#include <d2tweaks/common/protocol.h>

#include <diablo2/d2common.h>
#include <diablo2/d2game.h>

#include <diablo2/structures/path.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>
#include <diablo2/structures/player_data.h>

MODULE_INIT(autosort)

struct backup_item {
	diablo2::structures::unit* item;
	uint8_t x;
	uint8_t y;

	backup_item() : item(nullptr), x(0), y(0) {}
};

struct packed_area {
	uint8_t x;
	uint8_t y;
	uint8_t w;
	uint8_t h;
};

// Define variables to store the inventory zone values
int iminValidX = GetPrivateProfileInt("InventoryZone", "iminValidX", 0, "./D2Tweaks.ini");
int imaxValidX = GetPrivateProfileInt("InventoryZone", "imaxValidX", 0, "./D2Tweaks.ini");
int iminValidY = GetPrivateProfileInt("InventoryZone", "iminValidY", 0, "./D2Tweaks.ini");
int imaxValidY = GetPrivateProfileInt("InventoryZone", "imaxValidY", 0, "./D2Tweaks.ini");

// Define variables to store the charm zone values
int cminValidX = GetPrivateProfileInt("CharmZone", "cminValidX", 0, "./D2Tweaks.ini");
int cmaxValidX = GetPrivateProfileInt("CharmZone", "cmaxValidX", 0, "./D2Tweaks.ini");
int cminValidY = GetPrivateProfileInt("CharmZone", "cminValidY", 0, "./D2Tweaks.ini");
int cmaxValidY = GetPrivateProfileInt("CharmZone", "cmaxValidY", 0, "./D2Tweaks.ini");

void d2_tweaks::server::modules::autosort::init() {
	char acPathToIni[MAX_PATH] = { 0 };
	const char* pcIniFile = "\\d2tweaks.ini";

	GetCurrentDirectory(MAX_PATH, acPathToIni);
	lstrcat(acPathToIni, pcIniFile);

	if (GetPrivateProfileInt("modules", "Autosort", 1, acPathToIni) != FALSE) {
		singleton<server>::instance().register_packet_handler(common::MESSAGE_TYPE_INVENTORY_SORT, this);
	}
}

bool d2_tweaks::server::modules::autosort::handle_packet(diablo2::structures::game* game,
	diablo2::structures::unit* player, common::packet_header* packet) {
	if (static_cast<common::inventory_sort_cs*>(packet)->remItem == 1) {
		diablo2::d2_common::inv_remove_item(player->inventory, static_cast<common::inventory_sort_cs*>(packet)->item_to_remove);
		diablo2::d2_game::update_inventory_items(game, player);

		static_cast<common::inventory_sort_cs*>(packet)->item_to_remove = nullptr;

		for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
			if (item == static_cast<common::inventory_sort_cs*>(packet)->item_to_remove) {
				diablo2::d2_common::inv_remove_item(player->inventory, item);
				diablo2::d2_game::update_inventory_items(game, player);
			}
		}

		MessageBoxA(NULL, "Item removed", "Item removed", MB_OK);
	}
	else {
		sort(game, player, static_cast<common::inventory_sort_cs*>(packet)->page);
	}
	return true;
}

bool d2_tweaks::server::modules::autosort::sort(diablo2::structures::game* game, diablo2::structures::unit* player, uint8_t page) {
	static common::inventory_sort_sc packet;
	static auto& instance = singleton<server>::instance();

	if (player == nullptr)
		return false;

	const auto inventoryIndex = diablo2::d2_common::get_inventory_index(player, page, game->item_format == 101);
	char data[0x18];
	diablo2::d2_common::get_inventory_data(inventoryIndex, 0, data);

	const auto inventoryWidth = static_cast<uint32_t>(data[0]);
	const auto inventoryHeight = static_cast<uint32_t>(data[1]);

	std::vector<packed_area> areas{ {0,0, static_cast<uint8_t>(inventoryWidth), static_cast<uint8_t>(inventoryHeight)} };
	std::vector<diablo2::structures::unit*> items;
	std::vector<diablo2::structures::unit*> charms;
	std::vector<backup_item> backup_items;
	std::unordered_map<uint32_t, std::vector<diablo2::structures::unit*>> items_typed;
	std::unordered_map<uint32_t, std::vector<diablo2::structures::unit*>> charms_typed;
	uint32_t occupied_cells = 0;

	for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
		if (item->item_data->page != page)
			continue;

		const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
		const auto record_type = diablo2::d2_common::get_item_type_record(record->type);
		bool is_charm = record_type->charm;

		backup_item backup;
		backup.item = item;
		backup.x = item->path->x;
		backup.y = item->path->y;

		backup_items.push_back(backup);

		// only check charmzone grid for inventory page 0
			// Check if item's coordinates are within the valid range
		if (is_charm) {
			// Coordinates are within the valid range, add the item to items vector
			charms_typed[item->data_record_index].push_back(item);
			charms.push_back(item);
			occupied_cells += record->inv_height * record->inv_width;
		}
		else {
			// Coordinates are within the valid range, add the item to items vector
			items_typed[item->data_record_index].push_back(item);
			items.push_back(item);
			occupied_cells += record->inv_height * record->inv_width;
		}

		//spdlog::info("--------------------------------");
		//spdlog::info("Item Type: {0}", record->type);
		//spdlog::info("Item Type 2: {0}", record->type2);
		//spdlog::info("Occupied Cells: {0}", occupied_cells);
		//spdlog::info("x: {0}", backup.x);
		//spdlog::info("y: {0}", backup.y);
		//spdlog::info("is_charm: {0}\n\n", is_charm);
	}

	if (occupied_cells > inventoryHeight * inventoryWidth) {
		//should never be happen in normal cases
		spdlog::warn("occupied_cells > inventoryHeight* inventoryWidth");
		return false;
	}

	// Remove all items from inventory
	for (auto item : items)
		diablo2::d2_common::inv_remove_item(player->inventory, item);
	for (auto item : charms)
		diablo2::d2_common::inv_remove_item(player->inventory, item);

	const auto itemsCount = items.size();
	const auto charmsCount = charms.size();

	if (itemsCount == 0)
		return true; //there's nothing to sort

	auto success = NULL;

	if (itemsCount > 0) {
		//sort items by height*width
		for (size_t i = 0; i < itemsCount - 1; i++) {
			auto swapped = false;
			for (size_t j = 0; j < itemsCount - i - 1; j++) {
				const auto record1 = diablo2::d2_common::get_item_record(items[j]->data_record_index);
				const auto record2 = diablo2::d2_common::get_item_record(items[j + 1]->data_record_index);

				if (record1->inv_height * record1->inv_width > record2->inv_height * record2->inv_width)
					continue;

				const auto temp = items[j];
				items[j] = items[j + 1];
				items[j + 1] = temp;
				swapped = true;
			}

			if (swapped == false)
				break;
		}
		success = TRUE;
		for (auto sorted_item : items) {
			for (auto& item : items_typed[sorted_item->data_record_index]) { //iterate through all items of the same type for grouping
				const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

				uint32_t tx, ty;

				if (!find_free_space(player->inventory, item, inventoryIndex, page, tx, ty, false)) {
					success = false;
					break;
				}

				if (!diablo2::d2_common::inv_add_item(player->inventory, item, tx, ty, inventoryIndex, false, page)) {
					success = false;
					break;
				}

				if (diablo2::d2_common::inv_update_item(player->inventory, item, false))
					continue;

				success = false;
				break;
			}

			if (!success)
				break;

			items_typed.erase(sorted_item->data_record_index);
		}
	}

	if (charmsCount > 0) {
		//sort charms by height*width
		for (size_t i = 0; i < charmsCount - 1; i++) {
			auto swapped = false;
			for (size_t j = 0; j < charmsCount - i - 1; j++) {
				const auto record1 = diablo2::d2_common::get_item_record(charms[j]->data_record_index);
				const auto record2 = diablo2::d2_common::get_item_record(charms[j + 1]->data_record_index);

				if (record1->inv_height * record1->inv_width > record2->inv_height * record2->inv_width)
					continue;

				const auto temp = charms[j];
				charms[j] = charms[j + 1];
				charms[j + 1] = temp;
				swapped = true;
			}

			if (swapped == false)
				break;
		}
		success = TRUE;
		for (auto sorted_item : charms) {
			for (auto& item : charms_typed[sorted_item->data_record_index]) { //iterate through all items of the same type for grouping
				const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

				uint32_t tx, ty;

				if (!find_free_space(player->inventory, item, inventoryIndex, page, tx, ty, true)) {
					success = false;
					break;
				}

				if (!diablo2::d2_common::inv_add_item(player->inventory, item, tx, ty, inventoryIndex, false, page)) {
					success = false;
					break;
				}

				if (diablo2::d2_common::inv_update_item(player->inventory, item, false))
					continue;

				success = false;
				break;
			}

			if (!success)
				break;

			charms_typed.erase(sorted_item->data_record_index);
		}
	}

	if (success) {
		while (items.size() > 0) {
			packet.page = page;
			packet.tx = items.back()->path->x;
			packet.ty = items.back()->path->y;
			packet.guid = items.back()->guid;

			items.pop_back();

			instance.send_packet(player->player_data->net_client, &packet, sizeof packet);
		}

		while (charms.size() > 0) {
			packet.page = page;
			packet.tx = charms.back()->path->x;
			packet.ty = charms.back()->path->y;
			packet.guid = charms.back()->guid;

			charms.pop_back();

			instance.send_packet(player->player_data->net_client, &packet, sizeof packet);
		}

		diablo2::d2_game::update_inventory_items(game, player);
		return true;
	}

	//sorting failed, remove all items and re-insert them at old coords

	for (auto& backup_item : backup_items) {
		const auto item = backup_item.item;

		diablo2::d2_common::inv_remove_item(player->inventory, item);
	}

	while (backup_items.size() > 0) {
		const auto backup = backup_items.back();
		const auto item = backup.item;
		const auto tx = backup.x;
		const auto ty = backup.y;

		packet.page = page;
		packet.tx = tx;
		packet.ty = ty;
		packet.guid = item->guid;

		diablo2::d2_common::inv_add_item(player->inventory, item, tx, ty, inventoryIndex, false, page);
		diablo2::d2_common::inv_update_item(player->inventory, item, false);

		backup_items.pop_back();

		instance.send_packet(player->player_data->net_client, &packet, sizeof packet);
	}

	diablo2::d2_game::update_inventory_items(game, player);

	return false;
}

bool d2_tweaks::server::modules::autosort::find_free_space(diablo2::structures::inventory* inv,
	diablo2::structures::unit* item,
	int32_t inventoryIndex, char page, uint32_t& x,
	uint32_t& y, bool isCharmZone) {
	char data[0x18];
	diablo2::d2_common::get_inventory_data(inventoryIndex, 0, data);

	// inventory coords
	const auto mx = static_cast<uint32_t>(data[0]);
	const auto my = static_cast<uint32_t>(data[1]);

	const unsigned int minValidX = isCharmZone ? cminValidX : iminValidX;
	const unsigned int maxValidX = isCharmZone ? cmaxValidX : imaxValidX;
	const unsigned int minValidY = isCharmZone ? cminValidY : iminValidY;
	const unsigned int maxValidY = isCharmZone ? cmaxValidY : imaxValidY;

	// Get the item record to access the item size
	const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

	// Update the maximum valid Y coordinate for the inventory zone
	const unsigned int inventoryMaxValidY = isCharmZone ? maxValidY : 11;

	if (page == 0x03 || page == 0x04) {
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
	else {
		for (x = minValidX; x <= maxValidX - record->inv_width + 1; x++) {
			for (y = minValidY; y <= inventoryMaxValidY - record->inv_height + 1; y++) {
				diablo2::structures::unit* blockingUnit = nullptr;
				uint32_t blockingUnitIndex = 0;

				// Check if the item can be placed at the current position without overflowing
				if (diablo2::d2_common::can_put_into_slot(inv, item, x, y, inventoryIndex, &blockingUnit, &blockingUnitIndex, page))
					return true;
			}
		}
		return false;
	}
}
