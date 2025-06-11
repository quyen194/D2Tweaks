
#include <Windows.h>

#include <D2Template.h>
#include <DllNotify.h>

#include "common/file_ini.h"

#include "diablo2/structures/data/item_types_line.h"
#include "diablo2/structures/data/items_line.h"
#include "diablo2/structures/inventory.h"
#include "diablo2/structures/item_data.h"
#include "diablo2/structures/player_data.h"
#include "diablo2/structures/room.h"
#include "diablo2/structures/unit.h"

#include "diablo2/d2client.h"
#include "diablo2/d2common.h"
#include "diablo2/d2gfx.h"
#include "diablo2/d2win.h"

#include "d2tweaks/common/asset_manager.h"
#include "d2tweaks/common/protocol.h"

#include "d2tweaks/ui/controls/label.h"
#include "d2tweaks/ui/menu.h"
#include "d2tweaks/ui/ui_manager.h"

#include "d2tweaks/client/client.h"
#include "d2tweaks/client/auto_gold_pickup/auto_gold_pickup_client.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

static uint32_t m_nDisplayTime = 2500;
static uint32_t m_nLastUpdate = 0;
static uint32_t m_nGoldValue = 0;
static int32_t m_iDistance = 4;

namespace d2_tweaks {
namespace client {

class DrawGoldMenu final : public ui::menu {
 public:
  label* m_label;

  DrawGoldMenu() {
    // load_xml("d2tweaks\\interfaces\\autogoldpickup.xml");
    if (DLLBASE_D2EXPRES != 0)
      load_xml("d2tweaks\\interface_d2expres\\autogoldpickup.xml");
    if (DLLBASE_SGD2FREERES != 0)
      load_xml("d2tweaks\\interface_sgd2freeres\\autogoldpickup.xml");
    if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
      load_xml("d2tweaks\\interface_vanilla\\autogoldpickup.xml");

    m_label = get_control<label>("m_gold_label");

    menu::set_visible(true);
    menu::set_enabled(true);
  }

  void draw() override {
    static wchar_t buffer[1024];

    if (m_nGoldValue == 0) {
      return;
    }

    if (GetTickCount() - m_nLastUpdate >= m_nDisplayTime) {
      m_nGoldValue = 0;
      return;
    }

    swprintf_s(buffer, L"+%i", m_nGoldValue);

    m_label->set_text(buffer);
    m_label->draw();
  }
};

MODULE_INIT(AutoGoldPickup)

void AutoGoldPickup::init_early() {}

void AutoGoldPickup::init() {
  FileIni config(common::get_config_path());

  if (config.Int("modules", "AutoGoldPickup", 1)) {
    m_iDistance = config.Int("AutoGoldPickup", "PickupDistance", 4);
    m_nDisplayTime = config.Int("AutoGoldPickup", "DisplayTime", 2500);
    Client::instance().register_packet_handler(
        common::message_types_t::MESSAGE_TYPE_GOLD_PICKUP_INFO, this);
    Client::instance().register_tick_handler(this);
    ui::Manager::instance().add_menu(new DrawGoldMenu());
  }
}

void AutoGoldPickup::tick() {
  const auto unit = d2_client::get_local_player();

  if (!unit)
    return;

  if (unit->type != unit_type_t::UNIT_TYPE_PLAYER)
    return;

  const auto room = d2_common::get_room_from_unit(unit);

  if (!room)
    return;

  for (auto item = room->unit; item; item = item->prev_unit_in_room) {
    if (!item)
      continue;

    if (item->type != unit_type_t::UNIT_TYPE_ITEM)
      continue;

    auto record = d2_common::get_item_record(item->data_record_index);

    if (!record)
      continue;

    const auto distance = d2_common::get_distance_between_units(unit, item);
    if (distance > m_iDistance)
      continue;

    if (record->string_code[0] == 'g' &&
        record->string_code[1] == 'l' &&
        record->string_code[2] == 'd') {
      static common::gold_pickup_info_cs request_packet_cs;
      request_packet_cs.item_guid = item->guid;
      d2_client::send_to_server(&request_packet_cs, sizeof request_packet_cs);
    }

    continue;
  }
}

void AutoGoldPickup::handle_packet(common::packet_header* packet) {
  const auto info = static_cast<common::gold_pickup_info_sc*>(packet);
  m_nLastUpdate = GetTickCount();
  m_nGoldValue += info->gold;
}

}  // namespace client
}  // namespace d2_tweaks
