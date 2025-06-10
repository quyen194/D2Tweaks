/********************************************************************
  Copyright 2014, Cong Quyen Knight. All rights reserved

  project:   Aries Base
  author:    quyen19492
  email:     quyen19492@gmail.com

  created:   2014/11/18
  created:   18:11:2014   11:19
  filename:  AriesBase\aries_lib\modules\ini_handle\ini_handle.cpp

  purpose:
*********************************************************************/


// -----------------------------------------------------------------------------
#include <Windows.h>

#include "common/strings.h"

#include "common/file_ini.h"
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------

FileIni::FileIni(const std::string &file_path) {
  SetFilePath(file_path);
}
// -----------------------------------------------------------------------------

FileIni::FileIni(const std::wstring &file_path) {
  SetFilePath(file_path);
}
// -----------------------------------------------------------------------------

void FileIni::SetFilePath(const std::string &file_path) {
  file_path_a_ = file_path;
  file_path_w_ = utils::ToWString(file_path);
}
// -----------------------------------------------------------------------------

void FileIni::SetFilePath(const std::wstring &file_path) {
  file_path_w_ = file_path;
  file_path_a_ = utils::ToString(file_path);
}
// -----------------------------------------------------------------------------

int FileIni::Int(const std::string &app, const std::string &key, int def) {
  EXPECT_TRUE1(!file_path_a_.empty(), 0);

  return GetPrivateProfileIntA(app.c_str(), key.c_str(), def,
                               file_path_a_.c_str());
}
// -----------------------------------------------------------------------------

int FileIni::Int(const std::wstring &app, const std::wstring &key, int def) {
  EXPECT_TRUE1(!file_path_w_.empty(), 0);

  return GetPrivateProfileIntW(app.c_str(), key.c_str(), def,
                               file_path_w_.c_str());
}
// -----------------------------------------------------------------------------

int FileIni::String(const std::string &app,
                    const std::string &key,
                    std::string &value,
                    size_t max_char,
                    const std::string &def) {
  EXPECT_TRUE1(!file_path_a_.empty(), 0);

  utils::bytes buff((max_char + 1) * sizeof(char));
  GetPrivateProfileStringA(app.c_str(),
                           key.c_str(),
                           def.c_str(),
                           (char *) &buff[0],
                           max_char,
                           file_path_a_.c_str());

  value = (char*) &buff[0];
  return value.length();
}
// -----------------------------------------------------------------------------

int FileIni::String(const std::wstring &app,
                    const std::wstring &key,
                    std::wstring &value,
                    size_t max_char,
                    const std::wstring &def) {
  EXPECT_TRUE1(!file_path_w_.empty(), 0);

  utils::bytes buff((max_char + 1) * sizeof(wchar_t));
  GetPrivateProfileStringW(app.c_str(),
                           key.c_str(),
                           def.c_str(),
                           (wchar_t *)&buff[0],
                           max_char,
                           file_path_w_.c_str());

  value = (wchar_t*) &buff[0];
  return value.length();
}
// -----------------------------------------------------------------------------

std::string FileIni::String(const std::string &app,
                            const std::string &key,
                            size_t max_char,
                            const std::string &def) {
  EXPECT_TRUE1(!file_path_a_.empty(), def);

  utils::bytes buff((max_char + 1) * sizeof(char));
  GetPrivateProfileStringA(app.c_str(),
                           key.c_str(),
                           def.c_str(),
                           (char *) &buff[0],
                           max_char,
                           file_path_a_.c_str());

  return (char *) &buff[0];
}
// -----------------------------------------------------------------------------

std::wstring FileIni::String(const std::wstring &app,
                             const std::wstring &key,
                             size_t max_char,
                             const std::wstring &def /*= L""*/) {
  EXPECT_TRUE1(!file_path_w_.empty(), def);
  utils::bytes buff((max_char + 1) * sizeof(wchar_t));
  GetPrivateProfileStringW(app.c_str(),
                           key.c_str(),
                           def.c_str(),
                           (wchar_t *) &buff[0],
                           max_char,
                           file_path_w_.c_str());

  return (wchar_t *) &buff[0];
}
// -----------------------------------------------------------------------------

bool FileIni::SetInt(const std::string &app,
                     const std::string &key,
                     int value) {
  EXPECT_TRUE1(!file_path_a_.empty(), 0);

  return !!WritePrivateProfileStringA(app.c_str(),
                                      key.c_str(),
                                      utils::ToString(value).c_str(),
                                      file_path_a_.c_str());
}
// -----------------------------------------------------------------------------

bool FileIni::SetInt(const std::wstring &app, const std::wstring &key,
                         int value) {
  EXPECT_TRUE1(!file_path_w_.empty(), 0);

  return !!WritePrivateProfileStringW(app.c_str(), key.c_str(),
                                      utils::ToWString(value).c_str(),
                                      file_path_w_.c_str());
}
// -----------------------------------------------------------------------------

bool FileIni::SetString(const std::string &app,
                        const std::string &key,
                        const std::string &value) {
  EXPECT_TRUE1(!file_path_a_.empty(), 0);

  return !!WritePrivateProfileStringA(app.c_str(),
                                      key.c_str(),
                                      value.c_str(),
                                      file_path_a_.c_str());
}
// -----------------------------------------------------------------------------

bool FileIni::SetString(const std::wstring &app,
                        const std::wstring &key,
                        const std::wstring &value) {
  EXPECT_TRUE1(!file_path_w_.empty(), 0);

  return !!WritePrivateProfileStringW(app.c_str(),
                                      key.c_str(),
                                      value.c_str(),
                                      file_path_w_.c_str());
}
// -----------------------------------------------------------------------------
