
#include <Windows.h>

#include "common/debug.h"
#include "common/file_ini.h"

#include "d2tweaks/common/asset_manager.h"
#include "d2tweaks/ui/ui_utils.h"

using namespace d2_tweaks;

Debugger::Debugger(std::string ini_key)
    : ini_key_(ini_key) {
}

void Debugger::WaitForDebugger(std::string module_name) {
#ifndef NDEBUG
  FileIni config(common::get_current_dir() + "\\d2tweaks_debugger.ini");

  if (!config.Int("WaitForDebugger", ini_key_, 0)) {
    return;
  }

  std::string msg = module_name;
  if (msg.empty()) {
    msg = ini_key_;
  }

  MessageBox(ui::FindDiabloIIWindow(),
             msg.c_str(),
             "D2TWeaks - Wait For Debugger",
             MB_OK);
#endif
}
