#pragma once

#include <cstdint>

#define MAX_STRING_LENGHT 65536

enum D2ItemQuality {
  ITEMQUALITY_CRACKED = 1,
  ITEMQUALITY_NORMAL = 2,
  ITEMQUALITY_SUPERIOR = 3,
  ITEMQUALITY_MAGIC = 4,
  ITEMQUALITY_SET = 5,
  ITEMQUALITY_RARE = 6,
  ITEMQUALITY_UNIQUE = 7,
  ITEMQUALITY_CRAFTED = 8,
  ITEMQUALITY_TEMPERED = 9
};

struct ItemCode {
  char code0;
  char code1;
  char code2;
  char code3;
  bool qualityinclude[10];
};

struct ItemType {
  uint32_t dwtype;
  bool qualityinclude[10];
};

struct Recipe {
  ItemCode input1_code;
  ItemCode input2_code;
  ItemCode input3_code;
  ItemCode input4_code;
  ItemCode input5_code;
  ItemCode input6_code;
  ItemCode input7_code;
  ItemType input1_type;
  ItemType input2_type;
  ItemType input3_type;
  ItemType input4_type;
  ItemType input5_type;
  ItemType input6_type;
  ItemType input7_type;
  uint32_t repeat_count = 0;
  bool back_to_inventory = false;
  bool auto_transmute = false;
};
