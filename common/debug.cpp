
#include <Windows.h>

#include <common/file_ini.h>
#include <common/debug.h>
#include <d2tweaks/ui/ui_utils.h>

using namespace d2_tweaks;

Debugger::Debugger(std::string ini_key)
    : ini_key_(ini_key) {
}

void Debugger::WaitForDebugger(std::string module_name) {
#ifndef NDEBUG
  FileIni config("d2tweaks_debugger.ini");

  if (!config.Int("WaitForDebugger", ini_key_, 0)) {
    return;
  }

  std::string msg = "D2TWeaks - ";

  if (module_name.empty()) {
    msg += ini_key_;
  }
  else {
    msg += module_name;
  }

  MessageBox(ui::FindDiabloIIWindow(),
             "Wait For Debugger",
             msg.c_str(),
             MB_OK);
#endif
}
