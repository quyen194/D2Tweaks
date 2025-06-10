
#include <Windows.h>

#include <DllNotify.h>
#include <D2Template.h>

#include <spdlog/spdlog.h>

#include "common/autopickup_lootfilter.h"
#include "common/file_ini.h"
#include "common/hooking.h"

#include "diablo2/structures/data/item_types_line.h"
#include "diablo2/structures/data/items_line.h"
#include "diablo2/structures/inventory.h"
#include "diablo2/structures/item_data.h"
#include "diablo2/structures/npc_record.h"
#include "diablo2/structures/unit.h"

#include "diablo2/d2client.h"
#include "diablo2/d2common.h"
#include "diablo2/d2common.h"
#include "diablo2/d2gfx.h"
#include "diablo2/d2lang.h"
#include "diablo2/d2win.h"

#include "d2tweaks/common/asset_manager.h"
#include "d2tweaks/common/common.h"
#include "d2tweaks/common/protocol.h"

#include "d2tweaks/ui/controls/button.h"
#include "d2tweaks/ui/menu.h"
#include "d2tweaks/ui/ui_manager.h"

#include "d2tweaks/client/client.h"
#include "d2tweaks/client/modules/trader_update/trader_update_client.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

namespace d2_tweaks {
namespace client {
namespace modules {

MODULE_INIT(trader_update)

static uint8_t m_nMenuOpen;
static uint8_t m_nNpcId;

enum trader_command {
  COMMAND_NULL,
  COMMAND_FREE_NPC_INVENTORY,
  COMMAND_FREE_NPC_GAMBLE,
  COMMAND_FILL_NPC_INVENTORY,
  COMMAND_FILL_NPC_GAMBLE,
  COMMAND_FINISHED,
  COMMAND_ABORT
};

class trader_update_menu : public ui::menu {
  common::asset* m_buttons_img;
  button* m_trader_update_btn;
  button* m_gamble_update_btn;

 public:
  trader_update_menu() {
    menu::set_enabled(true);
    menu::set_visible(true);

    //load_xml("d2tweaks\\interface_d2expres\\reloaditems.xml");
    if (DLLBASE_D2EXPRES != 0)
      load_xml("d2tweaks\\interface_d2expres\\reloaditems.xml");
    if (DLLBASE_SGD2FREERES != 0)
      load_xml("d2tweaks\\interface_sgd2freeres\\reloaditems.xml");
    if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
      load_xml("d2tweaks\\interface_vanilla\\reloaditems.xml");

    m_buttons_img = common::asset_manager::instance().get_mpq_file(
        "d2tweaks\\assets\\buttons", common::MPQ_FILE_TYPE_DC6);
    m_trader_update_btn =
        get_button("m_reload_items_btn",
                   std::bind(&trader_update_menu::trader_update_click, this));
  }

  void draw() override {
    if (!should_draw()) {
      m_trader_update_btn->set_enabled(false);
      m_trader_update_btn->set_visible(false);
      return;
    }

    m_trader_update_btn->set_enabled(true);
    m_trader_update_btn->set_visible(true);

    menu::draw();
  }

 private:
  static bool should_draw() {
    return d2_client::get_ui_window_state(UI_WINDOW_NPCSHOP);
  }

  button* get_button(const std::string& name,
                     const std::function<void()>& onClick) {
    auto result = static_cast<button*>(get_control(name));
    result->set_on_click(onClick);
    result->set_enabled(false);
    result->set_visible(false);
    return result;
  }

  void trader_update_click() {
    const auto unit = d2_client::get_local_player();

    static common::trader_update_cs request_packet_cs;
    request_packet_cs.client_id = unit->guid;
    request_packet_cs.npc_id = d2_client::current_vendor_guid();
    request_packet_cs.is_gamble_menu_open = d2_client::is_gamble_open();

    if (request_packet_cs.is_gamble_menu_open == false) {
      request_packet_cs.command = COMMAND_FREE_NPC_INVENTORY;
      d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
    }

    if (request_packet_cs.is_gamble_menu_open == true) {
      request_packet_cs.command = COMMAND_FREE_NPC_GAMBLE;
      d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
    }
  }
};

void trader_update::init_early() {
}

void trader_update::init() {
  FileIni ini(common::get_config_path());

  if (ini.Int("modules", "ReloadTradeGamble", 1)) {
    ui::ui_manager::instance().add_menu(new trader_update_menu());
    client::instance().register_packet_handler(
        common::message_types_t::MESSAGE_TYPE_TRADER_UPDATE, this);
    // client::instance().register_tick_handler(this);
    // client::instance().register_packet_cs_handler(
    //     common::packet_types_cs_t::PACKET_0x38,
    //     common::message_types_t::MESSAGE_TYPE_TRADER_UPDATE,
    //     this);
  }
}

void trader_update::tick() {
  // const auto unit = d2_client::get_local_player();
}

void trader_update::handle_cs_packet(common::packet_header* packet) {
  static auto& instance = client::instance();
  const auto income_packet_cs = static_cast<common::d2_entity_action_cs*>(packet);

#ifndef NDEBUG
  spdlog::debug(
      "[D2CLIENT C > S] ENTITY ACTION! message {} action {} entity_id {} complement {}",
      (uint8_t) income_packet_cs->message_type,
      (void*) (income_packet_cs->action >> 24),
      (void*) (income_packet_cs->entity_id >> 24),
      (void*) income_packet_cs->complement);
#endif
  m_nMenuOpen = (uint8_t)income_packet_cs->message_type; // 1 = trade, 2 = gamble
  m_nNpcId = (income_packet_cs->action >> 24);
}

void trader_update::handle_packet(common::packet_header* packet) {
  static auto& instance = client::instance();
  const auto income_packet_sc = static_cast<common::trader_update_sc*>(packet);

  if (!d2_client::get_ui_window_state(UI_WINDOW_NPCSHOP))
    return;

  m_nNpcId = d2_client::current_vendor_guid();

  Unit* ptNPC = d2_client::get_unit_by_guid(
      m_nNpcId, (int32_t) unit_type_t::UNIT_TYPE_MONSTER);

  if (ptNPC == 0)
    return;

  if (income_packet_sc->command == COMMAND_FREE_NPC_INVENTORY) {
    // if the packet came from our merchant and the trade menu is open
    if (income_packet_sc->npc_id == m_nNpcId &&
        income_packet_sc->is_gamble_menu_open == d2_client::is_gamble_open()) {
      d2_client::resync_vendor_inventory(ptNPC);
      // d2_common::empty_inventory_2(ptNPC->inventory);

      static common::trader_update_cs request_packet_cs;
      request_packet_cs.command = COMMAND_FILL_NPC_INVENTORY;
      request_packet_cs.npc_id = m_nNpcId;
      request_packet_cs.is_gamble_menu_open = d2_client::is_gamble_open();
      request_packet_cs.client_id = income_packet_sc->client_id;
      d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
    }
  }

  if (income_packet_sc->command == COMMAND_FREE_NPC_GAMBLE) {
    // if the packet is from our merchant and the trade menu is open
    if (income_packet_sc->npc_id == m_nNpcId &&
        income_packet_sc->is_gamble_menu_open == d2_client::is_gamble_open()) {
      d2_client::resync_vendor_inventory(ptNPC);
      // d2_common::empty_inventory_2(ptNPC->inventory);

      static common::trader_update_cs request_packet_cs;
      request_packet_cs.command = COMMAND_FILL_NPC_GAMBLE;
      request_packet_cs.npc_id = m_nNpcId;
      request_packet_cs.is_gamble_menu_open = d2_client::is_gamble_open();
      request_packet_cs.client_id = income_packet_sc->client_id;
      d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
    }
  }
}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
