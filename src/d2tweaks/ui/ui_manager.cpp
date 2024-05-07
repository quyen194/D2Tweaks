#include <Windows.h>
#include <algorithm>
#include <chrono>
#include <common/hooking.h>
#include <d2tweaks/client/client.h>
#include <d2tweaks/client/modules/autosort/autosort_client.h>
#include <d2tweaks/common/asset_manager.h>
#include <d2tweaks/common/common.h>
#include <d2tweaks/common/protocol.h>
#include <d2tweaks/ui/controls/button.h>
#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>
#include <diablo2/d2client.h>
#include <diablo2/d2cmp.h>
#include <diablo2/d2common.h>
#include <diablo2/d2game.h>
#include <diablo2/d2gfx.h>
#include <diablo2/d2win.h>
#include <diablo2/structures/data/item_types_line.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/path.h>
#include <diablo2/structures/player_data.h>
#include <diablo2/structures/unit.h>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <spdlog/spdlog.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <time.h>
#include <unordered_map>
#include <vector>
#include <CommCtrl.h> // Include for edit control

// Define a static variable to keep track of the last time the stash window was toggled
static std::chrono::steady_clock::time_point lastToggleTime;

#pragma pack(push, 1)

using namespace std;

diablo2::structures::unit* g_item1;

static LRESULT(__stdcall* g_wnd_proc_original)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

d2_tweaks::ui::ui_manager::ui_manager(token) {
	hooking::hook(diablo2::d2_win::get_base() + 0xD9B0, wnd_proc, reinterpret_cast<void**>(&g_wnd_proc_original));
}

void d2_tweaks::ui::ui_manager::add_menu(menu* m) {
	if (m == nullptr)
		return;

	const auto it = std::find(m_menus.begin(), m_menus.end(), m);

	if (it != m_menus.end())
		return;

	m_menus.push_back(m);
}

d2_tweaks::ui::menu* d2_tweaks::ui::ui_manager::get_menu(const std::string& name) {
	if (name.empty())
		return nullptr;

	//TODO: optimize somehow
	for (auto menu : m_menus) {
		if (menu->get_name() == name)
			return menu;
	}

	return nullptr;
}

void d2_tweaks::ui::ui_manager::remove_menu(menu* m) {
	if (m == nullptr)
		return;

	m_menus.erase(std::remove(m_menus.begin(), m_menus.end(), m), m_menus.end());
}

void d2_tweaks::ui::ui_manager::draw() {
	//process_inputs();

	for (auto menu : m_menus) {
		if (!menu->get_visible())
			continue;

		menu->draw();
	}
}

const char* ITEMS_armor_and_weapons[] = {
	"hax", "axe", "2ax", "mpi", "wax", "lax", "bax", "btx", "gax", "gix",
	"wnd", "ywn", "bwn", "gwn", "clb", "scp", "gsc", "wsp", "spc", "mac",
	"mst", "fla", "whm", "mau", "gma", "ssd", "scm", "sbr", "flc", "crs",
	"bsd", "lsd", "wsd", "2hs", "clm", "gis", "bsw", "flb", "gsd", "dgr",
	"dir", "kri", "bld", "tkf", "tax", "bkf", "bal", "jav", "pil", "ssp",
	"glv", "tsp", "spr", "tri", "brn", "spt", "pik", "bar", "vou", "scy",
	"pax", "hal", "wsc", "sst", "lst", "cst", "bst", "wst", "sbw", "hbw",
	"lbw", "cbw", "sbb", "lbb", "swb", "lwb", "lxb", "mxb", "hxb", "rxb",
	"gps", "ops", "gpm", "opm", "gpl", "opl", "d33", "g33", "leg", "hdm",
	"hfh", "hst", "msf", "9ha", "9ax", "92a", "9mp", "9wa", "9la", "9ba",
	"9bt", "9ga", "9gi", "9wn", "9yw", "9bw", "9gw", "9cl", "9sc", "9qs",
	"9ws", "9sp", "9ma", "9mt", "9fl", "9wh", "9m9", "9gm", "9ss", "9sm",
	"9sb", "9fc", "9cr", "9bs", "9ls", "9wd", "92h", "9cm", "9gs", "9b9",
	"9fb", "9gd", "9dg", "9di", "9kr", "9bl", "9tk", "9ta", "9bk", "9b8",
	"9ja", "9pi", "9s9", "9gl", "9ts", "9sr", "9tr", "9br", "9st", "9p9",
	"9b7", "9vo", "9s8", "9pa", "9h9", "9wc", "8ss", "8ls", "8cs", "8bs",
	"8ws", "8sb", "8hb", "8lb", "8cb", "8s8", "8l8", "8sw", "8lw", "8lx",
	"8mx", "8hx", "8rx", "qf1", "qf2", "ktr", "wrb", "axf", "ces", "clw",
	"btl", "skr", "9ar", "9wb", "9xf", "9cs", "9lw", "9tw", "9qr", "7ar",
	"7wb", "7xf", "7cs", "7lw", "7tw", "7qr", "7ha", "7ax", "72a", "7mp",
	"7wa", "7la", "7ba", "7bt", "7ga", "7gi", "7wn", "7yw", "7bw", "7gw",
	"7cl", "7sc", "7qs", "7ws", "7sp", "7ma", "7mt", "7fl", "7wh", "7m7",
	"7gm", "7ss", "7sm", "7sb", "7fc", "7cr", "7bs", "7ls", "7wd", "72h",
	"7cm", "7gs", "7b7", "7fb", "7gd", "7dg", "7di", "7kr", "7bl", "7tk",
	"7ta", "7bk", "7b8", "7ja", "7pi", "7s7", "7gl", "7ts", "7sr", "7tr",
	"7br", "7st", "7p7", "7o7", "7vo", "7s8", "7pa", "7h7", "7wc", "6ss",
	"6ls", "6cs", "6bs", "6ws", "6sb", "6hb", "6lb", "6cb", "6s7", "6l7",
	"6sw", "6lw", "6lx", "6mx", "6hx", "6rx", "ob1", "ob2", "ob3", "ob4",
	"ob5", "am1", "am2", "am3", "am4", "am5", "ob6", "ob7", "ob8", "ob9",
	"oba", "am6", "am7", "am8", "am9", "ama", "obb", "obc", "obd", "obe",
	"obf", "amb", "amc", "amd", "ame", "amf", "cap", "skp", "hlm", "fhl",
	"ghm", "crn", "msk", "qui", "lea", "hla", "stu", "rng", "scl", "chn",
	"brs", "spl", "plt", "fld", "gth", "ful", "aar", "ltp", "buc", "sml",
	"lrg", "kit", "tow", "gts", "lgl", "vgl", "mgl", "tgl", "hgl", "lbt",
	"vbt", "mbt", "tbt", "hbt", "lbl", "vbl", "mbl", "tbl", "hbl", "bhm",
	"bsh", "spk", "xap", "xkp", "xlm", "xhl", "xhm", "xrn", "xsk", "xui",
	"xea", "xla", "xtu", "xng", "xcl", "xhn", "xrs", "xpl", "xlt", "xld",
	"xth", "xul", "xar", "xtp", "xuc", "xml", "xrg", "xit", "xow", "xts",
	"xlg", "xvg", "xmg", "xtg", "xhg", "xlb", "xvb", "xmb", "xtb", "xhb",
	"zlb", "zvb", "zmb", "ztb", "zhb", "xh9", "xsh", "xpk", "dr1", "dr2",
	"dr3", "dr4", "dr5", "ba1", "ba2", "ba3", "ba4", "ba5", "pa1", "pa2",
	"pa3", "pa4", "pa5", "ne1", "ne2", "ne3", "ne4", "ne5", "ci0", "ci1",
	"ci2", "ci3", "uap", "ukp", "ulm", "uhl", "uhm", "urn", "usk", "uui",
	"uea", "ula", "utu", "ung", "ucl", "uhn", "urs", "upl", "ult", "uld",
	"uth", "uul", "uar", "utp", "uuc", "uml", "urg", "uit", "uow", "uts",
	"ulg", "uvg", "umg", "utg", "uhg", "ulb", "uvb", "umb", "utb", "uhb",
	"ulc", "uvc", "umc", "utc", "uhc", "uh9", "ush", "upk", "dr6", "dr7",
	"dr8", "dr9", "dra", "ba6", "ba7", "ba8", "ba9", "baa", "pa6", "pa7",
	"pa8", "pa9", "paa", "ne6", "ne7", "ne8", "ne9", "nea", "drb", "drc",
	"drd", "dre", "drf", "bab", "bac", "bad", "bae", "baf", "pab", "pac",
	"pad", "pae", "paf", "neb", "neg", "ned", "nee", "nef", "tor", "ooc",
	"eaq", "ebq", "ib1", "ib3",

	// demon keys/chests
	"dkr1", "dkr2", "dkr3", "dkr4", "dkr5", "da1", "db1", "dc1"
	// crafting items
	"afr", "afy", "afp", "af0", "afx", "afh", "afq", "afj", "aft", "afi",
	"afe", "afg", "afk", "ae7", "afn", "ae8", "afl", "ae9", "afv", "afz",
	"af3", "af1", "afm", "af5", "afc", "afo", "afw", "afs", "af2", "afd",
	"afb", "af4", "afu"
};

const char* ITEMS_gems_and_runes[] = {
	"gcv", "gcw", "gcg", "gcr", "gcb", "skc", "gcy", "gfv", "gfw", "gfg",
	"gfr", "gfb", "skf", "gfy", "gsv", "gsw", "gsg", "gsr", "gsb", "sku",
	"gsy", "gzv", "glw", "glg", "glr", "glb", "skl", "gly", "gpv", "gpw",
	"gpg", "gpr", "gpb", "skz", "gpy", "ib1", "r01", "r02", "r03", "r04",
	"r05", "r06", "r07", "r08", "r09", "r10", "r11", "r12", "r13", "r14",
	"r15", "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23", "r24",
	"r25", "r26", "r27", "r28", "r29", "r30", "r31", "r32", "r33", "ib2",
};

const char* ITEMS_Stones[] = {
	// stones
	"abc", "abd", "abe", "abf", "abg", "abh", "abi", "abj", "abk", "abl",
	"abm", "abn", "abo", "abp", "abq", "abr", "abs", "abt", "abu", "abv",
	"abw", "abx", "aby", "abz", "ab0", "ab1", "ab2", "ab3", "ab4", "ab5",
	"ab6", "ab7", "ab8", "ab9", "acb", "acd", "ace", "acf", "acg", "ach",
	"aci", "acj", "ack", "acl", "acm", "acn", "aco", "acp", "acq", "acr",
	"acs", "act", "acu", "acv", "acw", "acx", "acy", "acz", "ac0", "ac1",
	"ac2", "ac3", "ac4", "ac5", "ac6", "ac7", "ac8", "ac9", "adb", "adc",
	"ade", "adf", "adg", "adh", "adi", "adj", "adk", "adl", "adm", "adn",
	"ado", "adp", "adq", "adr", "ads", "adt", "adu", "adv", "adw", "adx",
	"ady", "adz", "ad0", "ad1", "ad2", "ad3", "ad4", "ad5", "ad6", "ad7",
	"ad8", "ad9", "aeb", "aec", "aed", "aef", "aeg", "aeh", "aei", "aej",
	"aek", "ael", "aem", "aen", "aeo", "aep", "aeq", "aer", "aes", "aet",
	"aeu", "aev", "aew", "aex", "aey", "aez", "ae0", "ae1", "ae2", "ae3",
	"ae4", "ae5", "ae6",
};

// Define a structure named GemType
struct GemType {
	// Comment: The number of chipped gems of this type
	int chippedCount;

	// Comment: The row number in properties.txt minus 3, representing the type of gem
	int rowID;
};

// Gem/Rune Extractors
static std::unordered_map<std::string, GemType> stoneTypes = {
};

// Gems/runes and their corresponding codes and properties.txt line numbers
static std::unordered_map<std::string, GemType> gemTypes = {
	{"gcv", {1, 382}},   // Chipped Amethyst
	{"gcw", {1, 383}},   // Chipped Diamond
	{"gcg", {1, 384}},   // Chipped Emerald
	{"gcr", {1, 381}},   // Chipped Ruby
	{"gcb", {1, 385}},   // Chipped Sapphire
	{"skc", {1, 387}},   // Chipped Skull
	{"gcy", {1, 386}},   // Chipped Topaz
	{"gfv", {3, 382}},   // Flawed Amethyst
	{"gfw", {3, 383}},   // Flawed Diamond
	{"gfg", {3, 384}},   // Flawed Emerald
	{"gfr", {3, 381}},   // Flawed Ruby
	{"gfb", {3, 385}},   // Flawed Sapphire
	{"skf", {3, 387}},   // Flawed Skull
	{"gfy", {3, 386}},   // Flawed Topaz
	{"gsv", {9, 382}},   // Amethyst
	{"gsw", {9, 383}},   // Diamond
	{"gsg", {9, 384}},   // Emerald`
	{"gsr", {9, 381}},   // Ruby
	{"gsb", {9, 385}},   // Sapphire
	{"sku", {9, 387}},   // Skull
	{"gsy", {9, 386}},   // Topaz
	{"gzv", {27, 382}},  // Flawless Amethyst
	{"glw", {27, 383}},  // Flawless Diamond
	{"glg", {27, 384}},  // Flawless Emerald
	{"glr", {27, 381}},  // Flawless Ruby
	{"glb", {27, 385}},  // Flawless Sapphire
	{"skl", {27, 387}},  // Flawless Skull
	{"gly", {27, 386}},  // Flawless Topaz
	{"gpv", {81, 382}},  // Perfect Amethyst
	{"gpw", {81, 383}},  // Perfect Diamond
	{"gpg", {81, 384}},  // Perfect Emerald
	{"gpr", {81, 381}},  // Perfect Ruby
	{"gpb", {81, 385}},  // Perfect Sapphire
	{"skz", {81, 387}},  // Perfect Skull
	{"gpy", {81, 386}},  // Perfect Topaz
	{"r01", {1, 388}},   // El Rune
	{"r02", {3, 388}},   // Eld Rune
	{"r03", {9, 388}},   // Tir Rune
	{"r04", {27, 388}},  // Nef Rune
	{"r05", {81, 388}},  // Eth Rune
	{"r06", {243, 388}}, // Ith Rune
	{"r07", {1, 389}},   // Tal Rune
	{"r08", {3, 389}},   // Ral Rune
	{"r09", {9, 389}},   // Ort Rune
	{"r10", {27, 389}},  // Thul Rune
	{"r11", {81, 389}},  // Amn Rune
	{"r12", {243, 389}}, // Sol Rune
	{"r13", {1, 390}},   // Shael Rune
	{"r14", {3, 390}},   // Dol Rune
	{"r15", {9, 390}},   // Hel Rune
	{"r16", {27, 390}},  // Io Rune
	{"r17", {81, 390}},  // Lum Rune
	{"r18", {243, 390}}, // Ko Rune
	{"r19", {1, 391}},   // Fal Rune
	{"r20", {3, 391}},   // Lem Rune
	{"r21", {9, 391}},   // Pul Rune
	{"r22", {27, 391}},  // Um Rune
	{"r23", {81, 3901}}, // Mal Rune
	{"r24", {243, 391}}, // Ist Rune
	{"r25", {1, 392}},   // Gul Rune
	{"r26", {3, 392}},   // Vex Rune
	{"r27", {9, 392}},   // Ohm Rune
	{"r28", {27, 392}},  // Lo Rune
	{"r29", {81, 392}},  // Sur Rune
	{"r30", {243, 392}}, // Ber Rune
	{"r31", {1, 393}},   // Jah Rune
	{"r32", {2, 393}},   // Cham Rune
	{"r33", {4, 393}},    // Zod Rune
	//stones
	{"abc", {1, 351}},
	{"abd", {2, 351}},
	{"abe", {4, 351}},
	{"abf", {8, 351}},
	{"abg", {16, 351}},
	{"abh", {32, 351}},
	{"abi", {64, 351}},
	{"abj", {128, 351}},
	{"abk", {256, 351}},
	{"abl", {512, 351}},
	{"abm", {1024, 351}},
	{"abn", {2048, 351}},
	{"abo", {4096, 351}},
	{"abp", {8192, 351}},
	{"abq", {16384, 351}},
	{"abr", {32768, 351}},
	{"abs", {65536, 351}},
	{"abt", {131072, 351}},
	{"abu", {262144, 351}},
	{"abv", {1, 352}},
	{"abw", {2, 352}},
	{"abx", {4, 352}},
	{"aby", {8, 352}},
	{"abz", {16, 352}},
	{"ab0", {32, 352}},
	{"ab1", {64, 352}},
	{"ab2", {128, 352}},
	{"ab3", {256, 352}},
	{"ab4", {512, 352}},
	{"ab5", {1024, 352}},
	{"ab6", {2048, 352}},
	{"ab7", {4096, 352}},
	{"ab8", {8192, 352}},
	{"ab9", {16384, 352}},
	{"acb", {32768, 352}},
	{"acd", {65536, 352}},
	{"ace", {131072, 352}},
	{"acf", {262144, 352}},
	{"acg", {1, 353}},
	{"ach", {2, 353}},
	{"aci", {4, 353}},
	{"acj", {8, 353}},
	{"ack", {16, 353}},
	{"acl", {32, 353}},
	{"acm", {64, 353}},
	{"acn", {128, 353}},
	{"aco", {256, 353}},
	{"acp", {512, 353}},
	{"acq", {1024, 353}},
	{"acr", {2048, 353}},
	{"acs", {4096, 353}},
	{"act", {8192, 353}},
	{"acu", {16384, 353}},
	{"acv", {32768, 353}},
	{"acw", {65536, 353}},
	{"acx", {131072, 353}},
	{"acy", {262144, 353}},
	{"acz", {1, 354}},
	{"ac0", {2, 354}},
	{"ac1", {4, 354}},
	{"ac2", {8, 354}},
	{"ac3", {16, 354}},
	{"ac4", {32, 354}},
	{"ac5", {64, 354}},
	{"ac6", {128, 354}},
	{"ac7", {256, 354}},
	{"ac8", {512, 354}},
	{"ac9", {1024, 354}},
	{"adb", {2048, 354}},
	{"adc", {4096, 354}},
	{"ade", {8192, 354}},
	{"adf", {16384, 354}},
	{"adg", {32768, 354}},
	{"adh", {65536, 354}},
	{"adi", {131072, 354}},
	{"adj", {262144, 354}},
	{"adk", {1, 355}},
	{"adl", {2, 355}},
	{"adm", {4, 355}},
	{"adn", {8, 355}},
	{"ado", {16, 355}},
	{"adp", {32, 355}},
	{"adq", {64, 355}},
	{"adr", {128, 355}},
	{"ads", {256, 355}},
	{"adt", {512, 355}},
	{"adu", {1024, 355}},
	{"adv", {2048, 355}},
	{"adw", {4096, 355}},
	{"adx", {8192, 355}},
	{"ady", {16384, 355}},
	{"adz", {32768, 355}},
	{"ad0", {65536, 355}},
	{"ad1", {131072, 355}},
	{"ad2", {262144, 355}},
	{"ad3", {1, 356}},
	{"ad4", {2, 356}},
	{"ad5", {4, 356}},
	{"ad6", {8, 356}},
	{"ad7", {16, 356}},
	{"ad8", {32, 356}},
	{"ad9", {64, 356}},
	{"aeb", {128, 356}},
	{"aec", {256, 356}},
	{"aed", {512, 356}},
	{"aef", {1024, 356}},
	{"aeg", {2048, 356}},
	{"aeh", {4096, 356}},
	{"aei", {8192, 356}},
	{"aej", {16384, 356}},
	{"aek", {32768, 356}},
	{"ael", {65536, 356}},
	{"aem", {131072, 356}},
	{"aen", {262144, 356}},
	{"aeo", {1, 357}},
	{"aep", {2, 357}},
	{"aeq", {4, 357}},
	{"aer", {8, 357}},
	{"aes", {16, 357}},
	{"aet", {32, 357}},
	{"aeu", {64, 357}},
	{"aev", {128, 357}},
	{"aew", {256, 357}},
	{"aex", {512, 357}},
	{"aey", {1024, 357}},
	{"aez", {2048, 357}},
	{"ae0", {4096, 357}},
	{"ae1", {8192, 357}},
	{"ae2", {16384, 357}},
	{"ae3", {32768, 357}},
	{"ae4", {65536, 357}},
	{"ae5", {131072, 357}},
	{"ae6", {262144, 357}},
};

// Gem/Rune Extractors
static std::unordered_map<std::string, GemType> exTypes = {
	{"g25", {-1, 381}},   // Chipped Amethyst
	{"g24", {-1, 383}},   // Chipped Diamond
	{"ge8", {-1, 384}},   // Chipped Emerald
	{"g22", {-1, 381}},   // Chipped Ruby
	{"g21", {-1, 382}},   // Chipped Sapphire
	{"g15", {-1, 384}},   // Chipped Skull
	{"g14", {-1, 383}},   // Chipped Topaz
	{"gb4", {-3, 378}},   // Flawed Amethyst
	{"g12", {-3, 383}},   // Flawed Diamond
	{"g11", {-3, 384}},   // Flawed Emerald
	{"g30", {-3, 381}},   // Flawed Ruby
	{"g29", {-3, 382}},   // Flawed Sapphire
	{"g28", {-3, 384}},   // Flawed Skull
	{"g27", {-3, 383}},   // Flawed Topaz
	{"g10", {-9, 378}},   // Amethyst
	{"ge9", {-9, 383}},   // Diamond
	{"g23", {-9, 384}},   // Emerald
	{"ge7", {-9, 381}},   // Ruby
	{"ge6", {-9, 382}},   // Sapphire
	{"g35", {-9, 384}},   // Skull
	{"gb5", {-9, 383}},   // Topaz
	{"g18", {-27, 378}},  // Flawless Amethyst
	{"g32", {-27, 383}},  // Flawless Diamond
	{"g31", {-27, 384}},  // Flawless Emerald
	{"ge5", {-27, 381}},  // Flawless Ruby
	{"ge4", {-27, 382}},  // Flawless Sapphire
	{"g13", {-27, 384}},  // Flawless Skull
	{"ge2", {-27, 383}},  // Flawless Topaz
	{"ge1", {-81, 378}},  // Perfect Amethyst
	{"g20", {-81, 383}},  // Perfect Diamond
	{"g19", {-81, 384}},  // Perfect Emerald
	{"ge3", {-81, 381}},  // Perfect Ruby
	{"g17", {-81, 382}},  // Perfect Sapphire
	{"g16", {-81, 384}},  // Perfect Skull

	{"x01", {-1, 388}},   // El Rune
	{"x02", {-3, 388}},   // Eld Rune
	{"x03", {-9, 388}},   // Tir Rune
	{"x04", {-27, 388}},  // Nef Rune
	{"x05", {-81, 388}},  // Eth Rune
	{"x06", {-243, 388}}, // Ith Rune
	{"x07", {-1, 389}},   // Tal Rune
	{"x08", {-3, 389}},   // Ral Rune
	{"x09", {-9, 389}},   // Ort Rune
	{"x10", {-27, 389}},  // Thul Rune
	{"x11", {-81, 389}},  // Amn Rune
	{"x12", {-243, 389}}, // Sol Rune
	{"x13", {-1, 390}},   // Shael Rune
	{"x14", {-3, 390}},   // Dol Rune
	{"x15", {-9, 390}},   // Hel Rune
	{"x16", {-27, 390}},  // Io Rune
	{"x17", {-81, 390}},  // Lum Rune
	{"x18", {-243, 390}}, // Ko Rune
	{"x19", {-1, 391}},   // Fal Rune
	{"x20", {-3, 391}},   // Lem Rune
	{"x21", {-9, 391}},   // Pul Rune
	{"x22", {-27, 391}},  // Um Rune
	{"x23", {-81, 3901}}, // Mal Rune
	{"x24", {-243, 391}}, // Ist Rune
	{"x25", {-1, 392}},   // Gul Rune
	{"x26", {-3, 392}},   // Vex Rune
	{"x27", {-9, 392}},   // Ohm Rune
	{"x28", {-27, 392}},  // Lo Rune
	{"x29", {-81, 392}},  // Sur Rune
	{"x30", {-243, 392}}, // Ber Rune
	{"x31", {-1, 393}},   // Jah Rune
	{"x32", {-2, 393}},   // Cham Rune
	{"x33", {-4, 393}}    // Zod Rune
};

bool isGemOrRuneCode(const char* normCode) {
	// Iterate over the gemRuneCodes array and check if normCode matches any of the codes
	for (const auto& code : ITEMS_gems_and_runes) {
		if (strncmp(normCode, code, 3) == 0) {
			return true;
		}
	}
	return false;
}

bool isArmorOrWeaponCode(const char* normCode) {
	// Iterate over the gemRuneCodes array and check if normCode matches any of the codes
	for (const auto& code : ITEMS_armor_and_weapons) {
		if (strncmp(normCode, code, 3) == 0) {
			return true;
		}
	}
	return false;
}

bool isStoneCode(const char* normCode) {
	// Iterate over the gemRuneCodes array and check if normCode matches any of the codes
	for (const auto& code : ITEMS_Stones) {
		if (strncmp(normCode, code, 3) == 0) {
			return true;
		}
	}
	return false;
}

LRESULT d2_tweaks::ui::ui_manager::wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static auto& instance = singleton<ui_manager>::instance();

	bool block;

	// Send transmute packet
	if (wParam == 'X') {
		diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);
		block = true; // block the game from processing this key
	}

	if (wParam == 'V') {
		// Define a cooldown duration in milliseconds
		constexpr int cooldownDuration = 500; // Adjust this value as needed

		// Get the current time
		auto currentTime = std::chrono::steady_clock::now();

		// Calculate the time elapsed since the last stash toggle
		auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastToggleTime).count();

		// Check if enough time has elapsed since the last toggle
		if (timeElapsed >= cooldownDuration) {
			// Update the last toggle time
			lastToggleTime = currentTime;

			if (!diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH)) {
				// Code to open stash
				const auto player = diablo2::d2_client::get_local_player();
				int32_t st0Guid = 0;
				uint32_t st0X = 0;
				uint32_t st0Y = 0;
				diablo2::structures::unit* box{};
				for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
					const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
					char* st0Code = record->string_code;
					if (strncmp(st0Code, "st0", 3) == 0) {
						box = item;
						st0Guid = box->guid;
						st0X = player->path->mapx;
						st0Y = player->path->mapy;
					}
				}
				struct D2GSPacketClt20 {
					uint8_t PacketId; // 0x01
					uint32_t guid;    // 0x06
					uint32_t tx;    // 0x07
					uint32_t ty;    // 0x09
				};
				D2GSPacketClt20 packet;
				packet.PacketId = 0x20;
				packet.guid = st0Guid;
				packet.tx = st0X;
				packet.ty = st0Y;
				diablo2::d2_client::send_to_server(&packet, sizeof packet);
				block = true;

				// MessageBoxA(NULL, "Stash opened", "Stash", MB_OK);
				// spdlog::info("Stash opened");
			}
			else {
				// Code to close stash
				// Close stash
				diablo2::d2_client::set_ui_toggle(0x19, 1, FALSE);
				// send to server7 to close cube packet 0x4F
				diablo2::d2_client::send_to_server_7(0x4F, 0x17, 0, 0);
				block = true;
			}
		}
	}

	// Send item move packet + transmute packet for certain codes only for runes and gems
	if (wParam == 'Z' || wParam == 'G') {
		// Call the item_click function using the function pointer
		const auto g_hoverItem = (*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4));

		if (g_hoverItem != 0) {
			char currentPage = diablo2::d2_common::get_item_page(g_hoverItem);

			// Create the packet
			static d2_tweaks::common::item_move_cs packet;
			packet.item_guid = g_hoverItem->guid;

			if (currentPage == 0) { //item is in inventory
				if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH))
					packet.target_page = 4;

				if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE))
					packet.target_page = 3;
			}
			else {
				packet.target_page = 0;
			}

			diablo2::d2_client::send_to_server(&packet, sizeof packet);
			(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
		}

		if (g_hoverItem != 0) {
			const auto record = diablo2::d2_common::get_item_record(g_hoverItem->data_record_index);
			char* normCode = record->string_code;

			// Iterate over the codes array and check if normCode matches any of the codes
			bool found = false;
			for (const auto& code : ITEMS_gems_and_runes) {
				if (strncmp(normCode, code, 3) == 0) {
					found = true;
					break;
				}
			}

			if (found) {
				char currentPage = diablo2::d2_common::get_item_page(g_hoverItem);

				// Create the packet
				static d2_tweaks::common::item_move_cs packet;
				packet.item_guid = g_hoverItem->guid;

				if (currentPage == 0) { //item is in inventory
					if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH))
						packet.target_page = 4;

					if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE))
						packet.target_page = 3;
				}
				else {
					packet.target_page = 0;
				}

				diablo2::d2_client::send_to_server(&packet, sizeof packet);
				(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
				if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE))
					diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);
			}
		}

		block = true; // block the game from processing this key
	}

	switch (msg) {
	case WM_LBUTTONDOWN:
	{
		block = instance.process_left_mouse(false);
		break;
	}

	case WM_LBUTTONUP:
	{
		block = instance.process_left_mouse(true);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		// Get the local player object from the Diablo 2 client
		const auto player = diablo2::d2_client::get_local_player();

		// Obtain a pointer to the player's inventory
		auto pInventory = player->inventory;

		// Initialize variables to store GUIDs and coordinates
		int32_t gemBagGuid = 0;
		int32_t harvesterGuid = 0;
		int32_t boxGuid;
		uint32_t boxX;
		uint32_t boxY;

		// Calculate the memory address of the hover item in the game
		// by adding an offset to the base address of the client
		const auto g_hoverItem = *reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4);

		if (g_hoverItem != nullptr) {
			// Obtain the item record associated with the hovered item
			const auto record = diablo2::d2_common::get_item_record(g_hoverItem->data_record_index);

			// Extract the string code of the item record
			char* normCode = record->string_code;

			// Retrieve the item type record based on the item record's type
			const auto itemtype_record = diablo2::d2_common::get_item_type_record(record->type);

			// Retrieve item type records corresponding to the equivalent types of the item
			auto itemtype_record_equiv1 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv1);
			auto itemtype_record_equiv2 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv2);

			// Initialize a vector to store items and pointers to specific items
			std::vector<diablo2::structures::unit*> items;
			diablo2::structures::unit* gemBag{};
			diablo2::structures::unit* box{};
			diablo2::structures::unit* harvester{};

			// Loop through the items in the player's inventory to find specific items
			for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
				// Get the item record associated with the current item
				const auto record = diablo2::d2_common::get_item_record(item->data_record_index);

				// Extract the string code of the item record
				char* normCode1 = record->string_code;

				// Check if the current item is a gem bag (type 101)
				if (record->type == 101) {
					gemBag = item; // Set gemBag pointer to the current item
					gemBagGuid = gemBag->guid; // Store the GUID of the gem bag
				}

				// Check if the current item's string code starts with "box"
				if (strncmp(normCode1, "box", 3) == 0) {
					box = item; // Set box pointer to the current item
					boxGuid = box->guid; // Store the GUID of the box
					boxX = player->path->mapx; // Store the player's X coordinate
					boxY = player->path->mapy; // Store the player's Y coordinate
				}

				// Check if the current item's string code starts with "ib3"
				if (strncmp(normCode1, "ib3", 3) == 0) {
					harvester = item; // Set harvester pointer to the current item
					harvesterGuid = harvester->guid; // Store the GUID of the harvester
				}
			}

			// Comment: Explanation of the relationship between the row number in properties.txt and the structure definition
			// The row number in properties.txt corresponds to the rowID property in the GemType structure, minus 3

			// Obtain the current page of the hovered item
			auto currentPage = diablo2::d2_common::get_item_page(g_hoverItem);

			// Declare a pointer to store the string key
			const char* key;

			// Check if the current page is the stash or inventory, right click on gem/rune will
			// add it to the cube and send it to page 99 which is non existent
			// (hope it doesn't bloat save file size and actualy disappears from memory)
			if (currentPage == 0 || currentPage == 4) {
				// Check if the stash or inventory window is open
				if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH) ||
					diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY)) {
					// Iterate through each gem type in the gemTypes map
					for (const auto& gem : gemTypes) {
						// Accessing the key and value of the gemTypes map
						const std::string& _key = gem.first;
						key = gem.first.c_str();

						const GemType& value = gem.second;

						// Check if the code of the hovered item matches the current gem type
						if (strncmp(normCode, key, 3) == 0) {
							// Create a D2PropertyStrc structure to represent the gem property
							D2PropertyStrc itemProperty = {};
							itemProperty.nProperty = value.rowID - 3; // Adjust the property ID
							itemProperty.nLayer = 0;
							itemProperty.nMin = value.chippedCount;
							itemProperty.nMax = value.chippedCount;

							// Add the gem property to the gem bag
							diablo2::d2_common::add_property(gemBag, &itemProperty, 0);

							// Play the drop sound associated with the hovered item
							diablo2::d2_client::play_sound(record->drop_sound, nullptr, 0, 0, 0);

							// Create and send a packet to the server to move the item
							static d2_tweaks::common::item_move_cs packet;
							packet.item_guid = g_hoverItem->guid;
							packet.item_code = key;
							packet.bag_guid = gemBagGuid;
							packet.updateBag = 1;
							packet.prop = itemProperty.nProperty;
							packet.val = itemProperty.nMin;
							packet.target_page = 99;
							diablo2::d2_client::send_to_server(&packet, sizeof packet);

							// Clear the hovered item after processing
							(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
						}
					}
				}
				// Check if the cube window is open and the current page is 3 (gen/rune is in cube)
				// move the item back to inventory
				else if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE) && currentPage == 3) {
					// Iterate through each gem type in the gemTypes map
					for (const auto& gem : gemTypes) {
						// Accessing the key and value of the gemTypes map
						const std::string& _key = gem.first;
						key = gem.first.c_str();

						const GemType& value = gem.second;

						// Check if the code of the hovered item matches the current gem type
						if (strncmp(normCode, key, 3) == 0) {
							// Move the item to the inventory
							char currentPage = diablo2::d2_common::get_item_page(g_hoverItem);

							// Create and send a packet to the server to move the item to the inventory
							static d2_tweaks::common::item_move_cs packet;
							packet.item_guid = g_hoverItem->guid;
							packet.target_page = 0;
							diablo2::d2_client::send_to_server(&packet, sizeof packet);

							// Clear the hovered item after processing
							(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
						}
					}
				}
				// Check if the cube window is open and the current page is 0 (gem/rune is in inventory)
				// on right click add it to the gembag and send the original gem/rune to page 99
				else if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE) && currentPage == 0) {
					// Iterate through each gem type in the gemTypes map
					for (const auto& gem : gemTypes) {
						// Accessing the key and value of the gemTypes map
						const std::string& _key = gem.first;
						key = gem.first.c_str();

						const GemType& value = gem.second;

						// Check if the code of the hovered item matches the current gem type
						if (strncmp(normCode, key, 3) == 0) {
							// Create a D2PropertyStrc structure to represent the gem property
							D2PropertyStrc itemProperty = {};
							itemProperty.nProperty = value.rowID - 3; // Adjust the property ID
							itemProperty.nLayer = 0;
							itemProperty.nMin = value.chippedCount;
							itemProperty.nMax = value.chippedCount;

							// Add the gem property to the gem bag
							diablo2::d2_common::add_property(gemBag, &itemProperty, 0);

							// Play the drop sound associated with the hovered item
							diablo2::d2_client::play_sound(record->drop_sound, nullptr, 0, 0, 0);

							// Create and send a packet to the server to move the item
							static d2_tweaks::common::item_move_cs packet;
							packet.item_guid = g_hoverItem->guid;
							packet.item_code = key;
							packet.bag_guid = gemBagGuid;
							packet.updateBag = 1;
							packet.prop = itemProperty.nProperty;
							packet.val = itemProperty.nMin;
							packet.target_page = 99;
							diablo2::d2_client::send_to_server(&packet, sizeof packet);

							// Clear the hovered item after processing
							(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
						}
					}
				}
			}

			// For gem/rune extractors, on right click, move the bag and extractor to the cube, send transmute packet
			// Check if the current page is the inventory, cube, or stash
			if (currentPage == 0 || currentPage == 3 || currentPage == 4) {
				// Check if the stash, cube, or inventory window is open
				if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH) ||
					diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE) ||
					diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY)) {
					// Iterate through each gem type in the exTypes map
					for (const auto& gem : exTypes) {
						// Accessing the key and value of the exTypes map
						const std::string& _key = gem.first;
						key = gem.first.c_str();

						const GemType& value = gem.second;

						// Check if the code of the hovered item matches the current gem type
						if (strncmp(normCode, key, 3) == 0) {
							// Open the cube
							struct D2GSPacketClt20 {
								uint8_t PacketId; // Packet identifier
								uint32_t guid;    // GUID of the cube item
								uint32_t tx;      // X-coordinate of the cube's location
								uint32_t ty;      // Y-coordinate of the cube's location
							};
							D2GSPacketClt20 D2GSPacketClt20;
							D2GSPacketClt20.PacketId = 0x20; // Packet ID for cube opening
							D2GSPacketClt20.guid = boxGuid; // GUID of the cube item
							D2GSPacketClt20.tx = player->path->mapx; // X-coordinate of the player's location
							D2GSPacketClt20.ty = player->path->mapy; // Y-coordinate of the player's location
							diablo2::d2_client::send_to_server(&D2GSPacketClt20, sizeof D2GSPacketClt20);

							// Wait for 100 milliseconds for the cube to open
							Sleep(100);

							// Get the current page of the hovered item
							char currentPage = diablo2::d2_common::get_item_page(g_hoverItem);

							// Create a packet to move the hovered item to the cube
							static d2_tweaks::common::item_move_cs packet;
							packet.item_guid = g_hoverItem->guid;
							packet.target_page = 3; // Target page for the cube
							diablo2::d2_client::send_to_server(&packet, sizeof packet);

							// Create a packet to move the gem bag to the cube
							static d2_tweaks::common::item_move_cs packetBag;
							packetBag.item_guid = gemBag->guid;
							packetBag.target_page = 3; // Target page for the cube
							diablo2::d2_client::send_to_server(&packetBag, sizeof packetBag);

							// Send a packet to activate the transmute button in the cube
							diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);

							// Clear the hovered item after processing
							(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
						}
					}
				}
			}

			// Move gems/runes from the cube to the inventory
			if (isGemOrRuneCode(normCode)) {
				// Get the local player object
				const auto player = diablo2::d2_client::get_local_player();

				// Loop through the items in the player's inventory
				for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
					// Get the current page of the item
					currentPage = diablo2::d2_common::get_item_page(item);

					// Check if the item is in the cube (page 3)
					if (currentPage == 3) {
						// Get the item record associated with the item
						const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
						char* normCode = record->string_code;

						// If the item code starts with "ib1", skip sending the item back to inventory
						if (strncmp(normCode, "ib1", 3) == 0) {
							continue; // Skip to the next iteration of the loop
						}
						else {
							// Create a packet to move the item from the cube to the inventory
							static d2_tweaks::common::item_move_cs movePacket;
							movePacket.item_guid = item->guid;
							movePacket.target_page = 0; // Move to inventory
							diablo2::d2_client::send_to_server(&movePacket, sizeof movePacket);
						}
					}
				}
			}

			// For armor and weapon codes, right click will move the item to the stash or cube and transmute
			if (isArmorOrWeaponCode(normCode)
				|| record->type == 61 - 3 // jewel
				|| record->type == 43 - 3 // key
				|| record->type == 109
				|| record->type == 111
				|| record->type == 112
				|| record->type == 113
				|| record->type == 120
				|| record->type == 113
				|| record->type == 122
				|| record->type == 123
				|| record->type == 123 - 3 // sum0
				|| record->type == 125
				|| record->type == 126 - 3
				|| record->type == 128 - 3
				|| record->type == 4 - 3
				|| record->type == 5 - 3
				|| record->type == 17 - 3
				|| record->type == 18 - 3
				|| record->type == 21 - 3
				|| record->type == 26 - 3
				|| record->type == 27 - 3
				|| record->type == 28 - 3
				|| record->type == 29 - 3
				|| record->type == 30 - 3
				|| record->type == 31 - 3
				|| record->type == 32 - 3) {
				// Create a packet to move an item
				static d2_tweaks::common::item_move_cs packet;

				// Set the GUID of the item to be moved
				packet.item_guid = g_hoverItem->guid;

				// Check if the item is currently in the inventory (page 0)
				if (currentPage == 0) {
					// If the inventory is open and stash is open, set target page to stash (page 4)
					if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH))
						packet.target_page = 4;

					// If the inventory is open and cube is open, set target page to cube (page 3)
					if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE))
						packet.target_page = 3;
				}
				// If the item is not in the inventory, set target page to inventory (page 0)
				else {
					packet.target_page = 0;
				}

				// Send the item move packet to the server
				diablo2::d2_client::send_to_server(&packet, sizeof packet);

				// Send a packet to activate the transmute button in the cube
				// diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);

				// Clear the hovered item after processing
				(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
			}
		}

		block = instance.process_right_mouse(false);
		break;
	}

	case WM_RBUTTONUP:
	{
		block = instance.process_right_mouse(true);
		break;
	}

	// Handle the WM_MOUSEWHEEL message
	case WM_MOUSEWHEEL:
	{
		// Extract the distance the wheel is rotated from the message
		short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		// Check if the wheel is scrolled upwards
		if (zDelta > 0) {
			// Process the mouse wheel input by scrolling upwards

			// Calculate the memory address of the hover item in the game
			// by adding an offset to the base address of the client
			const auto g_hoverItem = *reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4);

			// Get the local player object from the Diablo 2 client
			const auto player = diablo2::d2_client::get_local_player();

			// Obtain a pointer to the player's inventory
			auto pInventory = player->inventory;

			if (g_hoverItem != nullptr) {
				// Obtain the item record associated with the hovered item
				const auto record = diablo2::d2_common::get_item_record(g_hoverItem->data_record_index);

				// Extract the string code of the item record
				char* normCode = record->string_code;

				// Create the packet
				static d2_tweaks::common::item_move_cs packet;
				packet.item_guid = g_hoverItem->guid;
				packet.target_page = 0;
				packet.tmog = 1;
				packet.item_code = normCode;
				diablo2::d2_client::send_to_server(&packet, sizeof packet);
			}
			block = instance.process_mouse_wheel(true);
		}
		// Check if the wheel is scrolled downwards
		else if (zDelta < 0) {
			// Process the mouse wheel input by scrolling downwards

			// Calculate the memory address of the hover item in the game
			// by adding an offset to the base address of the client
			const auto g_hoverItem = *reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4);

			// Get the local player object from the Diablo 2 client
			const auto player = diablo2::d2_client::get_local_player();

			// Obtain a pointer to the player's inventory
			auto pInventory = player->inventory;

			if (g_hoverItem != nullptr) {
				// Obtain the item record associated with the hovered item
				const auto record = diablo2::d2_common::get_item_record(g_hoverItem->data_record_index);

				// Extract the string code of the item record
				char* normCode = record->string_code;

				// Create the packet
				static d2_tweaks::common::item_move_cs packet;
				packet.item_guid = g_hoverItem->guid;
				packet.target_page = 0;
				packet.tmog = 1;
				packet.item_code = normCode;
				diablo2::d2_client::send_to_server(&packet, sizeof packet);
			}
			block = instance.process_mouse_wheel(true);
		}
		break;
	}

	case WM_MBUTTONUP:
	{
		block = instance.process_middle_mouse(true);
		break;
	}

	case WM_MBUTTONDOWN:
	{
		// Define a cooldown duration in milliseconds
		constexpr int cooldownDuration = 500; // Adjust this value as needed

		// Get the current time
		auto currentTime = std::chrono::steady_clock::now();

		// Calculate the time elapsed since the last stash toggle
		auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastToggleTime).count();

		// Get the local player object from the Diablo 2 client
		const auto player = diablo2::d2_client::get_local_player();

		// Obtain a pointer to the player's inventory
		auto pInventory = player->inventory;

		// Initialize variables to store GUIDs and coordinates
		int32_t gemBagGuid = 0;
		int32_t harvesterGuid = 0;
		int32_t boxGuid;
		uint32_t boxX;
		uint32_t boxY;

		// Calculate the memory address of the hover item in the game
		// by adding an offset to the base address of the client
		const auto g_hoverItem = *reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4);

		if (g_hoverItem != nullptr) {
			// Obtain the item record associated with the hovered item
			const auto record = diablo2::d2_common::get_item_record(g_hoverItem->data_record_index);

			// Extract the string code of the item record
			char* normCode = record->string_code;

			// Retrieve the item type record based on the item record's type
			const auto itemtype_record = diablo2::d2_common::get_item_type_record(record->type);

			// Retrieve item type records corresponding to the equivalent types of the item
			auto itemtype_record_equiv1 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv1);
			auto itemtype_record_equiv2 = diablo2::d2_common::get_item_type_record(itemtype_record->equiv2);

			// Initialize a vector to store items and pointers to specific items
			std::vector<diablo2::structures::unit*> items;
			diablo2::structures::unit* gemBag{};
			diablo2::structures::unit* box{};
			diablo2::structures::unit* harvester{};


			// get the gembag item
			for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
				const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
				char* normCode1 = record->string_code;
				if (record->type == 101) {
					gemBag = item;
					gemBagGuid = gemBag->guid;
				}
				if (strncmp(normCode1, "box", 3) == 0) {
					box = item;
					boxGuid = box->guid;
					boxX = player->path->mapx;
					boxY = player->path->mapy;
				}
				if (strncmp(normCode1, "ib3", 3) == 0) {
					harvester = item;
					harvesterGuid = harvester->guid;
				}
			}

			if (strncmp(normCode, "rvs", 3) == 0) {
				// Create the packet
				static d2_tweaks::common::item_move_cs packet;
				packet.bag_guid = gemBagGuid;
				packet.updateBag = 1;
				packet.iCode = 'rvs ';
				packet.prop = 396 - 3;
				packet.val = 1;
				packet.item_guid = g_hoverItem->guid;
				packet.target_page = 99;
				diablo2::d2_client::send_to_server(&packet, sizeof packet);

				D2PropertyStrc itemProperty = {};
				itemProperty.nProperty = 396 - 3; // Adjust the property ID
				itemProperty.nLayer = 0;
				itemProperty.nMin = 1;
				itemProperty.nMax = 1;
				diablo2::d2_common::add_property(gemBag, &itemProperty, 0);
			}
			if (strncmp(normCode, "rvl", 3) == 0) {
				// Create the packet
				static d2_tweaks::common::item_move_cs packet;
				packet.bag_guid = gemBagGuid;
				packet.updateBag = 1;
				packet.iCode = 'rvl ';
				packet.prop = 396 - 3;
				packet.val = 3;
				packet.item_guid = g_hoverItem->guid;
				packet.target_page = 99;
				diablo2::d2_client::send_to_server(&packet, sizeof packet);

				D2PropertyStrc itemProperty = {};
				itemProperty.nProperty = 396 - 3; // Adjust the property ID
				itemProperty.nLayer = 0;
				itemProperty.nMin = 3;
				itemProperty.nMax = 3;
				diablo2::d2_common::add_property(gemBag, &itemProperty, 0);
			}

			if (isArmorOrWeaponCode(normCode)
				|| record->type == 61 - 3 // jewel
				|| record->type == 43 - 3 // key
				|| record->type == 109
				|| record->type == 111
				|| record->type == 112
				|| record->type == 113
				|| record->type == 120
				|| record->type == 113
				|| record->type == 122
				|| record->type == 123
				|| record->type == 123 - 3 // sum0
				|| record->type == 125
				|| record->type == 126 - 3
				|| record->type == 128 - 3
				|| record->type == 4 - 3
				|| record->type == 5 - 3
				|| record->type == 17 - 3
				|| record->type == 18 - 3
				|| record->type == 21 - 3
				|| record->type == 26 - 3
				|| record->type == 27 - 3
				|| record->type == 28 - 3
				|| record->type == 29 - 3
				|| record->type == 30 - 3
				|| record->type == 31 - 3
				|| record->type == 32 - 3) {
				char currentPage;

				// open the cube
				struct D2GSPacketClt20 {
					uint8_t PacketId; // 0x01
					uint32_t guid;	// 0x06
					uint32_t tx;	// 0x07
					uint32_t ty;	// 0x09
				};
				D2GSPacketClt20 D2GSPacketClt20;
				D2GSPacketClt20.PacketId = 0x20;
				D2GSPacketClt20.guid = boxGuid;
				D2GSPacketClt20.tx = player->path->mapx;
				D2GSPacketClt20.ty = player->path->mapy;
				diablo2::d2_client::send_to_server(&D2GSPacketClt20, sizeof D2GSPacketClt20);

				Sleep(100);

				// Check if enough time has elapsed since the last toggle
				if (timeElapsed >= cooldownDuration) {
					// Update the last toggle time
					lastToggleTime = currentTime;

					// now move the harvester guid to the cube
					// Create the packet
					static d2_tweaks::common::item_move_cs hpacket;
					hpacket.item_guid = harvesterGuid;
					hpacket.target_page = 3;
					diablo2::d2_client::send_to_server(&hpacket, sizeof hpacket);

					currentPage = diablo2::d2_common::get_item_page(g_hoverItem);
					// Create the packet
					static d2_tweaks::common::item_move_cs packet;
					packet.item_guid = g_hoverItem->guid;
					if (currentPage == 0) { //item is in inventory
						if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH))
							packet.target_page = 4;
						if (diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE))
							packet.target_page = 3;
					}
					else {
						packet.target_page = 0;
					}
					diablo2::d2_client::send_to_server(&packet, sizeof packet);

					diablo2::d2_client::send_to_server_7(0x4F, 0x18, 0, 0);

					// now move the harvester back to the inv
					//static d2_tweaks::common::item_move_cs h1packet;
					//h1packet.item_guid = harvesterGuid;
					//h1packet.target_page = 0;
					//diablo2::d2_client::send_to_server(&h1packet, sizeof h1packet);

					(*reinterpret_cast<diablo2::structures::unit**>(diablo2::d2_client::get_base() + 0x1158F4)) = nullptr;
				}
			}
		}

		block = instance.process_middle_mouse(false);
		break;
	}

	// Handling Windows messages related to key presses and releases.
	// These cases deal with key down and key up events.

	// When a system key down or key down message is received,
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		// Call the process_key_event function to handle the key down event.
		// Pass wParam (the virtual-key code) and false (indicating it's a key down event) as parameters.
		block = instance.process_key_event(wParam, false);
		break;
	}

	// When a system key up or key up message is received,
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		// Call the process_key_event function to handle the key up event.
		// Pass wParam (the virtual-key code) and true (indicating it's a key up event) as parameters.
		block = instance.process_key_event(wParam, true);
		break;
	}

	// If the message is not related to key events, return the original window procedure.
	default:
		// Call the original window procedure to handle the message.
		// Pass the window handle (hWnd), the message (msg), wParam, and lParam.
		return g_wnd_proc_original(hWnd, msg, wParam, lParam);
	}

	if (block)
		return 0;

	return g_wnd_proc_original(hWnd, msg, wParam, lParam);
}
// This function, belonging to the 'ui_manager' class within the 'ui' namespace of the 'd2_tweaks' namespace, is responsible for processing user inputs.
void d2_tweaks::ui::ui_manager::process_inputs() {
	// Check if the left mouse button is pressed asynchronously.
	if (GetAsyncKeyState(VK_LBUTTON)) {
		// If the left mouse button was not previously pressed (indicating a new press event),
		if (!m_was_down_before_left) {
			// Set the flag indicating that the left mouse button was pressed before.
			m_was_down_before_left = true;
			// Set the state of the left mouse button to true (pressed).
			m_mouse_state_left = true;

			// Call the function to process the left mouse button event with the parameter indicating whether it's an up event (false for down event).
			process_left_mouse(false);
		}
	}
	// If the left mouse button is not currently pressed,
	else if (m_was_down_before_left) {
		// Reset the flag indicating that the left mouse button was pressed before.
		m_was_down_before_left = false;
		// Set the state of the left mouse button to false (not pressed).
		m_mouse_state_left = false;

		// Call the function to process the left mouse button event with the parameter indicating it's an up event (true for up event).
		process_left_mouse(true);
	}

	// Check if the right mouse button is pressed asynchronously.
	if (GetAsyncKeyState(VK_RBUTTON)) {
		// If the right mouse button was not previously pressed (indicating a new press event),
		if (!m_was_down_before_right) {
			// Set the flag indicating that the right mouse button was pressed before.
			m_was_down_before_right = true;
			// Set the state of the right mouse button to true (pressed).
			m_mouse_state_right = true;

			// Call the function to process the right mouse button event with the parameter indicating whether it's an up event (false for down event).
			process_right_mouse(false);
		}
	}
	// If the right mouse button is not currently pressed,
	else if (m_was_down_before_right) {
		// Reset the flag indicating that the right mouse button was pressed before.
		m_was_down_before_right = false;
		// Set the state of the right mouse button to false (not pressed).
		m_mouse_state_right = false;

		// Call the function to process the right mouse button event with the parameter indicating it's an up event (true for up event).
		process_right_mouse(true);
	}
}

// This function processes the left mouse button event, either up or down.
bool d2_tweaks::ui::ui_manager::process_left_mouse(bool up) {
	// Initialize a flag to determine if the event was handled or not.
	auto block = false;

	// Iterate through all menus in the list of menus.
	for (auto menu : m_menus) {
		// If the menu is not enabled, skip to the next one.
		if (!menu->get_enabled())
			continue;

		// Call the left mouse button event handler of the current menu and update the block flag.
		block |= menu->left_mouse(up);
	}

	// Return the final block flag indicating whether the event was handled or not.
	return block;
}

// Similar to the previous function, this one processes the middle mouse button event.
bool d2_tweaks::ui::ui_manager::process_middle_mouse(bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->middle_mouse(up);
	}

	return block;
}

// This function processes the mouse wheel event.
bool d2_tweaks::ui::ui_manager::process_mouse_wheel(bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->mouse_wheel(up);
	}

	return block;
}

// This function processes the right mouse button event, either up or down.
bool d2_tweaks::ui::ui_manager::process_right_mouse(bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->right_mouse(up);
	}

	return block;
}

// This function processes key events (e.g., keyboard keys pressed or released).
bool d2_tweaks::ui::ui_manager::process_key_event(uint32_t key, bool up) {
	auto block = false;

	for (auto menu : m_menus) {
		if (!menu->get_enabled())
			continue;

		block |= menu->key_event(key, up);
	}

	return block;
}

#pragma pack(pop)