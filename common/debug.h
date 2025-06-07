#pragma once

#include <string>

class Debugger {
 public:
  Debugger(std::string ini_key);

  void WaitForDebugger(std::string module_name = "");

 private:
  std::string ini_key_;
};
