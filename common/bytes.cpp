/********************************************************************
  Copyright 2014, Cong Quyen Knight. All rights reserved

  project:   Aries Protector Project
  author:    quyen19492
  email:     quyen19492@gmail.com

  created:   2024/04/09 11:23
  filename:  AriesBase\aries_lib\modules\utils\bytes.h

  purpose:
*********************************************************************/


// -----------------------------------------------------------------------------
#include <Windows.h>

#include "include/macro.h"

#include "bytes.h"
#include "strings.h"
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
namespace utils {
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

utils::bytes ToBytes(const void* data, size_t size) {
  if (!size) {
    return bytes();
  }

  bytes buff(size, 0);
  memcpy(&buff[0], data, size);
  return buff;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
}  // namespace utils
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

utils::bytes operator+(const utils::bytes& lhs, const utils::bytes& rhs) {
  utils::bytes result(lhs);
  result.insert(result.end(), rhs.begin(), rhs.end());
  return result;
}
// -----------------------------------------------------------------------------

utils::bytes& operator+=(utils::bytes& lhs, const utils::bytes& rhs) {
  lhs.insert(lhs.end(), rhs.begin(), rhs.end());
  return lhs;
}
// -----------------------------------------------------------------------------
