#include <diablo2/d2game.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/game_server.h>
#include <diablo2/structures/unit.h>
#include <diablo2/structures/npc_record.h>
#include <common/ptr_wrapper.h>

DWORD D2GAME_BASE = 0x6FC30000;

DWORD D2GAME_GetOffset(DWORD offset) {
  return offset - D2GAME_BASE;
}

namespace diablo2 {

using namespace structures;

char* d2_game::get_base() {
  static auto base = reinterpret_cast<char*>(GetModuleHandle("d2game.dll"));
  return base;
}

void d2_game::enqueue_packet(NetClient* client, void* packet, size_t size) {
  static wrap_func_fast<void(NetClient*, void*, size_t)> enqueue_packet(
      0xC710, get_base());
  enqueue_packet(client, packet, size);
}

NpcRecord* d2_game::get_npc_record(Game* pGame, Unit* npc, Unit** ptnpc) {
  static wrap_func_fast<NpcRecord*(Game*, Unit*, Unit**)> get_npc_record(
      0x9B910, get_base());
  return get_npc_record(pGame, npc, ptnpc);
}

void d2_game::free_gamble(Game* pGame,
                          Unit* player,
                          Unit* npc,
                          NpcRecord* npcrecord) {
  static wrap_func_fast<void(Game*, Unit*, Unit*, NpcRecord*)> free_gamble(
      0x9D190, get_base());
  free_gamble(pGame, player, npc, npcrecord);
}

void d2_game::fill_gamble(Game* pGame,
                          Unit* player,
                          Unit* npc,
                          NpcRecord* npcrecord) {
  static wrap_func_fast<void(Game*, Unit*, Unit*, NpcRecord*)> fill_gamble(
      0x9A9F0, get_base());
  fill_gamble(pGame, player, npc, npcrecord);
}

void d2_game::create_vendor_cache1(
    Game* pGame, Unit* player, Unit* npc, uint32_t param, bool bGamble) {
  static wrap_func_fast<void(Game*, Unit*, Unit*, uint32_t, bool)>
      create_vendor_cache1(0x974F0, get_base());
  create_vendor_cache1(pGame, player, npc, param, bGamble);
}

void d2_game::create_vendor_cache2(
    Game* pGame, Unit* pPlayer, Unit* pNpc, uint32_t param, bool bGamble) {
  static wrap_func_fast<void(Game*, Unit*, Unit*, uint32_t, bool)>
      create_vendor_cache2(0x9AE20, get_base());
  create_vendor_cache2(pGame, pPlayer, pNpc, param, bGamble);
}

Unit* d2_game::get_server_unit(Game* pGame,
                               unit_type_t type,
                               uint32_t uniqueid) {
  static wrap_func_fast<Unit*(Game*, unit_type_t, uint32_t)> get_server_unit(
      0x8BB00, get_base());
  return get_server_unit(pGame, type, uniqueid);
}

Game* d2_game::get_game_from_client_id(int32_t id) {
  static wrap_func_fast<Game*(int32_t)> get_game_from_client_id(0x94E0,
                                                                get_base());
  return get_game_from_client_id(id);
}

// __fastcall D2CLIENTS_GetClientByClientNumber(pGame, int nNumber)
NetClient* d2_game::get_net_client_from_id(Game* pGame, int32_t id) {
  static wrap_func_fast<NetClient*(Game*, int32_t)> get_net_client_from_id(
      0x1DE0, get_base());
  return get_net_client_from_id(pGame, id);
}

NetClient* d2_game::get_net_client_from_id_2(Game* pGame, int32_t id) {
  static wrap_func_fast<NetClient*(Game*, int32_t)> get_net_client_from_id_2(
      0x37B0, get_base());
  return get_net_client_from_id_2(pGame, id);
}

Unit* d2_game::get_player_pet(Game* pGame,
                              Unit* pUnit,
  uint32_t type, uint32_t index) {
  static wrap_func_fast<Unit*(Game*, Unit*, uint32_t, uint32_t)> get_player_pet(
      0x4E8B0, get_base());
  return get_player_pet(pGame, pUnit, type, index);
}

int32_t d2_game::identify_item(Game* pGame, Unit* player, Unit* item) {
  static wrap_func_fast<int32_t(Game*, Unit*, Unit*)> identify_item(0x19670,
                                                                    get_base());
  return identify_item(pGame, player, item);
}

int32_t d2_game::pickup_gold_pile(Game* pGame, Unit* pUnit, Unit* pItem) {
  static wrap_func_fast<int32_t(Game*, Unit*, Unit*)> pickup_gold_pile(
      0x12DD0, get_base());
  return pickup_gold_pile(pGame, pUnit, pItem);
}

bool __fastcall d2_game::pickup_item(Game* pGame,
                                     Unit* pPlayer,
                                     uint32_t guid,
                                     uint32_t* ptrNull) {
  static wrap_func_fast<bool(Game*, Unit*, uint32_t, uint32_t*)> pickup_item(
      0x13340, get_base());
  return pickup_item(pGame, pPlayer, guid, ptrNull);
}

void d2_game::update_inventory_items(Game* pGame, Unit* player) {
  static wrap_func_fast<void(Game*, Unit*, uint32_t)> update_inventory_items(
      0x14A90, get_base());
  update_inventory_items(pGame, player, 0);
}

uint32_t* d2_game::get_game_id_array_begin() {
  static wrap_value<uint32_t> game_id_array_begin(0x1147F8, get_base());
  return game_id_array_begin;
}

uint32_t* d2_game::get_game_id_array_end() {
  static wrap_value<uint32_t> game_id_array_end(0x1157F8, get_base());
  return game_id_array_end;
}

GameServer* d2_game::get_game_server() {
  static wrap_value<GameServer*> game_server(0x115818, get_base());
  return *game_server;
}

static Game*(__thiscall* g_get_game)(GameServer* gs, uint32_t gameId, void*) =
    decltype(g_get_game)(0xB6A0 + d2_game::get_base());

Game* d2_game::get_game(GameServer* gs, uint32_t gameId) {
  static wrap_func_std<Game*(uint32_t, void*)> get_game(0xB6A0, get_base());
  return g_get_game(gs, gameId, reinterpret_cast<char*>(gs) + 0x44);
}

Unit* d2_game::get_unit_owner(Game* pGame, Unit* pUnit) {
  static wrap_func_fast<Unit*(Game*, Unit*)> get_unit_owner(0x8BB70,
                                                            get_base());
  return get_unit_owner(pGame, pUnit);
}

static void __fastcall unit_pet_iterator(Game* pGame,
                                         Unit* owner,
                                         Unit* pUnit,
                                         void* arg) {
  const std::function<void(Game*, Unit*, Unit*)>* cb =
      reinterpret_cast<decltype(cb)>(arg);
  cb->operator()(pGame, owner, pUnit);
}

void* d2_game::iterate_unit_pets(
    Game* pGame,
    Unit* pUnit,
    const std::function<void(Game*, Unit*, Unit*)>& cb) {
  static wrap_func_fast<void*(
      Game*, Unit*, void(__fastcall*)(Game*, Unit*, Unit*, void*), void*)>
      iterate_unit_pets(0x4E7C0, get_base());
  const auto cbref = &cb;
  // ReSharper disable once CppCStyleCast
  return iterate_unit_pets(pGame, pUnit, unit_pet_iterator, (void*) cbref);
}

uint32_t __fastcall d2_game::transmogrify(Game* pGame, Unit* pPlayer) {
  static wrap_func_fast<uint32_t(Game*, Unit*)> transmogrify(0x62130,
                                                             get_base());
  return transmogrify(pGame, pPlayer);
}

// d2game:$0x60010
//int __fastcall CRAFT_Transmogrify(D2GameStrc* pGame,
//                                  D2UnitStrc* pPlayer,
//                                  D2CubemainTXT* pCubeTxt,
//                                  void* pUnknown)

// Add a wrapper for the following function:
// Unit* __fastcall D2GAME_CreateItemEx_6FC4ED80(Game* pGame,
//                                               D2ItemDropStrc* pItemDrop,
//                                               int32_t a3);
Unit* __fastcall d2_game::D2GAME_CreateItemEx_6FC4ED80(
    Game* pGame, D2ItemDropStrc* pItemDrop, int32_t a3) {
  static wrap_func_fast<Unit*(Game*, D2ItemDropStrc*, int32_t)>
      D2GAME_CreateItemEx_6FC4ED80(0x1ed80, get_base());
  return D2GAME_CreateItemEx_6FC4ED80(pGame, pItemDrop, a3);
}

// Add a wrapper for the following function:
// D2Game.0x6FC4A660
// int32_t __fastcall D2GAME_Transmogrify_6FC4A660(D2GameStrc* pGame,
//                                                 D2UnitStrc* pUnit,
//                                                 D2UnitStrc* pItem);
int32_t __fastcall d2_game::D2GAME_Transmogrify_6FC4A660(Game* pGame,
                                                         Unit* pPlayer,
                                                         Unit* pItem) {
  static wrap_func_fast<int32_t(Game*, Unit*, Unit*)>
      D2GAME_Transmogrify_6FC4A660(0x1a660, get_base());
  return D2GAME_Transmogrify_6FC4A660(pGame, pPlayer, pItem);
}

// D2Game.0x6FC95DF0
// D2UnitStrc* __fastcall QUESTS_CreateItem(D2GameStrc* pGame,
//                                          D2UnitStrc* pPlayer,
//                                          uint32_t dwCode,
//                                          int32_t nLevel,
//                                          uint8_t nQuality,
//                                          int32_t bDroppable);
Unit* __fastcall d2_game::QUESTS_CreateItem(Game* pGame,
                                            Unit* pPlayer,
                                            uint32_t dwCode,
                                            int32_t nLevel,
                                            uint8_t nQuality,
                                            int32_t bDroppable) {
  static wrap_func_fast<Unit*(
      Game*, Unit*, uint32_t, int32_t, uint8_t, int32_t)>
      QUESTS_CreateItem(0x65DF0, get_base());
  return QUESTS_CreateItem(
      pGame, pPlayer, dwCode, nLevel, nQuality, bDroppable);
}

// Add wrapper for D2Game.0x6FC603D0
// D2MonPropTxt* __fastcall MONSTER_GetMonPropTxtRecord(int32_t nId) 
D2MonPropTxt* __fastcall d2_game::MONSTER_GetMonPropTxtRecord(int32_t nId) {
  static wrap_func_fast<D2MonPropTxt*(int32_t)> MONSTER_GetMonPropTxtRecord(
      0x303D0, get_base());
  return MONSTER_GetMonPropTxtRecord(nId);
}

// add wrapper for //D2Game.0x6FCBC900
// D2UnitStrc* __stdcall SUNIT_GetTargetUnit(D2GameStrc* pGame, D2UnitStrc* pUnit);
Unit* __stdcall d2_game::SUNIT_GetTargetUnit(Game* pGame, Unit* pUnit) {
  static wrap_func_std<Unit*(Game*, Unit*)> SUNIT_GetTargetUnit(0x8C900,
                                                                get_base());
  return SUNIT_GetTargetUnit(pGame, pUnit);
}

// D2Game.0x6FCF5B90
// D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId)
D2SkillsTxt* __fastcall d2_game::SKILLS_GetSkillsTxtRecord(int32_t nSkillId) {
  static wrap_func_fast<D2SkillsTxt*(int32_t)> SKILLS_GetSkillsTxtRecord(
      0xC5B90, get_base());
  return SKILLS_GetSkillsTxtRecord(nSkillId);
}

// D2Game.0x6FD15580
// int32_t __fastcall D2GAME_GetSummonIdFromSkill_6FD15580(D2UnitStrc* pUnit,
//                                                         int32_t bFromMonster,
//                                                         int32_t nSkillId,
//                                                         int32_t nSkillLevel,
//                                                         int32_t* pSpawnMode,
//                                                         int32_t* pX,
//                                                         int32_t* pY);
int32_t __fastcall d2_game::D2GAME_GetSummonIdFromSkill_6FD15580(
    Unit* pUnit,
    int32_t bFromMonster,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t* pSpawnMode,
    int32_t* pX,
    int32_t* pY) {
  static wrap_func_fast<int32_t(
      Unit*, int32_t, int32_t, int32_t, int32_t*, int32_t*, int32_t*)>
      D2GAME_GetSummonIdFromSkill_6FD15580(0xE5580, get_base());
  return D2GAME_GetSummonIdFromSkill_6FD15580(
      pUnit, bFromMonster, nSkillId, nSkillLevel, pSpawnMode, pX, pY);
}

// D2Game.0x6FD14430
// D2UnitStrc* __fastcall D2GAME_SummonPet_6FD14430(D2GameStrc* pGame,
//                                                  D2SummonArgStrc* pSummonArg);
Unit* __fastcall d2_game::D2GAME_SummonPet_6FD14430(
    Game* pGame, D2SummonArgStrc* pSummonArg) {
  static wrap_func_fast<Unit*(Game*, D2SummonArgStrc*)>
      D2GAME_SummonPet_6FD14430(0xE4430, get_base());
  return D2GAME_SummonPet_6FD14430(pGame, pSummonArg);
}

// D2Game.0x6FD0CB10
// int32_t __fastcall D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(
//     D2GameStrc* pGame,
//     D2UnitStrc* pUnit,
//     D2UnitStrc* pPet,
//     int32_t nPetLevelArg,
//     int32_t nSkillLevel);
int32_t __fastcall d2_game::D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(
    Game* pGame,
    Unit* pUnit,
    Unit* pPet,
    int32_t nPetLevelArg,
    int32_t nSkillLevel) {
  static wrap_func_fast<int32_t(Game*, Unit*, Unit*, int32_t, int32_t)>
      D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(D2GAME_GetOffset(0x6FD0CB10),
                                                get_base());
  return D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(
      pGame, pUnit, pPet, nPetLevelArg, nSkillLevel);
}

// D2Game.0x6FD0C530
// int32_t __fastcall D2GAME_SetSummonPassiveStats_6FD0C530(D2GameStrc* pGame,
//                                                          D2UnitStrc* pUnit,
//                                                          D2UnitStrc* pPet,
//                                                          int32_t nSkillId,
//                                                          int32_t nSkillLevel,
//                                                          int32_t nItemLevel);
// use D2GAME_GetOffset(0x6FD0C530)
int32_t __fastcall d2_game::D2GAME_SetSummonPassiveStats_6FD0C530(
    Game* pGame,
    Unit* pUnit,
    Unit* pPet,
    int32_t nSkillId,
    int32_t nSkillLevel,
    int32_t nItemLevel) {
  static wrap_func_fast<int32_t(Game*, Unit*, Unit*, int32_t, int32_t, int32_t)>
      D2GAME_SetSummonPassiveStats_6FD0C530(D2GAME_GetOffset(0x6FD0C530),
                                            get_base());
  return D2GAME_SetSummonPassiveStats_6FD0C530(
      pGame, pUnit, pPet, nSkillId, nSkillLevel, nItemLevel);
}

// D2Game.0x6FD0C2E0
// void __fastcall D2GAME_SetSummonResistance_6FD0C2E0(D2UnitStrc* pUnit,
//                                                     D2UnitStrc* pPet);
void __fastcall d2_game::D2GAME_SetSummonResistance_6FD0C2E0(Unit* pUnit,
                                                             Unit* pPet) {
  static wrap_func_fast<void(Unit*, Unit*)> D2GAME_SetSummonResistance_6FD0C2E0(
      D2GAME_GetOffset(0x6FD0C2E0), get_base());
  D2GAME_SetSummonResistance_6FD0C2E0(pUnit, pPet);
}

// D2Game.0x6FC3E200
// void __fastcall sub_6FC3E200(D2ClientStrc* pClient, D2UnitStrc* pUnit)
void __fastcall d2_game::sub_6FC3E200(NetClient* pClient, Unit* pUnit) {
  static wrap_func_fast<void(NetClient*, Unit*)> sub_6FC3E200(
      D2GAME_GetOffset(0x6FC3E200), get_base());
  sub_6FC3E200(pClient, pUnit);
}

// D2Game.0x6FC69F10
// D2UnitStrc* __fastcall D2GAME_SpawnMonster_6FC69F10(D2GameStrc* pGame,
//                                                     D2ActiveRoomStrc* pRoom,
//                                                     int32_t nX,
//                                                     int32_t nY,
//                                                     int32_t nMonsterId,
//                                                     int32_t nAnimMode,
//                                                     int32_t a7,
//                                                     int16_t nFlags);
Unit* __fastcall d2_game::D2GAME_SpawnMonster_6FC69F10(Game* pGame,
                                                       Room* pRoom,
                                                       int32_t nX,
                                                       int32_t nY,
                                                       int32_t nMonsterId,
                                                       int32_t nAnimMode,
                                                       int32_t a7,
                                                       int16_t nFlags) {
  static wrap_func_fast<Unit*(
      Game*, Room*, int32_t, int32_t, int32_t, int32_t, int32_t, int16_t)>
      D2GAME_SpawnMonster_6FC69F10(D2GAME_GetOffset(0x6FC69F10), get_base());
  return D2GAME_SpawnMonster_6FC69F10(
      pGame, pRoom, nX, nY, nMonsterId, nAnimMode, a7, nFlags);
}

}  // namespace diablo2
