#include <d2tweaks/client/modules/loot_filter/loot_filter_settings_menu.h>
#include <d2tweaks/client/modules/loot_filter/loot_filter_settings.h>
#include <d2tweaks/client/client.h>
#include <common/hooking.h>
#include <common/asm_code.h>

#include <Windows.h>

#include <diablo2/d2client.h>
#include <diablo2/d2common.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>

#include <d2tweaks/ui/controls/checkbox.h>
#include <DllNotify.h>
#include <D2Template.h>
#include <d2tweaks/common/protocol.h>

#include <d2tweaks/client/modules/autosort/autosort_client.h>

#include <d2tweaks/client/client.h>

#include <spdlog/spdlog.h>

#include <d2tweaks/common/common.h>
#include <d2tweaks/common/protocol.h>
#include <d2tweaks/common/asset_manager.h>

#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>
#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/controls/button.h>

#include <diablo2/d2common.h>
#include <diablo2/d2client.h>
#include <diablo2/d2win.h>
#include <diablo2/d2gfx.h>
#include <diablo2/d2cmp.h>

#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/player_data.h>

#include <diablo2/structures/path.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <time.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <map>

#include <DllNotify.h>
#include <D2Template.h>

#include <diablo2/d2gfx.h>

#include <string>
#include <vector>
#include <sstream>

#include <string>
#include <locale>
#include <codecvt>

d2_tweaks::client::modules::loot_filter_settings_menu::loot_filter_settings_menu(token) {
	menu::set_enabled(false);
	menu::set_visible(false);

	//load_xml("d2tweaks\\interfaces\\loot_filter_settings_menu.xml");
	if (DLLBASE_D2EXPRES != 0)
		load_xml("d2tweaks\\interface_d2expres\\loot_filter_settings_menu.xml");
	if (DLLBASE_SGD2FREERES != 0)
		load_xml("d2tweaks\\interface_sgd2freeres\\loot_filter_settings_menu.xml");
	if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
		load_xml("d2tweaks\\interface_vanilla\\loot_filter_settings_menu.xml");

	register_misc_checkboxes();
	register_quality_checkboxes();

	setup_hooks();
}

void d2_tweaks::client::modules::loot_filter_settings_menu::reload_settings() {
	register_misc_checkboxes();
	register_quality_checkboxes();
}

void d2_tweaks::client::modules::loot_filter_settings_menu::draw() {
	if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MAINMENU))
		return;

	menu::draw();
}

void d2_tweaks::client::modules::loot_filter_settings_menu::register_misc_checkboxes() {
	m_altonly = get_control<ui::controls::checkbox>("m_altonly");
	m_show_gold = get_control<ui::controls::checkbox>("m_show_gold");
	m_show_runes = get_control<ui::controls::checkbox>("m_show_runes");
	m_show_gems = get_control<ui::controls::checkbox>("m_show_gems");

	m_show_amethyst = get_control<ui::controls::checkbox>("m_show_amethyst");
	m_show_diamond = get_control<ui::controls::checkbox>("m_show_diamond");
	m_show_emerald = get_control<ui::controls::checkbox>("m_show_emerald");
	m_show_ruby = get_control<ui::controls::checkbox>("m_show_ruby");
	m_show_sapphire = get_control<ui::controls::checkbox>("m_show_sapphire");
	m_show_skull = get_control<ui::controls::checkbox>("m_show_skull");
	m_show_topaz = get_control<ui::controls::checkbox>("m_show_topaz");

	m_show_r01 = get_control<ui::controls::checkbox>("m_show_r01");
	m_show_r02 = get_control<ui::controls::checkbox>("m_show_r02");
	m_show_r03 = get_control<ui::controls::checkbox>("m_show_r03");
	m_show_r04 = get_control<ui::controls::checkbox>("m_show_r04");
	m_show_r05 = get_control<ui::controls::checkbox>("m_show_r05");
	m_show_r06 = get_control<ui::controls::checkbox>("m_show_r06");
	m_show_r07 = get_control<ui::controls::checkbox>("m_show_r07");
	m_show_r08 = get_control<ui::controls::checkbox>("m_show_r08");
	m_show_r09 = get_control<ui::controls::checkbox>("m_show_r09");
	m_show_r10 = get_control<ui::controls::checkbox>("m_show_r10");
	m_show_r11 = get_control<ui::controls::checkbox>("m_show_r11");
	m_show_r12 = get_control<ui::controls::checkbox>("m_show_r12");
	m_show_r13 = get_control<ui::controls::checkbox>("m_show_r13");
	m_show_r14 = get_control<ui::controls::checkbox>("m_show_r14");
	m_show_r15 = get_control<ui::controls::checkbox>("m_show_r15");
	m_show_r16 = get_control<ui::controls::checkbox>("m_show_r16");
	m_show_r17 = get_control<ui::controls::checkbox>("m_show_r17");
	m_show_r18 = get_control<ui::controls::checkbox>("m_show_r18");
	m_show_r19 = get_control<ui::controls::checkbox>("m_show_r19");
	m_show_r20 = get_control<ui::controls::checkbox>("m_show_r20");
	m_show_r21 = get_control<ui::controls::checkbox>("m_show_r21");
	m_show_r22 = get_control<ui::controls::checkbox>("m_show_r22");
	m_show_r23 = get_control<ui::controls::checkbox>("m_show_r23");
	m_show_r24 = get_control<ui::controls::checkbox>("m_show_r24");
	m_show_r25 = get_control<ui::controls::checkbox>("m_show_r25");
	m_show_r26 = get_control<ui::controls::checkbox>("m_show_r26");
	m_show_r27 = get_control<ui::controls::checkbox>("m_show_r27");
	m_show_r28 = get_control<ui::controls::checkbox>("m_show_r28");
	m_show_r29 = get_control<ui::controls::checkbox>("m_show_r29");
	m_show_r30 = get_control<ui::controls::checkbox>("m_show_r30");
	m_show_r31 = get_control<ui::controls::checkbox>("m_show_r31");
	m_show_r32 = get_control<ui::controls::checkbox>("m_show_r32");
	m_show_r33 = get_control<ui::controls::checkbox>("m_show_r33");


	m_show_chipped = get_control<ui::controls::checkbox>("m_show_chipped");
	m_show_flawed = get_control<ui::controls::checkbox>("m_show_flawed");
	m_show_normal = get_control<ui::controls::checkbox>("m_show_normal");
	m_show_flawless = get_control<ui::controls::checkbox>("m_show_flawless");
	m_show_perfect = get_control<ui::controls::checkbox>("m_show_perfect");


	if (m_show_r01) {
		m_show_r01->set_state(loot_filter_settings::get().m_show_r01);
		m_show_r01->set_on_click(std::bind(&loot_filter_settings_menu::extract_r01,
			this, std::placeholders::_1));
	}

	if (m_show_r02) {
		m_show_r02->set_state(loot_filter_settings::get().m_show_r02);
		m_show_r02->set_on_click(std::bind(&loot_filter_settings_menu::extract_r02,
			this, std::placeholders::_1));
	}

	if (m_show_r03) {
		m_show_r03->set_state(loot_filter_settings::get().m_show_r03);
		m_show_r03->set_on_click(std::bind(&loot_filter_settings_menu::extract_r03,
			this, std::placeholders::_1));
	}

	if (m_show_r04) {
		m_show_r04->set_state(loot_filter_settings::get().m_show_r04);
		m_show_r04->set_on_click(std::bind(&loot_filter_settings_menu::extract_r04,
			this, std::placeholders::_1));
	}

	if (m_show_r05) {
		m_show_r05->set_state(loot_filter_settings::get().m_show_r05);
		m_show_r05->set_on_click(std::bind(&loot_filter_settings_menu::extract_r05,
			this, std::placeholders::_1));
	}

	if (m_show_r06) {
		m_show_r06->set_state(loot_filter_settings::get().m_show_r06);
		m_show_r06->set_on_click(std::bind(&loot_filter_settings_menu::extract_r06,
			this, std::placeholders::_1));
	}

	if (m_show_r07) {
		m_show_r07->set_state(loot_filter_settings::get().m_show_r07);
		m_show_r07->set_on_click(std::bind(&loot_filter_settings_menu::extract_r07,
			this, std::placeholders::_1));
	}

	if (m_show_r08) {
		m_show_r08->set_state(loot_filter_settings::get().m_show_r08);
		m_show_r08->set_on_click(std::bind(&loot_filter_settings_menu::extract_r08,
			this, std::placeholders::_1));
	}

	if (m_show_r09) {
		m_show_r09->set_state(loot_filter_settings::get().m_show_r09);
		m_show_r09->set_on_click(std::bind(&loot_filter_settings_menu::extract_r09,
			this, std::placeholders::_1));
	}

	if (m_show_r10) {
		m_show_r10->set_state(loot_filter_settings::get().m_show_r10);
		m_show_r10->set_on_click(std::bind(&loot_filter_settings_menu::extract_r10,
			this, std::placeholders::_1));
	}

	if (m_show_r11) {
		m_show_r11->set_state(loot_filter_settings::get().m_show_r11);
		m_show_r11->set_on_click(std::bind(&loot_filter_settings_menu::extract_r11,
			this, std::placeholders::_1));
	}

	if (m_show_r12) {
		m_show_r12->set_state(loot_filter_settings::get().m_show_r12);
		m_show_r12->set_on_click(std::bind(&loot_filter_settings_menu::extract_r12,
			this, std::placeholders::_1));
	}

	if (m_show_r13) {
		m_show_r13->set_state(loot_filter_settings::get().m_show_r13);
		m_show_r13->set_on_click(std::bind(&loot_filter_settings_menu::extract_r13,
			this, std::placeholders::_1));
	}

	if (m_show_r14) {
		m_show_r14->set_state(loot_filter_settings::get().m_show_r14);
		m_show_r14->set_on_click(std::bind(&loot_filter_settings_menu::extract_r14,
			this, std::placeholders::_1));
	}

	if (m_show_r15) {
		m_show_r15->set_state(loot_filter_settings::get().m_show_r15);
		m_show_r15->set_on_click(std::bind(&loot_filter_settings_menu::extract_r15,
			this, std::placeholders::_1));
	}

	if (m_show_r16) {
		m_show_r16->set_state(loot_filter_settings::get().m_show_r16);
		m_show_r16->set_on_click(std::bind(&loot_filter_settings_menu::extract_r16,
			this, std::placeholders::_1));
	}

	if (m_show_r17) {
		m_show_r17->set_state(loot_filter_settings::get().m_show_r17);
		m_show_r17->set_on_click(std::bind(&loot_filter_settings_menu::extract_r17,
			this, std::placeholders::_1));
	}

	if (m_show_r18) {
		m_show_r18->set_state(loot_filter_settings::get().m_show_r18);
		m_show_r18->set_on_click(std::bind(&loot_filter_settings_menu::extract_r18,
			this, std::placeholders::_1));
	}

	if (m_show_r19) {
		m_show_r19->set_state(loot_filter_settings::get().m_show_r19);
		m_show_r19->set_on_click(std::bind(&loot_filter_settings_menu::extract_r19,
			this, std::placeholders::_1));
	}

	if (m_show_r20) {
		m_show_r20->set_state(loot_filter_settings::get().m_show_r20);
		m_show_r20->set_on_click(std::bind(&loot_filter_settings_menu::extract_r20,
			this, std::placeholders::_1));
	}

	if (m_show_r21) {
		m_show_r21->set_state(loot_filter_settings::get().m_show_r21);
		m_show_r21->set_on_click(std::bind(&loot_filter_settings_menu::extract_r21,
			this, std::placeholders::_1));
	}

	if (m_show_r22) {
		m_show_r22->set_state(loot_filter_settings::get().m_show_r22);
		m_show_r22->set_on_click(std::bind(&loot_filter_settings_menu::extract_r22,
			this, std::placeholders::_1));
	}

	if (m_show_r23) {
		m_show_r23->set_state(loot_filter_settings::get().m_show_r23);
		m_show_r23->set_on_click(std::bind(&loot_filter_settings_menu::extract_r23,
			this, std::placeholders::_1));
	}

	if (m_show_r24) {
		m_show_r24->set_state(loot_filter_settings::get().m_show_r24);
		m_show_r24->set_on_click(std::bind(&loot_filter_settings_menu::extract_r24,
			this, std::placeholders::_1));
	}

	if (m_show_r25) {
		m_show_r25->set_state(loot_filter_settings::get().m_show_r25);
		m_show_r25->set_on_click(std::bind(&loot_filter_settings_menu::extract_r25,
			this, std::placeholders::_1));
	}

	if (m_show_r26) {
		m_show_r26->set_state(loot_filter_settings::get().m_show_r26);
		m_show_r26->set_on_click(std::bind(&loot_filter_settings_menu::extract_r26,
			this, std::placeholders::_1));
	}

	if (m_show_r27) {
		m_show_r27->set_state(loot_filter_settings::get().m_show_r27);
		m_show_r27->set_on_click(std::bind(&loot_filter_settings_menu::extract_r27,
			this, std::placeholders::_1));
	}

	if (m_show_r28) {
		m_show_r28->set_state(loot_filter_settings::get().m_show_r28);
		m_show_r28->set_on_click(std::bind(&loot_filter_settings_menu::extract_r28,
			this, std::placeholders::_1));
	}

	if (m_show_r29) {
		m_show_r29->set_state(loot_filter_settings::get().m_show_r29);
		m_show_r29->set_on_click(std::bind(&loot_filter_settings_menu::extract_r29,
			this, std::placeholders::_1));
	}

	if (m_show_r30) {
		m_show_r30->set_state(loot_filter_settings::get().m_show_r30);
		m_show_r30->set_on_click(std::bind(&loot_filter_settings_menu::extract_r30,
			this, std::placeholders::_1));
	}

	if (m_show_r31) {
		m_show_r31->set_state(loot_filter_settings::get().m_show_r31);
		m_show_r31->set_on_click(std::bind(&loot_filter_settings_menu::extract_r31,
			this, std::placeholders::_1));
	}

	if (m_show_r32) {
		m_show_r32->set_state(loot_filter_settings::get().m_show_r32);
		m_show_r32->set_on_click(std::bind(&loot_filter_settings_menu::extract_r32,
			this, std::placeholders::_1));
	}

	if (m_show_r33) {
		m_show_r33->set_state(loot_filter_settings::get().m_show_r33);
		m_show_r33->set_on_click(std::bind(&loot_filter_settings_menu::extract_r33,
			this, std::placeholders::_1));
	}





















	if (m_altonly) {
		m_altonly->set_state(loot_filter_settings::get().alt_only);
		m_altonly->set_on_click(std::bind(&loot_filter_settings_menu::update_alt_only,
			this, std::placeholders::_1));
	}

	if (m_show_gold) {
		m_show_gold->set_state(loot_filter_settings::get().show_gold);
		m_show_gold->set_on_click(std::bind(&loot_filter_settings_menu::update_show_gold,
			this, std::placeholders::_1));
	}

	if (m_show_runes) {
		m_show_runes->set_state(loot_filter_settings::get().show_runes);
		m_show_runes->set_on_click(std::bind(&loot_filter_settings_menu::update_show_runes,
			this, std::placeholders::_1));
	}

	if (m_show_gems) {
		m_show_gems->set_state(loot_filter_settings::get().show_gems);
		m_show_gems->set_on_click(std::bind(&loot_filter_settings_menu::update_show_gems,
			this, std::placeholders::_1));
	}
}

void d2_tweaks::client::modules::loot_filter_settings_menu::register_quality_checkboxes() {
	using namespace diablo2::structures;

	register_quality_checkbox("m_inferior", item_quality_t::ITEM_QUALITY_INFERIOR);
	register_quality_checkbox("m_normal", item_quality_t::ITEM_QUALITY_NORMAL);
	register_quality_checkbox("m_superior", item_quality_t::ITEM_QUALITY_SUPERIOR);
	register_quality_checkbox("m_magic", item_quality_t::ITEM_QUALITY_MAGIC);
	register_quality_checkbox("m_set", item_quality_t::ITEM_QUALITY_SET);
	register_quality_checkbox("m_rare", item_quality_t::ITEM_QUALITY_RARE);
	register_quality_checkbox("m_unique", item_quality_t::ITEM_QUALITY_UNIQUE);
	register_quality_checkbox("m_crafted", item_quality_t::ITEM_QUALITY_CRAFTED);
	register_quality_checkbox("m_tempered", item_quality_t::ITEM_QUALITY_TEMPERED);
}

void d2_tweaks::client::modules::loot_filter_settings_menu::update_alt_only(bool value) {
	loot_filter_settings::get().alt_only = value;
	loot_filter_settings::get().save(diablo2::d2_client::get_local_player_name());
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r01(bool value) {
	loot_filter_settings::get().m_show_r01 = value;

	// display m_show_r01 value in a messagebox
	//MessageBoxA(NULL, value ? "true" : "false", "m_show_r01", MB_OK);

	auto player = diablo2::d2_client::get_local_player();
	auto inventory = player->inventory;

	diablo2::structures::unit* bag;
	uint32_t bagGuid;
	uint32_t statValue;

	// iterate over all items in player inventory
	for (auto item = inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
		const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
		auto recordType = diablo2::d2_common::get_item_type_record(record->type);
		char* normCode1 = record->string_code;
		if (strncmp(normCode1, "ib1", 3) == 0) {
			bag = item;
			bagGuid = item->guid;
			// get item stat
			statValue = diablo2::d2_common::get_stat(item, diablo2::UNIT_STAT_runebag_RunesA, NULL);

		}
	}

	// show bag guid in a messagebox
	std::string bagGuidStr = std::to_string(bagGuid);
	//MessageBoxA(NULL, bagGuidStr.c_str(), "bagGuid", MB_OK);

	// show statValue in a messagebox
	std::string statValueStr = std::to_string(statValue);
	//MessageBoxA(NULL, statValueStr.c_str(), "statValue", MB_OK);

	if (statValue > 1) {
		// Create the packet
		static d2_tweaks::common::item_move_cs packet;
		packet.item_guid = bagGuid;
		packet.bag_guid = bagGuid;
		packet.target_page = 0;
		packet.extract = 1;
		packet.iCode = 'r01 ';
		diablo2::d2_client::send_to_server(&packet, sizeof packet);

		D2PropertyStrc itemProperty = {};
		itemProperty.nProperty = 388 - 3; // Adjust the property ID
		itemProperty.nLayer = 0;
		itemProperty.nMin = - 1;
		itemProperty.nMax = - 1;

		// Add the gem property to the gem bag
		diablo2::d2_common::add_property(bag, &itemProperty, 0);

	}

}


void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r02(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r03(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r04(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r05(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r06(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r07(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r08(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r09(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r10(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r11(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r12(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r13(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r14(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r15(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r16(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r17(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r18(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r19(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r20(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r21(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r22(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r23(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r24(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r25(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r26(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r27(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r28(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r29(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r30(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r31(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r32(bool value) {
}

void d2_tweaks::client::modules::loot_filter_settings_menu::extract_r33(bool value) {
}


void d2_tweaks::client::modules::loot_filter_settings_menu::update_show_gold(bool value) {
	loot_filter_settings::get().show_gold = value;
	loot_filter_settings::get().save(diablo2::d2_client::get_local_player_name());
}

void d2_tweaks::client::modules::loot_filter_settings_menu::update_show_runes(bool value) {
	loot_filter_settings::get().show_runes = value;
	loot_filter_settings::get().save(diablo2::d2_client::get_local_player_name());
}

void d2_tweaks::client::modules::loot_filter_settings_menu::update_show_gems(bool value) {
	loot_filter_settings::get().show_gems = value;
	loot_filter_settings::get().save(diablo2::d2_client::get_local_player_name());
}

void d2_tweaks::client::modules::loot_filter_settings_menu::update_quality_allowance(bool value,
	diablo2::structures::item_quality_t quality) {
	loot_filter_settings::get().quality_settings[static_cast<uint32_t>(quality)] = value;
	loot_filter_settings::get().save(diablo2::d2_client::get_local_player_name());
}

void d2_tweaks::client::modules::loot_filter_settings_menu::register_quality_checkbox(const std::string& name,
	diablo2::structures::item_quality_t quality) {
	auto control = get_control<ui::controls::checkbox>(name);

	if (!control)
		return;

	control->set_state(loot_filter_settings::get().quality_settings[static_cast<size_t>(quality)]);
	control->set_on_click(std::bind(&loot_filter_settings_menu::update_quality_allowance,
		this, std::placeholders::_1, quality));
}

void d2_tweaks::client::modules::loot_filter_settings_menu::setup_hooks() {
	m_handle_dropped_items_original =
		static_cast<decltype(m_handle_dropped_items_original)>(hooking::get_call(
			diablo2::d2_client::get_base() + 0x1641B));
	m_draw_dropped_items_names_original =
		static_cast<decltype(m_draw_dropped_items_names_original)>(hooking::get_call(
			diablo2::d2_client::get_base() + 0x81BF5));

	hooking::set_call(diablo2::d2_client::get_base() + 0x81BF5, draw_dropped_items_names);
	hooking::set_call(diablo2::d2_client::get_base() + 0x1641B, handle_dropped_items);

	setup_alt_hook();
}

void d2_tweaks::client::modules::loot_filter_settings_menu::setup_alt_hook() const {
	asm_code asmCode;
	asmCode.add({ 0x83, 0x7D, 0x00, 0x04 }); //cmp dword ptr [ebp+0], 4
	asmCode.add({ 0x0F, 0x85 },
		new asm_address_relative(2, 6, diablo2::d2_client::get_base() + 0x6A399)); //jnz D2Client.dll+6A399

	asmCode.add({ 0x56 }); //push esi
	asmCode.add({ 0xB8 }, new asm_address_static(1, check_alt_item)); //mov eax, check_alt_item
	asmCode.add({ 0x8B, 0xCD }); //mov ecx, ebp
	asmCode.add({ 0xFF, 0xD0 }); //call eax
	asmCode.add({ 0x5E }); //pop esi

	asmCode.add({ 0x84, 0xC0 }); //test al, al
	asmCode.add({ 0x0F, 0x84 },
		new asm_address_relative(2, 6, diablo2::d2_client::get_base() + 0x6A399)); //je D2Client.dll+6A399
	asmCode.add({ 0xE9 },
		new asm_address_relative(1, 5, diablo2::d2_client::get_base() + 0x6A027)); //jmp D2Client.dll+6A027

	auto addr = diablo2::d2_client::get_base() + 0x6A022;

	DWORD old_protect;
	VirtualProtect(addr, 5, PAGE_EXECUTE_READWRITE, &old_protect);

	memset(addr, 0x90, 10);
	*reinterpret_cast<unsigned char*>(addr) = 0xE9;
	*reinterpret_cast<int32_t*>(static_cast<char*>(addr) + 1) =
		reinterpret_cast<int32_t>(asmCode.get_code()) - reinterpret_cast<int32_t>(addr) - 5;

	VirtualProtect(addr, 5, old_protect, &old_protect);
}

bool d2_tweaks::client::modules::loot_filter_settings_menu::is_gold(diablo2::structures::unit* item) {
	static auto goldRecordIndex = -1;

	if (item == nullptr)
		return false;

	if (item->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM)
		return false;

	if (item->item_data == nullptr)
		return false;

	if (goldRecordIndex == -1) {
		auto record = diablo2::d2_common::get_item_record(item->data_record_index);

		if (record->string_code[0] == 'g' &&
			record->string_code[1] == 'l' &&
			record->string_code[2] == 'd') { //Gold pile
			goldRecordIndex = item->data_record_index;
		}
	}

	return item->data_record_index == goldRecordIndex;
}

bool d2_tweaks::client::modules::loot_filter_settings_menu::is_rune(diablo2::structures::unit* item) {
	static auto runeTypeId = -1;

	if (item == nullptr)
		return false;

	if (item->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM)
		return false;

	if (item->item_data == nullptr)
		return false;

	const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

	if (runeTypeId == -1) {
		const auto itemTypeRecord = diablo2::d2_common::get_item_type_record(record->type);

		const auto code = itemTypeRecord->code;

		if (code[0] == 'r' &&
			code[1] == 'u' &&
			code[2] == 'n' &&
			code[3] == 'e') {
			runeTypeId = record->type;
		}
	}

	return record->type == runeTypeId;
}

bool d2_tweaks::client::modules::loot_filter_settings_menu::is_gem(diablo2::structures::unit* item) {
	static auto gemTypeId = -1;

	if (item == nullptr)
		return false;

	if (item->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM)
		return false;

	if (item->item_data == nullptr)
		return false;

	const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

	if (gemTypeId == -1) {
		const auto itemTypeRecord = diablo2::d2_common::get_item_type_record(record->type);

		const auto code = itemTypeRecord->code;

		if (code[0] == 'g' &&
			code[1] == 'e' &&
			code[2] == 'm') {
			gemTypeId = record->type;
		}
	}

	return record->type == gemTypeId;
}

bool d2_tweaks::client::modules::loot_filter_settings_menu::check_alt_item(diablo2::structures::unit* unit) {
	static auto& instance = singleton<loot_filter_settings_menu>::instance();

	if (!unit || unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM) {
		return true;
	}

	if (unit->item_data == nullptr) {
		return true;
	}

	if (is_gold(unit))
		return instance.m_show_gold && instance.m_show_gold->get_state();

	if (is_rune(unit))
		return instance.m_show_runes && instance.m_show_runes->get_state();

	if (is_gem(unit))
		return instance.m_show_gems && instance.m_show_gems->get_state();

	return loot_filter_settings::get().quality_settings[static_cast<int32_t>(unit->item_data->quality)];
}

void d2_tweaks::client::modules::loot_filter_settings_menu::draw_dropped_items_names(diablo2::structures::unit* unit,
	void* edx) {
	static auto& instance = singleton<loot_filter_settings_menu>::instance();

	if (!unit || unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM) {
		instance.m_draw_dropped_items_names_original(unit, edx);
		return;
	}

	if (unit->item_data == nullptr) {
		instance.m_draw_dropped_items_names_original(unit, edx);
		return;
	}

	if (instance.m_altonly && instance.m_altonly->get_state())
		return;

	if (is_gold(unit) && instance.m_show_gold && !instance.m_show_gold->get_state())
		return;

	if (is_rune(unit) && instance.m_show_runes && !instance.m_show_runes->get_state())
		return;

	if (is_gem(unit) && instance.m_show_gems && !instance.m_show_gems->get_state())
		return;

	if (!loot_filter_settings::get().quality_settings[static_cast<int32_t>(unit->item_data->quality)])
		return;

	instance.m_draw_dropped_items_names_original(unit, edx);
}

void d2_tweaks::client::modules::loot_filter_settings_menu::handle_dropped_items(diablo2::structures::unit* unit,
	void* edx) {
	static auto& instance = singleton<loot_filter_settings_menu>::instance();

	if (!unit || unit->type != diablo2::structures::unit_type_t::UNIT_TYPE_ITEM) {
		instance.m_handle_dropped_items_original(unit, edx);
		return;
	}

	if (unit->item_data == nullptr) {
		instance.m_handle_dropped_items_original(unit, edx);
		return;
	}

	if (instance.m_altonly && instance.m_altonly->get_state())
		return;

	if (is_gold(unit) && instance.m_show_gold && !instance.m_show_gold->get_state())
		return;

	if (is_rune(unit) && instance.m_show_runes && !instance.m_show_runes->get_state())
		return;

	if (is_gem(unit) && instance.m_show_gems && !instance.m_show_gems->get_state())
		return;

	if (!loot_filter_settings::get().quality_settings[static_cast<int32_t>(unit->item_data->quality)])
		return;

	instance.m_handle_dropped_items_original(unit, edx);
}