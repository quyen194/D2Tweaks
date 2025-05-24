#include <d2tweaks/server/modules/test/test.h>

#include <common/hooking.h>
#include <diablo2/d2common.h>
#include <spdlog/spdlog.h>
#include <diablo2/structures/unit.h>
#include <diablo2/d2game.h>
#include <diablo2/structures/game.h>

using namespace d2_tweaks;
using namespace diablo2;
using namespace structures;

namespace d2_tweaks {
namespace server {
namespace modules {

MODULE_INIT(test)

static int(__stdcall* g_set_stat_original)(unit* unit,
                                           unit_stats_t stat,
                                           uint32_t value,
                                           int16_t param);
static int __stdcall set_stat(unit* unit,
                              unit_stats_t stat,
                              uint32_t value,
                              int16_t param) {
  if (unit->type == unit_type_t::UNIT_TYPE_PLAYER && stat == UNIT_STAT_GOLD) {
    spdlog::debug("Setting UNIT_STAT_GOLD stat!");
  }

  return g_set_stat_original(unit, stat, value, param);
}

static int32_t(__stdcall* g_set_stat_in_list_original)(void*,
                                                       unit_stats_t stat,
                                                       uint32_t value,
                                                       uint32_t param);
static int32_t __stdcall set_stat_in_list(void* a1,
                                          unit_stats_t stat,
                                          uint32_t value,
                                          uint32_t param) {
  if (stat == UNIT_STAT_GOLD) {
    spdlog::debug("Setting UNIT_STAT_GOLD stat!");
  }

  return g_set_stat_in_list_original(a1, stat, value, param);
}

static int(__fastcall* g_regen_tick_original)(game*, unit*, int32_t, int32_t);
static int __fastcall regen_tick(game* game,
                                 unit* unit,
                                 int32_t a3,
                                 int32_t a4) {
  return g_regen_tick_original(game, unit, a3, a4);
}

void test::init() {
  return;

  hooking::hook<10463>(
      d2_common::get_base(), set_stat_in_list, &g_set_stat_in_list_original);
  hooking::hook<10517>(d2_common::get_base(), set_stat, &g_set_stat_original);

  hooking::hook(
      d2_game::get_base() + 0x50F80, regen_tick, &g_regen_tick_original);
}

}  // namespace modules
}  // namespace server
}  // namespace d2_tweaks
