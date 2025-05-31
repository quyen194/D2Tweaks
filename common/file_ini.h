/********************************************************************
  Copyright 2014, Cong Quyen Knight. All rights reserved

  project:   Aries Base
  author:    quyen19492
  email:     quyen19492@gmail.com

  created:   2014/11/18
  created:   18:11:2014   11:00
  filename:  AriesBase\aries_lib\modules\ini_handle\ini_handle.h

  purpose:
*********************************************************************/


// -----------------------------------------------------------------------------
#ifndef ARIES_BASE_ARIES_LIB_MODULES_INI_HANDLE_INI_HANDLE_H
#define ARIES_BASE_ARIES_LIB_MODULES_INI_HANDLE_INI_HANDLE_H
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#include <string>

#include "include/macro.h"
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
class FileIni {
 public:
  FileIni(const std::string &file_path);
  FileIni(const std::wstring &file_path);
  virtual ~FileIni() {}

  void SetFilePath(const std::string &file_path);
  void SetFilePath(const std::wstring &file_path);

  // get
  int Int(const std::string &app, const std::string &key, int def = 0);
  int Int(const std::wstring &app, const std::wstring &key, int def = 0);

  int String(const std::string &app,
             const std::string &key,
             std::string &value,
             size_t max_char,
             const std::string &def = "");
  int String(const std::wstring &app,
             const std::wstring &key,
             std::wstring &value,
             size_t max_char,
             const std::wstring &def = L"");

  std::string String(const std::string &app,
                     const std::string &key,
                     size_t max_char,
                     const std::string &def = "");
  std::wstring String(const std::wstring &app,
                      const std::wstring &key,
                      size_t max_char,
                      const std::wstring &def = L"");

  // set
  bool SetInt(const std::string &app, const std::string &key, int value);
  bool SetInt(const std::wstring &app, const std::wstring &key, int value);
  bool SetString(const std::string &app,
                 const std::string &key,
                 const std::string &value);
  bool SetString(const std::wstring &app,
                 const std::wstring &key,
                 const std::wstring &value);

 protected:
  std::string file_path_a_;
  std::wstring file_path_w_;

 private:
  DISALLOW_COPY_AND_ASSIGN(FileIni);
};
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#endif  // ARIES_BASE_ARIES_LIB_MODULES_INI_HANDLE_INI_HANDLE_H
// -----------------------------------------------------------------------------
