#include <diablo2/d2client.h>

#include <common/ptr_wrapper.h>

namespace diablo2 {

char* d2_client::get_base() {
  static auto base = reinterpret_cast<char*>(GetModuleHandle("d2client.dll"));
  return base;
}

void d2_client::print_chat(wchar_t* string, uint32_t color) {
  static wrap_func_fast<void(wchar_t*, uint32_t)> print_chat(0x7C600,
                                                             get_base());
  print_chat(string, color);
}

bool d2_client::is_lod() {
  return *reinterpret_cast<int*>(get_base() + 0x1077C4) > 0;
}

Unit* d2_client::get_local_player() {
  static wrap_func_std<Unit* ()> get_local_player(0x883D0, get_base());
  return get_local_player();
}

const char* d2_client::get_local_player_name() {
  static wrap_value<char> get_local_player_name(0x107810, get_base());
  return get_local_player_name;
}

client_unit_list* d2_client::get_client_unit_list() {
  static auto unit_list =
      reinterpret_cast<client_unit_list*>(get_base() + 0x11AA00);
  return unit_list;
}

void d2_client::set_ui_toggle(int nToggle, int nUIState, BOOL bToggle) {
  static wrap_func_fast<void(int, int, BOOL)> set_ui_toggle(0x83260,
                                                            get_base());
  set_ui_toggle(nToggle, nUIState, bToggle);
}

int32_t d2_client::get_view_offset_x() {
  static wrap_func_std<int32_t()> get_view_offset_x(0x15890, get_base());
  return get_view_offset_x();
}

int32_t d2_client::get_view_offset_y() {
  static wrap_func_std<int32_t()> get_view_offset_y(0x158A0, get_base());
  return get_view_offset_y();
}

uint32_t d2_client::get_mouse_x() {
  static wrap_func_std<uint32_t()> get_mouse_x(0xB7BC0, get_base());
  return get_mouse_x();
}

uint32_t d2_client::get_mouse_y() {
  static wrap_func_std<uint32_t()> get_mouse_y(0xB7BD0, get_base());
  return get_mouse_y();
}

bool d2_client::get_ui_window_state(const ui_window_t window) {
  static auto ui_states = reinterpret_cast<BOOL*>(get_base() + 0x11A6A8);
  return ui_states[window];
}

void* d2_client::get_buysellbtn() {
  static wrap_func_cdecl<void* ()> get_buysellbtn(0x84110, get_base());
  return get_buysellbtn();
}

void d2_client::resync_vendor_inventory(Unit* ptNPC) {
  static wrap_func_fast<void(Unit * ptNPC)> resync_vendor_inventory(0x578E0,
                                                                    get_base());
  resync_vendor_inventory(ptNPC);
}

void d2_client::play_sound(const uint32_t soundId,
                           Unit* pUnit,
                           const uint32_t ticks,
                           const BOOL prePick,
                           const uint32_t cache) {
  static wrap_func_fast<void(uint32_t, Unit*, uint32_t, BOOL, uint32_t)>
      play_sound(0xB5820, get_base());
  play_sound(soundId, pUnit, ticks, prePick, cache);
}

Unit* d2_client::get_unit_by_guid(const int32_t guid, const int32_t type) {
  static wrap_func_fast<Unit*(int32_t, int32_t)> get_unit_by_guid(0x869F0,
                                                                  get_base());
  return get_unit_by_guid(guid, type);
}

void d2_client::send_to_server(void* data, const size_t size) {
  static wrap_func_fast<void(void*, size_t)> send_to_server(0xD850, get_base());
  send_to_server(data, size);
}

bool d2_client::cache_gfx_data(GfxData* pGfxdata, Unit* pUnit,
                               cellfile* cellfFile,
                               int32_t direction,
                               int32_t frame,
                               int32_t* outIndex,
                               int8_t flags,
                               int32_t colorTint) {
  static wrap_func_fast<int32_t(
      GfxData*, Unit*, cellfile*, int32_t, int32_t, int32_t*, int8_t, int32_t)>
      cache_gfx_data(0xBEC80, get_base());
  return cache_gfx_data(
      pGfxdata, pUnit, cellfFile, direction, frame, outIndex, flags, colorTint);
}

cellfile* d2_client::load_gfx_resource(char* path) {
  static wrap_func_fast<cellfile*(char*, int32_t)> load_gfx_resource(
      0x1000, get_base());
  return load_gfx_resource(path, 0);
}

int32_t d2_client::unload_gfx_resource(cellfile* handle) {
  static wrap_func_fast<int32_t(cellfile*)> unload_gfx_resource(0x1140,
                                                                get_base());
  return unload_gfx_resource(handle);
}

int32_t d2_client::send_to_server_7(BYTE type,
                                    DWORD num,
                                    DWORD unk1,
                                    DWORD unk2) {
  static wrap_func_fast<int32_t(BYTE type, DWORD num, DWORD unk1, DWORD unk2)>
      send_to_server_7(0xD9E0, get_base());
  return send_to_server_7(type, num, unk1, unk2);
}

//D2FUNC(D2CLIENT, SendToServer9, void, __fastcall, (BYTE type, DWORD num, DWORD unk1), 0xDA40) // Interact

int32_t d2_client::send_to_server_9(BYTE type, DWORD num, DWORD unk1) {
  static wrap_func_fast<int32_t(BYTE type, DWORD num, DWORD unk1)>
      send_to_server_9(0xDA40, get_base());
  return send_to_server_9(type, num, unk1);
}

uint32_t d2_client::screen_height() {
  return *reinterpret_cast<uint32_t*>(get_base() + 0xD40F0);
}

uint32_t d2_client::screen_width() {
  return *reinterpret_cast<uint32_t*>(get_base() + 0xD40F4);
}

uint32_t d2_client::current_vendor_id() {
  return *reinterpret_cast<uint32_t*>(get_base() + 0x115CFD);
}

uint32_t d2_client::current_vendor_guid() {
  return *reinterpret_cast<uint32_t*>(get_base() + 0x115CF5);
}

bool d2_client::is_gamble_open() {
  return *reinterpret_cast<bool*>(get_base() + 0x115D7C);
}

// 1 = talk
// 3 = trade or gamble
// 7 = imbue / add sockets / personalize
uint8_t d2_client::current_interact_menu() {
  return *reinterpret_cast<uint8_t*>(get_base() + 0x115C3B);
}

}  // namespace diablo2
