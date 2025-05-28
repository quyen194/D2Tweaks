/********************************************************************
  Copyright 2014, Cong Quyen Knight. All rights reserved

  project:   Aries Base
  author:    quyen19492
  email:     quyen19492@gmail.com

  created:   2014/05/24 18:39
  filename:  AriesBase\include\macro.h

  purpose:   * Một số các Macro hữu dụng
*********************************************************************/

// -----------------------------------------------------------------------------
#ifndef ARIES_BASE_INCLUDE_MACRO_H
#define ARIES_BASE_INCLUDE_MACRO_H
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#include <assert.h>
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

#define _assert(x) assert(!(x))

// -----------------------------------------------------------------------------
// Dùng để debug, kiểm tra tính hợp lệ của đầu vào
#define HOPE_NN0(pointer)                                 \
            if ((pointer) == nullptr)                     \
              return
#define EXPECT_NN0(pointer)                               \
            assert((pointer) != nullptr);                 \
            HOPE_NN0(pointer)
#define HOPE_NN1(pointer, return_value)                   \
            if ((pointer) == nullptr)                     \
              return return_value
#define EXPECT_NN1(pointer, return_value)                 \
            assert((pointer) != nullptr);                 \
            HOPE_NN1(pointer, return_value)

#define HOPE_NULL0(pointer)                               \
            if ((pointer) != nullptr)                     \
              return
#define EXPECT_NULL0(pointer)                             \
            assert((pointer) == nullptr);                 \
            HOPE_NULL0(pointer)
#define HOPE_NULL1(pointer, return_value)                 \
            if ((pointer) != nullptr)                     \
              return return_value
#define EXPECT_NULL1(pointer, return_value)               \
            assert((pointer) == nullptr);                 \
            HOPE_NULL1(pointer, return_value)

#define HOPE_NZ0(expression)                              \
            if ((expression) == 0)                        \
              return
#define EXPECT_NZ0(expression)                            \
            assert((expression) != 0);                    \
            HOPE_NZ0(expression)
#define HOPE_NZ1(expression, return_value)                \
            if ((expression) == 0)                        \
              return return_value
#define EXPECT_NZ1(expression, return_value)              \
            assert((expression) != 0);                    \
            HOPE_NZ1(expression, return_value)

#define HOPE_EZ0(expression)                              \
            if ((expression) != 0)                        \
              return
#define EXPECT_EZ0(expression)                            \
            assert((expression) == 0);                    \
            HOPE_EZ0(expression)
#define HOPE_EZ1(expression, return_value)                \
            if ((expression) != 0)                        \
              return return_value
#define EXPECT_EZ1(expression, return_value)              \
            assert((expression) == 0);                    \
            HOPE_EZ1(expression, return_value)

#define HOPE_TRUE0(expression)                            \
            if (!(expression))                            \
              return
#define EXPECT_TRUE0(expression)                          \
            assert(expression);                           \
            HOPE_TRUE0(expression)
#define HOPE_TRUE1(expression, return_value)              \
            if (!(expression))                            \
              return return_value
#define EXPECT_TRUE1(expression, return_value)            \
            assert(expression);                           \
            HOPE_TRUE1(expression, return_value)

#define HOPE_FALSE0(expression)                           \
            if (expression)                               \
              return
#define EXPECT_FALSE0(expression)                         \
            assert(!(expression));                        \
            HOPE_FALSE0(expression)
#define HOPE_FALSE1(expression, return_value)             \
            if (expression)                               \
              return return_value
#define EXPECT_FALSE1(expression, return_value)           \
            assert(!(expression));                        \
            HOPE_FALSE1(expression, return_value)

#define HOPE_EQ0(actual, expected)                        \
            if ((actual) != (expected))                   \
              return
#define EXPECT_EQ0(actual, expected)                      \
            assert((actual) == (expected));               \
            HOPE_EQ0(actual, expected)
#define HOPE_EQ1(actual, expected, return_value)          \
            if ((actual) != (expected))                   \
              return return_value
#define EXPECT_EQ1(actual, expected, return_value)        \
            assert((actual) == (expected));               \
            HOPE_EQ1(actual, expected, return_value)

#define HOPE_NE0(v1, v2)                                  \
            if ((v1) == (v2))                             \
              return
#define EXPECT_NE0(v1, v2)                                \
            assert((v1) != (v2));                         \
            HOPE_NE0(v1, v2)
#define HOPE_NE1(v1, v2, return_value)                    \
            if ((v1) == (v2))                             \
              return return_value
#define EXPECT_NE1(v1, v2, return_value)                  \
            assert((v1) != (v2));                         \
            HOPE_NE1(v1, v2, return_value)

#define HOPE_LT0(v1, v2)                                  \
            if ((v1) >= (v2))                             \
              return
#define EXPECT_LT0(v1, v2)                                \
            assert((v1) < (v2));                          \
            HOPE_LT0(v1, v2)
#define HOPE_LT1(v1, v2, return_value)                    \
            if ((v1) >= (v2))                             \
              return return_value
#define EXPECT_LT1(v1, v2, return_value)                  \
            assert((v1) < (v2));                          \
            HOPE_LT1(v1, v2, return_value)

#define HOPE_LE0(v1, v2)                                  \
            if ((v1) > (v2))                              \
              return
#define EXPECT_LE0(v1, v2)                                \
            assert((v1) <= (v2));                         \
            HOPE_LE0(v1, v2)
#define HOPE_LE1(v1, v2, return_value)                    \
            if ((v1) > (v2))                              \
              return return_value
#define EXPECT_LE1(v1, v2, return_value)                  \
            assert((v1) <= (v2));                         \
            HOPE_LE1(v1, v2, return_value)

#define HOPE_GT0(v1, v2)                                  \
            if ((v1) <= (v2))                             \
              return
#define EXPECT_GT0(v1, v2)                                \
            assert((v1) > (v2));                          \
            HOPE_GT0(v1, v2)
#define HOPE_GT1(v1, v2, return_value)                    \
            if ((v1) <= (v2))                             \
              return return_value
#define EXPECT_GT1(v1, v2, return_value)                  \
            assert((v1) > (v2));                          \
            HOPE_GT1(v1, v2, return_value)

#define HOPE_GE0(v1, v2)                                  \
            if ((v1) < (v2))                              \
              return
#define EXPECT_GE0(v1, v2)                                \
            assert((v1) >= (v2));                         \
            HOPE_GE0(v1, v2)
#define HOPE_GE1(v1, v2, return_value)                    \
            if ((v1) < (v2))                              \
              return return_value
#define EXPECT_GE1(v1, v2, return_value)                  \
            assert((v1) >= (v2));                         \
            HOPE_GE1(v1, v2, return_value)

#define HOPE_SUCCESS0(result)                             \
            if ((result) < 0)                             \
              return
#define EXPECT_SUCCESS0(result)                           \
            assert((result) >= 0);                        \
            HOPE_SUCCESS0(result)
#define HOPE_SUCCESS1(result, return_value)               \
            if ((result) < 0)                             \
              return return_value
#define EXPECT_SUCCESS1(result, return_value)             \
            assert((result) >= 0);                        \
            HOPE_SUCCESS1(result, return_value)

#define HOPE_FAILED0(result)                              \
            if ((result) >= 0)                            \
              return
#define EXPECT_FAILED0(result)                            \
            assert((result) < 0);                         \
            HOPE_FAILED0(result)
#define HOPE_FAILED1(result, return_value)                \
            if ((result) >= 0)                            \
              return return_value
#define EXPECT_FAILED1(result, return_value)              \
            assert((result) < 0);                         \
            HOPE_FAILED1(result, return_value)
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Dùng để Debug => Attact To Process
#define SHOW(Contents)                (wcslen(Contents) == 19492)
#define TRACE(Contents)               assert(SHOW(L(Contents)))
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Chỉ dùng để kiểm tra input của hàm, kiểm tra kết quả trả về của hàm gọi (bool)
// condition là kết quả mong đợi
// return_value là giá trị trả về trong trường hợp kết quả không như mong đợi
//
// Kiểm tra điều kiện
#define CHECK0(condition)                             \
            if (!(condition))                         \
              return

#define DCHECK0(condition)                            \
            assert(condition);                        \
            CHECK0(condition)

#define CHECK1(condition, return_value)               \
            if (!(condition))                         \
              return return_value

#define DCHECK1(condition, return_value)              \
            assert(condition);                        \
            CHECK1(condition, return_value)

// Kiểm tra con trỏ
#define CHECK_POINTER0(lpPointer)                     \
            if ((lpPointer) == nullptr)               \
              return

#define DCHECK_POINTER0(lpPointer)                    \
            assert((lpPointer) != nullptr);           \
            CHECK_POINTER0(lpPointer)

#define CHECK_POINTER1(lpPointer, return_value)       \
            if ((lpPointer) == nullptr)               \
              return return_value

#define DCHECK_POINTER1(lpPointer, return_value)      \
            assert((lpPointer) != nullptr);           \
            CHECK_POINTER1(lpPointer, return_value)

// Kiểm tra string
#define CHECK_STRING0(lpString)                       \
            if ((lpString) == nullptr)                \
              return;                                 \
            if (strlen(lpString) == 0)                \
              return

#define DCHECK_STRING0(lpString)                      \
            assert((lpString) != nullptr);            \
            CHECK_STRING0(lpString)

#define CHECK_STRING1(lpString, return_value)         \
            if ((lpString) == nullptr)                \
              return return_value;                    \
            if (strlen(lpString) == 0)                \
              return return_value

#define DCHECK_STRING1(lpString, return_value)        \
            assert((lpString) != nullptr);            \
            assert(strlen(lpString) != 0);            \
            CHECK_STRING1(lpString, return_value)

// Kiểm tra Wire String
#define CHECK_WSTRING0(lpWString)                     \
            if ((lpWString) == nullptr)               \
              return;                                 \
            if (wcslen(lpWString) == 0)               \
              return

#define DCHECK_WSTRING0(lpWString)                    \
            assert((lpWString) != nullptr);           \
            assert(wcslen(lpWString) != 0);           \
            CHECK_WSTRING0(lpWString)

#define CHECK_WSTRING1(lpWString, return_value)       \
            if ((lpWString) == nullptr)               \
              return return_value;                    \
            if (wcslen(lpWString) == 0)               \
              return return_value

#define DCHECK_WSTRING1(lpWString, return_value)      \
            assert((lpWString) != nullptr);           \
            assert(wcslen(lpWString) != 0);           \
            CHECK_WSTRING1(lpWString, return_value)
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName)            \
            TypeName(const TypeName&);                \
            void operator=(const TypeName&)
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#ifndef _QWORD_DEFINED
#define _QWORD_DEFINED
typedef __int64 QWORD, *LPQWORD;
#endif

#define LO_WORD(x) ((BYTE)((WORD)(x) & 0xFF))
#define HI_WORD(x) ((BYTE)((WORD)(x) >> 8) & 0xFF)
#define LO_DWORD(x) ((WORD)((DWORD)(x) & 0xFFFF))
#define HI_DWORD(x) ((WORD)((DWORD)(x) >> 16) & 0xFFFF)
#define LO_QWORD(x) ((DWORD)((QWORD)(x) & 0xFFFFFFFF))
#define HI_QWORD(x) ((DWORD)((QWORD)(x) >> 32) & 0xFFFFFFFF)

#define MAKE_WORD(l, h) ((WORD)(((WORD)((l) & 0x00FF)) | ((WORD)((h) & 0x00FF) << 8)))
#define MAKE_DWORD(l, h) ((DWORD)(((DWORD)((l) & 0x0000FFFF)) | ((DWORD)((h) & 0x0000FFFF) << 16)))
#define MAKE_QWORD(l, h) ((QWORD)(((QWORD)((l) & 0x00000000FFFFFFFF)) | (((QWORD)((h) & 0x00000000FFFFFFFF)) << 32)))

#pragma warning(push)
#pragma warning(disable : 4201)
union ___uint8 {
  unsigned char number;
  struct {
    unsigned char bit0 : 1;  // bit thấp nhất
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
  };
};

union ___int8 {
  char number;
  struct {
    char bit0 : 1;  // bit thấp nhất
    char bit1 : 1;
    char bit2 : 1;
    char bit3 : 1;
    char bit4 : 1;
    char bit5 : 1;
    char bit6 : 1;
    char bit7 : 1;
  };
};

union ___uint16 {
  unsigned short number;
  struct {
    unsigned char byte_l;
    unsigned char byte_h;
  };
};

union ___int16 {
  short number;
  struct {
    char byte_l;
    char byte_h;
  };
};

union ___uint32 {
  unsigned long number;
  struct {
    unsigned char byte_ll;
    unsigned char byte_lh;
    unsigned char byte_hl;
    unsigned char byte_hh;
  };
};

union ___int32 {
  long number;
  struct {
    char byte_ll;
    char byte_lh;
    char byte_hl;
    char byte_hh;
  };
};

union ___uint64 {
  unsigned long long number;
  struct {
    unsigned char byte_lll;
    unsigned char byte_llh;
    unsigned char byte_lhl;
    unsigned char byte_lhh;
    unsigned char byte_hll;
    unsigned char byte_hlh;
    unsigned char byte_hhl;
    unsigned char byte_hhh;
  };
};

union ___int64 {
  long long number;
  struct {
    char byte_lll;
    char byte_llh;
    char byte_lhl;
    char byte_lhh;
    char byte_hll;
    char byte_hlh;
    char byte_hhl;
    char byte_hhh;
  };
};
#pragma warning(pop)
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#define ROTARE_LEFT(bits, number) \
    (((number) << (bits)) | ((number) >> ((sizeof(number) * 8) - (bits))))
#define ROTARE_RIGHT(bits, number)  \
    (((number) >> (bits)) | ((number) << ((sizeof(number) * 8) - (bits))))
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#define SERIALIZED_OBJECT_ON  __pragma(pack(push, 1))
#define SERIALIZED_OBJECT_OFF __pragma(pack(pop))
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#define Zero(obj) memset(&obj, 0, sizeof(obj))
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#define VALID_HANDLE(h) (h != NULL && h != INVALID_HANDLE_VALUE)
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
#endif  // ARIES_BASE_INCLUDE_MACRO_H
// -----------------------------------------------------------------------------
