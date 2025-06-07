#include <diablo2/d2common.h>
#include <common/ptr_wrapper.h>
#include <d2tweaks/common/protocol.h>

namespace diablo2 {

char* d2_common::get_base() {
  static auto base = reinterpret_cast<char*>(GetModuleHandle("d2common.dll"));
  return base;
}

int8_t d2_common::get_item_page(Unit* item) {
  static wrap_func_std_import<int32_t(Unit*)> get_item_page(10719, get_base());
  return get_item_page(item);
}

uint32_t d2_common::get_max_cube_recipes() {
  static wrap_func_std_import<uint32_t()> get_max_cube_recipes(11233,
                                                               get_base());
  return get_max_cube_recipes();
}

int32_t d2_common::get_item_type_from_unit(Unit* item) {
  static wrap_func_std_import<int32_t(Unit*)> get_item_type_from_unit(
      10751, get_base());
  return get_item_type_from_unit(item);
}

//__stdcall UNITS_GetPreviousInteractGUID(D2UnitStrc* pUnit)
int32_t d2_common::get_previous_interact_guid(Unit* player) {
  static wrap_func_std_import<int32_t(Unit*)> get_previous_interact_guid(
      10383, get_base());
  return get_previous_interact_guid(player);
}

//__stdcall INV_EmptyInventory(D2InventoryStrc* pInventory)
// deletes items one by one
void d2_common::empty_inventory_1(inventory* inv) {
  static wrap_func_std_import<void(inventory*)> empty_inventory_1(10279,
                                                                  get_base());
  return empty_inventory_1(inv);
}

void d2_common::empty_inventory_2(inventory* inv) {
  static wrap_func_std_import<void(inventory*)> empty_inventory_2(10300,
                                                                  get_base());
  return empty_inventory_2(inv);
}

//__stdcall INV_FreeTradeInventory(D2InventoryStrc* pInventory)
void d2_common::free_trade_inventory(inventory* inv) {
  static wrap_func_std_import<void(inventory*)> free_trade_inventory(
      10281, get_base());
  return free_trade_inventory(inv);
}

Unit* d2_common::get_first_inventory_item(inventory* inv) {
  static wrap_func_std_import<Unit*(inventory*)> get_first_inventory_item(
      10277, get_base());
  return get_first_inventory_item(inv);
}

Unit* d2_common::get_last_inventory_item(inventory* inv) {
  static wrap_func_std_import<Unit*(inventory*)> get_last_inventory_item(
      10278, get_base());
  return get_last_inventory_item(inv);
}

Unit* d2_common::get_next_inventory_item(Unit* prev_item) {
  static wrap_func_std_import<Unit*(Unit*)> get_next_inventory_item(10304,
                                                                    get_base());
  return get_next_inventory_item(prev_item);
}

int32_t d2_common::get_inventory_index(Unit* item, char page, BOOL lod) {
  static wrap_func_std_import<int32_t(Unit*, char, BOOL)> get_inventory_index(
      10409, get_base());
  return get_inventory_index(item, page, lod);
}

void* d2_common::get_inventory_data(int32_t index, int32_t zero, char* data) {
  static wrap_func_std_import<void*(int32_t, int32_t, char*)>
      get_inventory_data(10636, get_base());
  return get_inventory_data(index, zero, data);
}

Unit* d2_common::get_item_at_cell(inventory* inv,
                                  uint32_t cellx,
                                  uint32_t celly,
                                  uint32_t* pcellx,
                                  uint32_t* pcelly,
                                  int32_t invIndex,
                                  uint8_t page) {
  static wrap_func_std_import<Unit*(
      inventory*, uint32_t, uint32_t, uint32_t*, uint32_t*, int32_t, uint8_t)>
      get_item_at_cell(10252, get_base());
  return get_item_at_cell(inv, cellx, celly, pcellx, pcelly, invIndex, page);
}

uint32_t d2_common::can_put_into_slot(inventory* inv,
                                      Unit* item,
                                      uint32_t x,
                                      uint32_t y,
                                      uint32_t invIndex,
                                      Unit** lastBlockingUnit,
                                      uint32_t* lastBlockingUnitIndex,
                                      uint8_t page) {
  static wrap_func_std_import<uint32_t(inventory*,
                                       Unit*,
                                       uint32_t,
                                       uint32_t,
                                       uint32_t,
                                       Unit**,
                                       uint32_t*,
                                       uint8_t)>
      can_put_into_slot(10247, get_base());
  return can_put_into_slot(
      inv, item, x, y, invIndex, lastBlockingUnit, lastBlockingUnitIndex, page);
}

uint32_t d2_common::get_item_type(Unit* item) {
  static wrap_func_std_import<uint32_t(Unit*)> get_item_type(10751, get_base());
  return get_item_type(item);
}

uint32_t d2_common::get_item_type_class(Unit* item) {
  static wrap_func_std_import<uint32_t(Unit*)> get_item_type_class(10739,
                                                                   get_base());
  return get_item_type_class(item);
}

uint32_t d2_common::get_item_primary_weapon_class(Unit* item) {
  static wrap_func_std_import<uint32_t(Unit*)> get_item_type_class(10744,
                                                                   get_base());
  return get_item_type_class(item);
}

uint32_t d2_common::set_unit_mode(Unit* item, uint32_t mode) {
  static wrap_func_std_import<uint32_t(Unit*, uint32_t mode)> set_unit_mode(
      10348, get_base());
  return set_unit_mode(item, mode);
}

Unit* d2_common::inv_remove_item(inventory* inv, Unit* item) {
  static wrap_func_std_import<Unit*(inventory*, Unit*)> inv_remove_item(
      10243, get_base());
  return inv_remove_item(inv, item);
}

BOOL d2_common::inv_add_item(inventory* inv,
                             Unit* item,
                             uint32_t x,
                             uint32_t y,
                             uint32_t invIndex,
                             BOOL isClient,
                             uint8_t page) {
  static wrap_func_std_import<BOOL(
      inventory*, Unit*, uint32_t, uint32_t, uint32_t, BOOL, uint8_t)>
      inv_add_item(10249, get_base());

  return inv_add_item(inv, item, x, y, invIndex, isClient, page);
}

BOOL d2_common::inv_update_item(inventory* inv, Unit* item, BOOL isClient) {
  static wrap_func_std_import<BOOL(inventory*, Unit*, BOOL)> inv_update_item(
      10242, get_base());
  return inv_update_item(inv, item, isClient);
}

items_line* d2_common::get_item_record(uint32_t guid) {
  static wrap_func_std_import<items_line*(uint32_t)> get_item_record(
      10600, get_base());
  return get_item_record(guid);
}

item_types_line* d2_common::get_item_type_record(uint32_t typeId) {
  static wrap_func_fast<item_types_line*(uint32_t)> get_item_type_record(
      0x2B1A0, get_base());
  return get_item_type_record(typeId);
}

uint32_t d2_common::check_item_type_equiv(uint32_t itemtype,
                                          uint32_t itemtype_equiv) {
  static wrap_func_std_import<uint32_t(uint32_t, uint32_t)>
      check_item_type_equiv(10730, get_base());
  return check_item_type_equiv(itemtype, itemtype_equiv);
}

uint32_t d2_common::get_item_unique_index(Unit* item) {
  static wrap_func_std_import<uint32_t(Unit*)> get_item_unique_index(
      10732, get_base());
  return get_item_unique_index(item);
}

uint32_t d2_common::get_maximum_character_gold(Unit* player) {
  static wrap_func_std_import<uint32_t(Unit*)> get_maximum_character_gold(
      10439, get_base());
  return get_maximum_character_gold(player);
}

int32_t d2_common::set_stat(Unit* pUnit,
                            unit_stats_t stat,
                            uint32_t value,
                            int16_t param) {
  static wrap_func_std_import<int32_t(Unit*, int32_t, int32_t, int32_t)>
      set_stat(10517, get_base());
  return set_stat(pUnit, stat, value, param);
}

int32_t d2_common::get_stat(Unit* pUnit, unit_stats_t stat, int16_t param) {
  static wrap_func_std_import<int32_t(Unit*, int32_t, int32_t)> get_stat(
      10519, get_base());
  return get_stat(pUnit, stat, param);
}

int32_t d2_common::get_stat_signed(Unit* pUnit,
                                   unit_stats_t stat,
                                   int16_t param) {
  static wrap_func_std_import<int32_t(Unit*, int32_t, int32_t)> get_stat_signed(
      10520, get_base());
  return get_stat_signed(pUnit, stat, param);
}

int32_t d2_common::_10111(int32_t* x, int32_t* y) {
  static wrap_func_std_import<int32_t(int32_t*, int32_t*)> get_unk_coords(
      10111, get_base());
  return get_unk_coords(x, y);
}

int32_t d2_common::_10116(int32_t x1, int32_t y1, int32_t* x, int32_t* y) {
  static wrap_func_std_import<int32_t(int32_t, int32_t, int32_t*, int32_t*)>
      get_unk_coords2(10116, get_base());
  return get_unk_coords2(x1, y1, x, y);
}

room* d2_common::get_room_from_unit(Unit* pUnit) {
  static wrap_func_std_import<room*(Unit*)> get_room_from_unit(10342,
                                                               get_base());
  return get_room_from_unit(pUnit);
}

int32_t d2_common::get_unit_size_x(Unit* pUnit) {
  static wrap_func_std_import<int32_t(Unit*)> get_unit_size_x(10336,
                                                              get_base());
  return get_unit_size_x(pUnit);
}

int32_t d2_common::get_unit_size_y(Unit* pUnit) {
  static wrap_func_std_import<int32_t(Unit*)> get_unit_size_y(10337,
                                                              get_base());
  return get_unit_size_y(pUnit);
}

int32_t d2_common::get_distance_between_units(Unit* unit1, Unit* unit2) {
  static wrap_func_std_import<int32_t(Unit*, Unit*)> get_distance_between_units(
      10397, get_base());
  return get_distance_between_units(unit1, unit2);
}

int32_t d2_common::get_unit_x(path* pPath) {
  static wrap_func_std_import<int32_t(path*)> get_unit_x(10162, get_base());
  return get_unit_x(pPath);
}

int32_t d2_common::get_unit_y(path* pPath) {
  static wrap_func_std_import<int32_t(path*)> get_unit_y(10163, get_base());
  return get_unit_y(pPath);
}

int32_t d2_common::get_unit_precise_x(Unit* pUnit) {
  static wrap_func_std_import<int32_t(Unit*)> get_unit_precise_x(10327,
                                                                 get_base());
  return get_unit_precise_x(pUnit);
}

int32_t d2_common::get_unit_precise_y(Unit* pUnit) {
  static wrap_func_std_import<int32_t(Unit*)> get_unit_precise_y(10330,
                                                                 get_base());
  return get_unit_precise_y(pUnit);
}

int32_t d2_common::get_item_quality(Unit* item) {
  static wrap_func_std_import<int32_t(Unit*)> get_item_quality(10695,
                                                               get_base());
  return get_item_quality(item);
}

Unit* d2_common::get_target_from_path(path* pPath) {
  static wrap_func_std_import<Unit*(path*)> get_target_from_path(10180,
                                                                 get_base());
  return get_target_from_path(pPath);
}

void d2_common::free_inventory(inventory* inv) {
  static wrap_func_std_import<void(inventory*)> free_inventory(10241,
                                                               get_base());
  free_inventory(inv);
}

void d2_common::refresh_unit_inventory(Unit* pUnit, bool set_update_flags) {
  static wrap_func_std_import<void(Unit * Unit, bool)>
      refresh_unit_inventory(10357, get_base());
  refresh_unit_inventory(pUnit, set_update_flags);
}

void d2_common::update_trade(inventory* inv, Unit* item) {
  static wrap_func_std_import<void(inventory*, Unit*)> update_trade(10283,
                                                                    get_base());
  update_trade(inv, item);
}

// void d2_common::set_item_flags(Unit* item, itemflags_t dwFlag, bool bSet) {
//   static wrap_func_std_import<void(Unit*, itemflags_t, bool)> set_item_flags(
//       10708, get_base());
//   set_item_flags(item, dwFlag, bSet);
// }

void d2_common::add_property(Unit* item, D2PropertyStrc* prop, int nUnused) {
  static wrap_func_std_import<void(Unit*, D2PropertyStrc*, int)> add_property(
      10868, get_base());
  add_property(item, prop, nUnused);
}

void d2_common::ITEMS_SetItemFlag(Unit* item, uint32_t dwFlag, BOOL bSet) {
  static wrap_func_std_import<void(Unit*, uint32_t, BOOL)> ITEMS_SetItemFlag(
      10708, get_base());
  ITEMS_SetItemFlag(item, dwFlag, bSet);
}

D2ItemStatCostTxt* d2_common::get_item_stat_cost_record(int statId) {
  static wrap_func_fast<D2ItemStatCostTxt*(int)> get_item_stat_cost_record(
      0x642b0, get_base());
  return get_item_stat_cost_record(statId);
}

// D2Common.0x6FD57720 (#10602)
// int __stdcall DATATBLS_GetItemIdFromItemCode(uint32_t dwCode)
// static int get_item_id_from_item_code(uint32_t dwCode);

int32_t d2_common::get_item_id_from_item_code(uint32_t dwCode) {
  static wrap_func_std_import<int32_t(uint32_t)> get_item_id_from_item_code(
      10602, get_base());
  return get_item_id_from_item_code(dwCode);
}

// D2Common.0x6FD576D0 (#10601)
// D2ItemsTxt* __stdcall DATATBLS_GetItemRecordFromItemCode(uint32_t dwCode,
//                                                          int* pItemId);
items_line* d2_common::get_item_record_from_item_code(uint32_t dwCode,
                                                      int* pItemId) {
  static wrap_func_std_import<items_line*(uint32_t, int*)>
      get_item_record_from_item_code(10601, get_base());
  return get_item_record_from_item_code(dwCode, pItemId);
}

// Add a wrapper function for the following:
// D2Common.0x6FD5EAA0 (#10620)
// D2COMMON_DLL_DECL D2RunesTxt* __stdcall DATATBLS_GetRunesTxtRecord(int nRunewordId);
D2RunesTxt* d2_common::get_runes_txt_record(int nRunewordId) {
  static wrap_func_std_import<D2RunesTxt*(int)> get_runes_txt_record(
      10620, get_base());
  return get_runes_txt_record(nRunewordId);
}

//D2Common.0x6FD57680 (#10600)
// D2ItemsTxt* __stdcall DATATBLS_GetItemsTxtRecord(int nItemId)
items_line* d2_common::get_items_txt_record(int nItemId) {
  static wrap_func_std_import<items_line*(int)> get_items_txt_record(
      10600, get_base());
  return get_items_txt_record(nItemId);
}

}  // namespace diablo2
