/********************************************************************
  Copyright 2014, Cong Quyen Knight. All rights reserved

  project:   Aries Protector Project
  author:    quyen19492
  email:     quyen19492@gmail.com

  created:   2021/05/02 19:18
  filename:  AriesBase\aries_lib\modules\utils\strings.cpp

  purpose:
*********************************************************************/


// -----------------------------------------------------------------------------
#include "strings.h"

#include <map>
#include "get_default.h"
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
namespace utils {
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
namespace {
// -----------------------------------------------------------------------------

static const std::map<char, char> kEscapeTable = {
    //
    {'a', static_cast<char>(0x07)},
    {'b', static_cast<char>(0x08)},
    {'f', static_cast<char>(0x0C)},
    {'n', static_cast<char>(0x0A)},
    {'r', static_cast<char>(0x0D)},
    {'t', static_cast<char>(0x09)},
    {'v', static_cast<char>(0x0B)},
    {'\\', static_cast<char>(0x5C)},
    {'\'', static_cast<char>(0x27)},
    {'\"', static_cast<char>(0x22)},
};
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
struct unescape_accumulator {
  std::string result;
  bool escaping = false;

  void operator()(char c) {
    if (escaping) {
      char val = utils::GetDefault(kEscapeTable, c, c);
      result.push_back(val);
      escaping = false;
    } else {
      if (c == '\\') {
        escaping = true;
      } else {
        result.push_back(c);
      }
    }
  }
};
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
static const std::wstring upper_vietnamese =
    L"AÀÁẢÃẠĂẰẮẲẴẶÂẦẤẨẪẬ"
    L"Đ"
    L"EÈÉẺẼẸÊỀẾỂỄỆ"
    L"IÌÍỈĨỊ"
    L"OÒÓỎÕỌÔỒỐỔỖỘƠỜỚỞỠỢ"
    L"UÙÚỦŨỤƯỪỨỬỮỰ"
    L"YỲÝỶỸỴ";
static const std::wstring lower_vietnamese =
    L"aàáảãạăằắẳẵặâầấẩẫậ"
    L"đ"
    L"eèéẻẽẹêềếểễệ"
    L"iìíỉĩị"
    L"oòóỏõọôồốổỗộơờớởỡợ"
    L"uùúủũụưừứửữự"
    L"yỳýỷỹỵ";
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

inline int isbase64(int c) { return isalnum(c) || (c == '+') || (c == '/'); }
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
}  // namespace
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// GENERIC STRING CONVERSIONS
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

std::string ToString(const std::wstring& s) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.to_bytes(s);
}
// -----------------------------------------------------------------------------

std::string ToString(const wchar_t* s) {
  std::wstring wtext = s;
  return ToString(wtext);
}
// -----------------------------------------------------------------------------

std::string string_format(const char* format, ...) {
  va_list args;
  va_start(args, format);
  size_t len = _vscprintf(format, args);
  bytes buff(len + 1);
  char* output = reinterpret_cast<char*>(&buff[0]);
  vsprintf_s(output, len + 1, format, args);
  va_end(args);
  return output;
}
// -----------------------------------------------------------------------------

bool replace(std::string& s,
             const std::string& sub_from,
             const std::string& sub_to) {
  if (sub_from.empty()) {
    return false;
  }

  size_t start_pos = s.find(sub_from);
  if (start_pos == std::string::npos) {
    return false;
  }

  s.replace(start_pos, sub_from.length(), sub_to);
  return true;
}
// -----------------------------------------------------------------------------

void replace_all(std::string& s,
                 const std::string& sub_from,
                 const std::string& sub_to) {
  if (sub_from.empty()) {
    return;
  }

  size_t start_pos = 0;
  while ((start_pos = s.find(sub_from, start_pos)) != std::string::npos) {
    s.replace(start_pos, sub_from.length(), sub_to);
    start_pos += sub_to.length();  // In case 'sub_to' contains 'sub_from'
                                   // like replacing 'x' with 'yx'
  }
}
// -----------------------------------------------------------------------------

std::string ToUpper(const std::string& s) {
  std::string upcase_string = s;
  std::transform(upcase_string.begin(),
                 upcase_string.end(),
                 upcase_string.begin(),
                 [](char c) { return static_cast<char>(std::toupper(c)); });
  return upcase_string;
}
// -----------------------------------------------------------------------------

std::string ToUpper(const char* s) {
  std::string text = s;
  return ToUpper(text);
}
// -----------------------------------------------------------------------------

std::string ToLower(const std::string& s) {
  std::string lowcase_string = s;
  std::transform(lowcase_string.begin(),
                 lowcase_string.end(),
                 lowcase_string.begin(),
                 [](char c) { return static_cast<char>(std::tolower(c)); });
  return lowcase_string;
}
// -----------------------------------------------------------------------------

std::string ToLower(const char* s) {
  std::string text = s;
  return ToLower(text);
}
// -----------------------------------------------------------------------------

std::wstring ToWString(const std::string& s) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.from_bytes(s);
}
// -----------------------------------------------------------------------------

std::wstring ToWString(const char* s) {
  std::string text = s;
  return ToWString(text);
}
// -----------------------------------------------------------------------------

std::wstring string_format(const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  size_t len = _vscwprintf(format, args);
  bytes buff((len + 1) * sizeof(wchar_t));
  wchar_t* output = reinterpret_cast<wchar_t*>(&buff[0]);
  vswprintf_s(output, len + 1, format, args);
  va_end(args);
  return output;
}
// -----------------------------------------------------------------------------

bool replace(std::wstring& s,
             const std::wstring& sub_from,
             const std::wstring& sub_to) {
  if (sub_from.empty()) {
    return false;
  }

  size_t start_pos = s.find(sub_from);
  if (start_pos == std::wstring::npos) {
    return false;
  }

  s.replace(start_pos, sub_from.length(), sub_to);
  return true;
}
// -----------------------------------------------------------------------------

void replace_all(std::wstring& s,
                 const std::wstring& sub_from,
                 const std::wstring& sub_to) {
  if (sub_from.empty()) {
    return;
  }

  size_t start_pos = 0;
  while ((start_pos = s.find(sub_from, start_pos)) != std::wstring::npos) {
    s.replace(start_pos, sub_from.length(), sub_to);
    start_pos += sub_to.length();  // In case 'sub_to' contains 'sub_from'
                                   // like replacing 'x' with 'yx'
  }
}
// -----------------------------------------------------------------------------

std::wstring ToUpper(const std::wstring& s) {
  std::wstring upcase_string = s;
  std::transform(upcase_string.begin(),
                 upcase_string.end(),
                 upcase_string.begin(),
                 [](wchar_t c) {
                   size_t pos = lower_vietnamese.find(c);
                   if (pos != std::wstring::npos) {
                     return upper_vietnamese[pos];
                   }
                   return static_cast<wchar_t>(towupper(c));
                 });
  return upcase_string;
}
// -----------------------------------------------------------------------------

std::wstring ToUpper(const wchar_t* s) {
  std::wstring text = s;
  return ToUpper(text);
}
// -----------------------------------------------------------------------------

std::wstring ToLower(const std::wstring& s) {
  std::wstring lowcase_string = s;
  std::transform(lowcase_string.begin(),
                 lowcase_string.end(),
                 lowcase_string.begin(),
                 [](wchar_t c) {
                   size_t pos = upper_vietnamese.find(c);
                   if (pos != std::wstring::npos) {
                     return lower_vietnamese[pos];
                   }
                   return static_cast<wchar_t>(towlower(c));
                 });
  return lowcase_string;
}
// -----------------------------------------------------------------------------

std::wstring ToLower(const wchar_t* s) {
  std::wstring text = s;
  return ToLower(text);
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// STRINGS FILTERING
// -----------------------------------------------------------------------------

std::string FilterNumeric(const std::string& in) {
  return FilterString(in, isdigit);
}
// -----------------------------------------------------------------------------

std::string FilterNumeric(const char* data, size_t size) {
  return FilterString(data, size, isdigit);
}
// -----------------------------------------------------------------------------

std::string FilterNumeric(const char* data) {
  return FilterString(data, isdigit);
}
// -----------------------------------------------------------------------------

std::string FilterAlphabetic(const std::string& in) {
  return FilterString(in, isalpha);
}
// -----------------------------------------------------------------------------

std::string FilterAlphabetic(const char* data, size_t size) {
  return FilterString(data, size, isalpha);
}
// -----------------------------------------------------------------------------

std::string FilterAlphabetic(const char* data) {
  return FilterString(data, isalpha);
}
// -----------------------------------------------------------------------------

std::string FilterAlpanumeric(const std::string& in) {
  return FilterString(in, isalnum);
}
// -----------------------------------------------------------------------------

std::string FilterAlpanumeric(const char* data, size_t size) {
  return FilterString(data, size, isalnum);
}
// -----------------------------------------------------------------------------

std::string FilterAlpanumeric(const char* data) {
  return FilterString(data, isalnum);
}
// -----------------------------------------------------------------------------

std::string FilterHexDigits(const std::string& in) {
  return FilterString(in, isxdigit);
}
// -----------------------------------------------------------------------------

std::string FilterHexDigits(const char* data, size_t size) {
  return FilterString(data, size, isxdigit);
}
// -----------------------------------------------------------------------------

std::string FilterHexDigits(const char* data) {
  return FilterString(data, isxdigit);
}
// -----------------------------------------------------------------------------

std::string FilterBase64Digits(const std::string& in) {
  return FilterString(in, isbase64);
}
// -----------------------------------------------------------------------------

std::string FilterBase64Digits(const char* data, size_t size) {
  return FilterString(data, size, isbase64);
}
// -----------------------------------------------------------------------------

std::string FilterBase64Digits(const char* data) {
  return FilterString(data, isbase64);
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// STRING <-> BYTES CONVERSIONS
// -----------------------------------------------------------------------------

utils::bytes ToBytes(const std::string& in) {
  if (in.empty()) {
    return bytes();
  }

  return bytes(in.begin(), in.end());
}
// -----------------------------------------------------------------------------

utils::bytes ToBytes(const std::wstring& in) {
  if (in.empty()) {
    return bytes();
  }

  bytes buff((in.size() + 1) * sizeof(wchar_t), 0);
  memcpy(&buff[0], in.c_str(), in.length() * sizeof(wchar_t));
  return buff;
}
// -----------------------------------------------------------------------------

std::string ToString(const bytes& data) {
  return std::string(data.begin(), data.end());
}
// -----------------------------------------------------------------------------

std::wstring ToWString(const bytes& data) {
  return reinterpret_cast<const wchar_t*>(&data[0]);
}
// -----------------------------------------------------------------------------

utils::bytes StringToBytes(const std::string& text) {
  return bytes(text.data(), text.data() + text.size());
}
// -----------------------------------------------------------------------------

utils::bytes HexStringToBytes(
    const std::string& text,
    size_t msz /*= (std::numeric_limits<size_t>::max)()*/) {
  return std::for_each(text.begin(), text.end(), hex_accumulator(msz)).result;
}
// -----------------------------------------------------------------------------

std::string BytesToHexString(const std::uint8_t *data, size_t size,
                             int bytes_per_group,
                             int groups_per_line) {
  if (size == 0)
    return "";

  std::ostringstream oss;
  int nbytes = 0;
  int ngroups = 0;
  oss << std::setfill('0') << std::hex << std::uppercase;
  for (size_t i = 0; i < size; i++) {
    oss << std::setw(2) << int { data[i] };
    if (++nbytes == bytes_per_group && i != (size - 1)) {
      if (++ngroups == groups_per_line) {
        oss << "\r\n";
        nbytes = ngroups = 0;
      }
      else {
        oss << " ";
        nbytes = 0;
      }
    }
  }

  return oss.str();
}
// -----------------------------------------------------------------------------

std::wstring BytesToHexWString(const std::uint8_t* data,
                              size_t size,
                              int bytes_per_group /*= 0*/,
                              int groups_per_line /*= 0*/) {
  return utils::ToWString(
      utils::BytesToHexString(data, size, bytes_per_group, groups_per_line));
}
// -----------------------------------------------------------------------------

std::string BytesToHexString(const bytes& in,
                             int bytes_per_group /*= 0*/,
                             int groups_per_line /*= 0*/) {
  return BytesToHexString(in.data(),  //
                          in.size(),
                          bytes_per_group,
                          groups_per_line);
}
// -----------------------------------------------------------------------------

std::wstring BytesToHexWString(const bytes& in,
                               int bytes_per_group /*= 0*/,
                               int groups_per_line /*= 0*/) {
  return utils::ToWString(
      utils::BytesToHexString(in, bytes_per_group, groups_per_line));
}
// -----------------------------------------------------------------------------

bytes Base64StringToBytes(const char* encoded_string, size_t in_len) {
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::vector<uint8_t> ret;

  while (in_len-- && (encoded_string[in_] != '=') &&
         isbase64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_];
    in_++;
    if (i == 4) {
      for (i = 0; i < 4; i++)
        char_array_4[i] = static_cast<char>(base64_chars.find(char_array_4[i]));

      char_array_3[0] =
          (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] =
          ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret.push_back(char_array_3[i]);
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 4; j++)
      char_array_4[j] = 0;

    for (j = 0; j < 4; j++)
      char_array_4[j] = static_cast<char>(base64_chars.find(char_array_4[j]));

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] =
        ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++)
      ret.push_back(char_array_3[j]);
  }

  return ret;
}
// -----------------------------------------------------------------------------

utils::bytes Base64StringToBytes(const std::string& in) {
  return Base64StringToBytes(in.data(), in.size());
}
// -----------------------------------------------------------------------------

std::string BytesToBase64String(const uint8_t* bytes_to_encode, size_t in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] =
          ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] =
          ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; (i < 4); i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] =
        ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] =
        ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while ((i++ < 3))
      ret += '=';
  }

  return ret;
}
// -----------------------------------------------------------------------------

std::string BytesToBase64String(const bytes& in) {
  return BytesToBase64String(in.data(), in.size());
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// STRING FORMATTING
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

std::string LeftAligned(const std::string& data,
                        size_t size,
                        char pad /*= ' '*/) {
  if (data.size() >= size)
    return data.substr(0, size);
  else
    return std::string(data).append(size - data.size(), pad);
}
// -----------------------------------------------------------------------------

std::string LeftTrimmed(const std::string& data,
                        size_t size,
                        std::uint8_t pad /*= ' '*/) {
  size_t begin_off = (std::max)(data.size(), size) - size;
  while (begin_off < data.size() && data[begin_off] == pad)
    begin_off++;
  return data.substr(begin_off);
}
// -----------------------------------------------------------------------------

std::string RightAligned(const std::string& data,
                         size_t size,
                         char pad /*= ' '*/) {
  if (data.size() >= size)
    return data.substr(data.size() - size, size);
  else
    return std::string(size - data.size(), pad).append(data);
}
// -----------------------------------------------------------------------------

std::string RightTrimmed(const std::string& data,
                         size_t size,
                         std::uint8_t pad /*= ' '*/) {
  size_t end_off = (std::min)(data.size(), size);
  while (end_off > 0 && data[end_off - 1] == pad)
    end_off--;
  return data.substr(0, end_off);
}
// -----------------------------------------------------------------------------

std::string NumericPadded(const std::string& data, size_t size) {
  return NumericPadded(data.begin(), data.end(), size);
}
// -----------------------------------------------------------------------------

std::string NumericPadded(const char* data, size_t size) {
  return NumericPadded(data, data + strlen(data), size);
}
// -----------------------------------------------------------------------------

std::string NumericTrimmed(const std::string& data) {
  std::string result = LeftTrimmed(data, data.size(), '0');
  if (result.size() == 0)
    result.push_back('0');
  return result;
}
// -----------------------------------------------------------------------------

std::string UnescapeCString(const std::string& text) {
  return std::for_each(text.begin(), text.end(), unescape_accumulator()).result;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
}  // namespace utils
// -----------------------------------------------------------------------------
