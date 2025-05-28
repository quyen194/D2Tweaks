/********************************************************************
  Copyright 2014, Cong Quyen Knight. All rights reserved

  project:   Aries Protector Project
  author:    quyen19492
  email:     quyen19492@gmail.com

  created:   2021/05/02 19:09
  filename:  AriesBase\aries_lib\modules\utils\strings.h

  purpose:
*********************************************************************/


// -----------------------------------------------------------------------------
#ifndef ARIESBASE_ARIES_LIB_MODULES_UTILS_STRINGS_H
#define ARIESBASE_ARIES_LIB_MODULES_UTILS_STRINGS_H
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <locale>
#include <codecvt>
#include <stdarg.h>
#include <stdio.h>
#include "bytes.h"
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
namespace utils {
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// GENERIC STRING CONVERSIONS
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

template<typename T>
std::string ToString(const T &t) {
  std::ostringstream oss;
  oss << t;
  return oss.str();
}
// -----------------------------------------------------------------------------

std::string ToString(const std::wstring &s);
// -----------------------------------------------------------------------------

std::string ToString(const wchar_t *s);
// -----------------------------------------------------------------------------

template<typename T>
T FromString(const std::string& s) {
  std::istringstream stream(s);
  T t;
  stream >> t;
  return t;
}
// -----------------------------------------------------------------------------

template <typename T>
std::string ToHexString(const T &t) {
  std::ostringstream oss;
  oss << std::hex << t;
  return oss.str();
}
// -----------------------------------------------------------------------------

template<typename T>
T FromHexString(const std::string& s) {
  std::istringstream stream(s);
  T t;
  stream >> std::hex >> t;
  return t;
}
// -----------------------------------------------------------------------------

std::string string_format(const char *format, ...);
// -----------------------------------------------------------------------------

bool replace(std::string &s,
                    const std::string &sub_from,
                    const std::string &sub_to);
// -----------------------------------------------------------------------------

void replace_all(std::string &s,
                        const std::string &sub_from,
                        const std::string &sub_to);
// -----------------------------------------------------------------------------

std::string ToUpper(const std::string &s);
// -----------------------------------------------------------------------------

std::string ToUpper(const char *s);
// -----------------------------------------------------------------------------

std::string ToLower(const std::string &s);
// -----------------------------------------------------------------------------

std::string ToLower(const char *s);
// -----------------------------------------------------------------------------

template<typename T>
std::wstring ToWString(const T &t) {
  std::wostringstream oss;
  oss << t;
  return oss.str();
}
// -----------------------------------------------------------------------------

std::wstring ToWString(const std::string &s);
// -----------------------------------------------------------------------------

std::wstring ToWString(const char *s);
// -----------------------------------------------------------------------------

template<typename T>
T FromWString(const std::wstring &s) {
  std::wistringstream stream(s);
  T t;
  stream >> t;
  return t;
}
// -----------------------------------------------------------------------------

template <typename T>
std::wstring ToHexWString(const T &t) {
  std::wostringstream oss;
  oss << t;
  return oss.str();
}
// -----------------------------------------------------------------------------

template<typename T>
T FromHexWString(const std::wstring &s) {
  std::wistringstream stream(s);
  T t;
  stream >> std::hex >> t;
  return t;
}
// -----------------------------------------------------------------------------

std::wstring string_format(const wchar_t *format, ...);
// -----------------------------------------------------------------------------

bool replace(std::wstring &s,
                    const std::wstring &sub_from,
                    const std::wstring &sub_to);
// -----------------------------------------------------------------------------

void replace_all(std::wstring &s,
                        const std::wstring &sub_from,
                        const std::wstring &sub_to);
// -----------------------------------------------------------------------------

std::wstring ToUpper(const std::wstring &s);
// -----------------------------------------------------------------------------

std::wstring ToUpper(const wchar_t *s);
// -----------------------------------------------------------------------------

std::wstring ToLower(const std::wstring &s);
// -----------------------------------------------------------------------------

std::wstring ToLower(const wchar_t *s);
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// STRINGS FILTERING
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

// filter characters from input string
template<typename InputIt, typename Pred>
std::string FilterString(InputIt begin, InputIt end, Pred pred) {
  std::string result;
  std::copy_if(begin, end, std::back_inserter(result), pred);
  return result;
}
// -----------------------------------------------------------------------------

template<typename Pred>
std::string FilterString(const std::string &in, Pred pred) {
  return FilterString(in.begin(), in.end(), pred);
}
// -----------------------------------------------------------------------------

template<typename Pred>
std::string FilterString(const char *data, size_t size, Pred pred) {
  return FilterString(data, data + size, pred);
}
// -----------------------------------------------------------------------------

template<typename Pred>
std::string FilterString(const char *data, Pred pred) {
  return FilterString(data, strlen(data), pred);
}
// -----------------------------------------------------------------------------

std::string FilterNumeric(const std::string &in);
// -----------------------------------------------------------------------------

std::string FilterNumeric(const char *data, size_t size);
// -----------------------------------------------------------------------------

std::string FilterNumeric(const char *data);
// -----------------------------------------------------------------------------

std::string FilterAlphabetic(const std::string &in);
// -----------------------------------------------------------------------------

std::string FilterAlphabetic(const char *data, size_t size);
// -----------------------------------------------------------------------------

std::string FilterAlphabetic(const char *data);
// -----------------------------------------------------------------------------

std::string FilterAlpanumeric(const std::string &in);
// -----------------------------------------------------------------------------

std::string FilterAlpanumeric(const char *data, size_t size);
// -----------------------------------------------------------------------------

std::string FilterAlpanumeric(const char *data);
// -----------------------------------------------------------------------------

std::string FilterHexDigits(const std::string &in);
// -----------------------------------------------------------------------------

std::string FilterHexDigits(const char *data, size_t size);
// -----------------------------------------------------------------------------

std::string FilterHexDigits(const char *data);
// -----------------------------------------------------------------------------

std::string FilterBase64Digits(const std::string &in);
// -----------------------------------------------------------------------------

std::string FilterBase64Digits(const char *data, size_t size);
// -----------------------------------------------------------------------------

std::string FilterBase64Digits(const char *data);
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// STRING <-> BYTES CONVERSIONS
// -----------------------------------------------------------------------------

 // -----------------------------------------------------------------------------

template<typename InputIt>
bytes TextStringToBytes(InputIt begin, InputIt end) {
  return bytes(begin, end);
}
// -----------------------------------------------------------------------------

bytes ToBytes(const std::string &in);
// -----------------------------------------------------------------------------

bytes ToBytes(const std::wstring &in);
// -----------------------------------------------------------------------------

std::string ToString(const bytes &data);
// -----------------------------------------------------------------------------

std::wstring ToWString(const bytes &data);
// -----------------------------------------------------------------------------

// convert hexadecimal character to integer
inline int xvalue(char c) {
  if ((c >= '0') && (c <= '9'))
    return c - '0';
  c &= ~0x20;   // force to upper case
  if ((c >= 'A') && (c <= 'F'))
    return c - 'A' + 10;
  return -1;
}
// -----------------------------------------------------------------------------

// decode string containing hex characters.
struct hex_accumulator {
  bytes result;
  bool even;
  size_t msz;

  explicit hex_accumulator(size_t msz = (std::numeric_limits<size_t>::max)())
      : even(true),
        msz(msz) {
  }
  void operator()(char c) {
    if (((result.size() < msz) || !even) && isxdigit(c)) {
      if (even)
        result.push_back(std::uint8_t(xvalue(c) << 4));
      else
        result.back() |= xvalue(c);
      even = !even;
    }
  }
};
// -----------------------------------------------------------------------------

bytes StringToBytes(const std::string &text);
// -----------------------------------------------------------------------------

bytes HexStringToBytes(const std::string &text,
                       size_t msz = (std::numeric_limits<size_t>::max)());
// -----------------------------------------------------------------------------

// convert bytes to hexadecimal string
std::string BytesToHexString(const std::uint8_t *data,
                             size_t size,
                             int bytes_per_group = 0,
                             int groups_per_line = 0);
// -----------------------------------------------------------------------------

std::wstring BytesToHexWString(const std::uint8_t *data,
                               size_t size,
                               int bytes_per_group = 0,
                               int groups_per_line = 0);
// -----------------------------------------------------------------------------

std::string BytesToHexString(const bytes &in,
                             int bytes_per_group = 0,
                             int groups_per_line = 0);
// -----------------------------------------------------------------------------

std::wstring BytesToHexWString(const bytes &in,
                               int bytes_per_group = 0,
                               int groups_per_line = 0);
// -----------------------------------------------------------------------------

// convert base64 string to bytes
bytes Base64StringToBytes(const char* data, size_t size);
// -----------------------------------------------------------------------------

bytes Base64StringToBytes(const std::string& in);
// -----------------------------------------------------------------------------

// convert bytes to base64 string
std::string BytesToBase64String(const std::uint8_t *data, size_t size);
// -----------------------------------------------------------------------------

std::string BytesToBase64String(const bytes& in);
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// STRING FORMATTING
// -----------------------------------------------------------------------------

// align beginning of data, padding to the right
std::string LeftAligned(const std::string& data,
                               size_t size,
                               char pad = ' ');
// -----------------------------------------------------------------------------

// remove padding and excess characters on the left
std::string LeftTrimmed(const std::string& data,
                               size_t size,
                               std::uint8_t pad = ' ');
// -----------------------------------------------------------------------------

// align end of data, padding to the left
std::string RightAligned(const std::string& data,
                                size_t size,
                                char pad = ' ');
// -----------------------------------------------------------------------------

// remove padding and excess characters on the right
std::string RightTrimmed(const std::string& data,
                                size_t size,
                                std::uint8_t pad = ' ');
// -----------------------------------------------------------------------------

// pad with leading 0s
template<typename T>
std::string NumericPadded(T data, size_t size) {
  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(size) << data;
  return oss.str();
}
// -----------------------------------------------------------------------------

template<typename InputIt>
std::string NumericPadded(InputIt begin, InputIt end, size_t size) {
  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(size)
      << FilterString(begin, end, isdigit);
  return oss.str();
}
// -----------------------------------------------------------------------------

std::string NumericPadded(const std::string& data, size_t size);
// -----------------------------------------------------------------------------

std::string NumericPadded(const char *data, size_t size);
// -----------------------------------------------------------------------------

// remove leading 0s
std::string NumericTrimmed(const std::string& data);
// -----------------------------------------------------------------------------

std::string UnescapeCString(const std::string &text);
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
}  // namespace utils
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#endif  // ARIESBASE_ARIES_LIB_MODULES_UTILS_STRINGS_H
// -----------------------------------------------------------------------------
