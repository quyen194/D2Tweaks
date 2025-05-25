#include <common/hooking.h>
#include <d2tweaks/client/modules/loot_filter/loot_filter.h>
#include <d2tweaks/client/modules/loot_filter/loot_filter_settings.h>
#include <d2tweaks/client/modules/loot_filter/loot_filter_settings_menu.h>
#include <d2tweaks/client/modules/loot_filter/loot_filter_settings_toggle_menu.h>
#include <d2tweaks/common/asset_manager.h>
#include <d2tweaks/ui/ui_manager.h>
#include <diablo2/d2client.h>
#include <diablo2/d2launch.h>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

namespace d2_tweaks {
namespace client {
namespace modules {

MODULE_INIT(loot_filter)

static char*(__fastcall* g_set_player_name_original)(void*, void*);
static char* __fastcall set_player_name(void* player, void* edx) {
  const auto result = g_set_player_name_original(player, edx);

  loot_filter_settings::load(d2_client::get_local_player_name());
  singleton<loot_filter_settings_menu>::instance().reload_settings();

  return result;
}

static HANDLE(__fastcall* g_delete_save_file_original)(char*, char*);
static HANDLE __fastcall delete_save_file(char* name, char* a2) {
  loot_filter_settings::remove(name);
  return g_delete_save_file_original(name, a2);
}

void loot_filter::init_early() {
  const char* config_path = common::get_config_path();

  if (GetPrivateProfileInt("modules", "LootFilter", 1, config_path)) {
    hooking::hook(d2_client::get_base() + 0xBDE0,
                  set_player_name,
                  &g_set_player_name_original);
    hooking::hook(d2_launch::get_base() + 0x17C00,
                  delete_save_file,
                  &g_delete_save_file_original);
  }
}

void loot_filter::init() {
  char acPathToIni[MAX_PATH] = { 0 };
  const char* pcIniFile = "\\d2tweaks.ini";

  GetCurrentDirectory(MAX_PATH, acPathToIni);
  lstrcat(acPathToIni, pcIniFile);

  if (GetPrivateProfileInt("modules", "LootFilter", 1, acPathToIni) != FALSE) {
    singleton<ui::ui_manager>::instance().add_menu(&singleton<loot_filter_settings_menu>::instance());
    singleton<ui::ui_manager>::instance().add_menu(&singleton<loot_filter_settings_toggle_menu>::instance());
  }
}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
