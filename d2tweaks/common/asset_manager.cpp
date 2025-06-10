#include <Windows.h>

#include "d2tweaks/common/asset_manager.h"

#include <spdlog/spdlog.h>

#include "d2tweaks/common/asset.h"

#include "diablo2/d2client.h"
#include "diablo2/d2win.h"

#include "common/hooking.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

extern HMODULE g_hModule;

namespace d2_tweaks {
namespace common {

std::string get_current_dir() {
  char path[MAX_PATH];
  DWORD length = GetModuleFileNameA(g_hModule, path, MAX_PATH);
  if (length == 0 || length == MAX_PATH)
      return "";

  std::string fullPath(path);

  size_t pos = fullPath.find_last_of("\\/");
  if (pos == std::string::npos)
      return "";

  return fullPath.substr(0, pos).c_str();
}

static std::string config_path;
std::string get_config_path() {
  if (!config_path.empty()) {
    return config_path.c_str();
  }

  config_path = get_current_dir() + "\\d2tweaks.ini";
  return config_path;
}

static void* g_d2_tweaks_mpq = nullptr;

static int32_t(__stdcall* g_reload_original)();

asset_manager::asset_manager(token) {}

void asset_manager::init() {
  detour::hook(d2_client::get_base() + 0x5E370,
               reload,
               reinterpret_cast<void **>(&g_reload_original));

  g_d2_tweaks_mpq = d2_win::load_mpq(
      "d2tweaks.dll", "d2tweaks.mpq", "D2TWEAKS", 6000);

  // if (g_d2_tweaks_mpq == nullptr) {
  //   spdlog::critical("Cannot find d2tweaks.mpq!");
  //   exit(-1);
  // }

  spdlog::debug("d2tweaks.mpq: {0}", g_d2_tweaks_mpq);
}

int32_t asset_manager::reload() {
  static auto& instance = singleton<asset_manager>::instance();
  const auto result = g_reload_original();

  spdlog::info("Reloading image assets!");

  for (const auto& it : instance.m_assets) {
    if (it.second->get_type() != MPQ_FILE_TYPE_DC6 &&
      it.second->get_type() != MPQ_FILE_TYPE_DCC)
      continue;

    spdlog::info("Reloading {0}...", it.second->get_path());

    if (it.second->get()) {
      const auto ures = d2_client::unload_gfx_resource(
          static_cast<CellFile*>(it.second->get()));
      spdlog::info("Unloaded: {0}", ures);
    }

    const auto newData =
        instance.load_asset_data(it.second->get_path(), it.second->get_type());
    it.second->update(newData);
    spdlog::info("Done! {0}", newData);
  }

  return result;
}

asset* asset_manager::get_mpq_file(const std::string& path,
                                   mpq_file_type_t type) {
  if (path.empty())
    return nullptr;

  const auto it = m_assets.find(path);

  if (it != m_assets.end())
    return it->second;

  const auto asset = new common::asset(path, load_asset_data(path, type), type);
  m_assets[path] = asset;

  return asset;
}

void* asset_manager::load_asset_data(const std::string& path, mpq_file_type_t type) {
  switch (type) {
  case MPQ_FILE_TYPE_DC6:
    return d2_client::load_gfx_resource(const_cast<char*>(path.c_str()));
  default:
    return nullptr;
  }
}

}  // namespace common
}  // namespace d2_tweaks
