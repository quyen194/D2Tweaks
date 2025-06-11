
#include <filesystem>

#include <spdlog/spdlog.h>

#include "diablo2/fog.h"

#include "d2tweaks/client/loot_filter/loot_filter_settings.h"

using namespace d2_tweaks;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace client {

static char g_buffer[sizeof(LootFilterSettings)]{ 0 };
static LootFilterSettings* g_settings = reinterpret_cast<LootFilterSettings*>(g_buffer);

LootFilterSettings& LootFilterSettings::get() {
  return *g_settings;
}

void LootFilterSettings::save(const char* name) {
  static char savePath[1024];
  static char buffer[1024];
  fog::get_save_path(savePath, sizeof savePath);
  sprintf_s(buffer, "%s%s_lf.bin", savePath, name);

  spdlog::debug("Saving loot filter settings to {0}", buffer);

  FILE* file = nullptr;
  const auto err = fopen_s(&file, buffer, "wb");

  if (err != 0) {
    spdlog::critical("Cannot save loot filter settings to {0}", buffer);
    exit(0);
  }

  fwrite(g_buffer, sizeof(LootFilterSettings), 1, file);
  fclose(file);
}

void LootFilterSettings::load(const char* name) {
  static char savePath[1024];
  static char buffer[1024];
  fog::get_save_path(savePath, sizeof savePath);
  sprintf_s(buffer, "%s%s_lf.bin", savePath, name);

  spdlog::debug("Loading loot filter settings from {0}", buffer);

  if (!std::filesystem::exists(buffer)) {
    FILE* file = nullptr;
    const auto err = fopen_s(&file, buffer, "wb");

    if (err != 0) {
      spdlog::critical("Cannot create loot filter settings at {0}", buffer);
      exit(0);
    }

    LootFilterSettings dummy;
    fwrite(&dummy, sizeof dummy, 1, file);
    fclose(file);
  }

  FILE* file = nullptr;
  const auto err = fopen_s(&file, buffer, "rb");

  if (err != 0) {
    spdlog::critical("Cannot read loot filter settings from {0}", buffer);
    exit(0);
  }

  auto readed = fread(g_buffer, sizeof(LootFilterSettings), 1, file);
  fclose(file);
}

void LootFilterSettings::remove(const char* name) {
  static char savePath[1024];
  static char buffer[1024];
  fog::get_save_path(savePath, sizeof savePath);
  sprintf_s(buffer, "%s%s_lf.bin", savePath, name);

  spdlog::debug("Deleting loot filter settings from {0}", buffer);

  if (!std::filesystem::exists(buffer))
    return;

  std::filesystem::remove(buffer);
  memset(g_buffer, 0x00, sizeof g_buffer);
}

}  // namespace client
}  // namespace d2_tweaks
