#include <common/config.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

config::config(token)
    : m_unlock_fps(false),
      m_prevent_minimize(false),
      m_gold_pickup_range(0) {
  m_json = new nlohmann::json();

  if (std::filesystem::exists("d2tweaks.json")) {
    const std::ifstream cfgFile("d2tweaks.json");
    std::stringstream ss;
    ss << cfgFile.rdbuf();

    nlohmann::json obj = m_json->parse(ss.str());
  }
}
