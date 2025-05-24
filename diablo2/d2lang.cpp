#include <diablo2/d2lang.h>
#include <diablo2/structures/unit.h>
#include <common/ptr_wrapper.h>

namespace diablo2 {

char* d2_lang::get_base() {
  static auto base = reinterpret_cast<char*>(GetModuleHandle("d2lang.dll"));
  return base;
}

wchar_t* d2_lang::get_string_from_index(short tbl_index_name_str) {
  static wrap_func_fast<wchar_t* (short)>get_string_from_index(0x3740, get_base());
  return get_string_from_index(tbl_index_name_str);
}

//D2FUNC(D2LANG, GetStringFromIndex, wchar_t*, __fastcall, (short nTblIndex_name_str), -10004)

//void d2_game::enqueue_packet(structures::net_client* client, void* packet, size_t size) {
//  static wrap_func_fast<void(structures::net_client*, void*, size_t)> enqueue_packet(0xC710, get_base());
//  enqueue_packet(client, packet, size);
//}

}  // namespace diablo2
