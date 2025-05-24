#pragma once

#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/controls/button.h>
#include <fw/singleton.h>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

namespace d2_tweaks {
namespace client {
namespace modules {

class loot_filter_settings_toggle_menu final
    : public ui::menu,
      singleton<loot_filter_settings_toggle_menu> {
  button* m_toggle_filter_settings_btn;
  button* m_btn_toggle_stats;
  button* m_btn_toggle_help;
  button* m_btn_toggle_cube;
  button* m_btn_toggle_stash;
  button* m_btn_toggle_bag;

 public:
  menu* m_filter_settings_menu;
  menu* m_menu;
  bool m_show;
  bool m_show_bag;

  explicit loot_filter_settings_toggle_menu(token);

  void toggle_show() {
    m_show = !m_show;
    set_enabled(m_show);
    set_visible(m_show);
  }

  void toggle_filter_settings_click();
  void toggle_stats_settings_click();
  void toggle_help_click();
  void toggle_cube_click();
  void toggle_stash_click();
  void toggle_bag_click();
  void draw() override;

  bool key_event(uint32_t key, bool up) override;
};

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
