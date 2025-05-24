#pragma once
#include <cstdint>

#define MAX_STRING_LENGHT 65536

enum D2ItemQuality
{
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

struct item_code {
	char code0 = 0;
	char code1 = 0;
	char code2 = 0;
	char code3 = 0;
	uint32_t qualityinclude[10] = { 0 };
};

struct item_type {
	uint32_t dwtype = 0;
	uint32_t qualityinclude[10] = { 0 };
};

struct recipe {
	item_code input1_code;
	item_code input2_code;
	item_code input3_code;
	item_code input4_code;
	item_code input5_code;
	item_code input6_code;
	item_code input7_code;
	item_type input1_type;
	item_type input2_type;
	item_type input3_type;
	item_type input4_type;
	item_type input5_type;
	item_type input6_type;
	item_type input7_type;
	uint32_t repeat_count = 0;
	bool back_to_inventory = false;
	bool auto_transmute = false;
};
