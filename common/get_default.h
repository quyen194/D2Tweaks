/********************************************************************
  Copyright 2014, Cong Quyen Knight. All rights reserved

  project:   Aries Protector Project
  author:    quyen19492
  email:     quyen19492@gmail.com

  created:   2021/05/02 19:09
  filename:  AriesBase\aries_lib\modules\string_utils\get_default.h

  purpose:
*********************************************************************/


// -----------------------------------------------------------------------------
#ifndef ARIESBASE_ARIES_LIB_MODULES_STRING_UTILS_GET_DEFAULT_H
#define ARIESBASE_ARIES_LIB_MODULES_STRING_UTILS_GET_DEFAULT_H
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
namespace utils {

template <template <class, class, class...> class C, typename K, typename V,
          typename... Args>
V GetDefault(const C<K, V, Args...>& m, K const& key, const V& defval) {
  typename C<K, V, Args...>::const_iterator it = m.find(key);
  if (it == m.end()) return defval;
  return it->second;
}

}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#endif  // ARIESBASE_ARIES_LIB_MODULES_STRING_UTILS_GET_DEFAULT_H
// -----------------------------------------------------------------------------
