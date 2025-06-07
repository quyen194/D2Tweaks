#include <d2tweaks/client/modules/test/test.h>

#include <spdlog/spdlog.h>

#include <common/ptr_wrapper.h>

#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>

#include <diablo2/structures/data_tables.h>
#include <diablo2/structures/unit.h>
#include <diablo2/d2common.h>
#include <diablo2/d2client.h>
#include <diablo2/d2win.h>

#include <diablo2/structures/data/bodylocs_line.h>
#include <diablo2/structures/data/elemtypes_line.h>
#include <diablo2/structures/data/hitclass_line.h>
#include <diablo2/structures/data/item_types_line.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/monmode_line.h>
#include <diablo2/structures/data/playerclass_line.h>
#include <diablo2/structures/data/storepage_line.h>

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;
using namespace ui::controls;

namespace d2_tweaks {
namespace client {
namespace modules {

MODULE_INIT(test)

class test_menu : public ui::menu {
 public:
  test_menu() {
    menu::set_enabled(true);
    menu::set_visible(true);
  }

  void draw() override {
    static wrap_value<data_tables*> dataTables(0x96A20, d2_common::get_base());
    const auto player = d2_client::get_local_player();

    if (!player)
      return;

    auto v = *dataTables;

    draw_cursor_pos();

    menu::draw();
  }

 private:
  static void draw_cursor_pos() {
    static wchar_t buffer[512]{ 0 };

    const auto mx = d2_client::get_mouse_x();
    const auto my = d2_client::get_mouse_y();

    swprintf_s(buffer, L"%i, %i", mx, my);
    d2_win::draw_text(buffer, mx, my, UI_COLOR_WHITE, 0);
  }
};

static test_menu* g_test_menu;

void test::init_early() {
}

void test::init() {
#ifndef NDEBUG
  g_test_menu = new test_menu();
  singleton<ui::ui_manager>::instance().add_menu(g_test_menu);
#endif
}

void test::handle_packet(common::packet_header* packet) {}

}  // namespace modules
}  // namespace client
}  // namespace d2_tweaks
