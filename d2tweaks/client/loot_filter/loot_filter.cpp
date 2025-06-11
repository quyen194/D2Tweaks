
#include <Windows.h>

#include "common/file_ini.h"
#include "common/hooking.h"

#include "diablo2/d2client.h"
#include "diablo2/d2launch.h"

#include "d2tweaks/common/asset_manager.h"

#include "d2tweaks/ui/ui_manager.h"

#include "d2tweaks/client/loot_filter/loot_filter_settings_toggle_menu.h"
#include "d2tweaks/client/loot_filter/loot_filter_settings_menu.h"
#include "d2tweaks/client/loot_filter/loot_filter_settings.h"
#include "d2tweaks/client/loot_filter/loot_filter.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

namespace d2_tweaks {
namespace client {

MODULE_INIT(LootFilter)

static char*(__fastcall* g_set_player_name_original)(void*, void*);
static char* __fastcall set_player_name(void* player, void* edx) {
  const auto result = g_set_player_name_original(player, edx);

  LootFilterSettings::load(d2_client::get_local_player_name());
  LootFilterSettingsMenu::instance().reload_settings();

  return result;
}

static HANDLE(__fastcall* g_delete_save_file_original)(char*, char*);
static HANDLE __fastcall delete_save_file(char* name, char* a2) {
  LootFilterSettings::remove(name);
  return g_delete_save_file_original(name, a2);
}

void LootFilter::init_early() {
  FileIni config(common::get_config_path());

  if (config.Int("modules", "LootFilter", 1)) {
    detour::hook(d2_client::get_base() + 0xBDE0,
                 set_player_name,
                 &g_set_player_name_original);
    detour::hook(d2_launch::get_base() + 0x17C00,
                 delete_save_file,
                 &g_delete_save_file_original);
  }
}

void LootFilter::init() {
  FileIni config(common::get_config_path());

  if (config.Int("modules", "LootFilter", 1)) {
    ui::Manager::instance().add_menu(&LootFilterSettingsMenu::instance());
    ui::Manager::instance().add_menu(&LootFilterSettingsToggleMenu::instance());
  }
}

}  // namespace client
}  // namespace d2_tweaks
