
#include <Windows.h>

#include <spdlog/spdlog.h>

#include "common/file_ini.h"
#include "common/hooking.h"

#include "diablo2/structures/damage.h"
#include "diablo2/structures/data/monstats_line.h"
#include "diablo2/structures/monster_data.h"
#include "diablo2/structures/net_client.h"
#include "diablo2/structures/player_data.h"
#include "diablo2/structures/unit.h"

#include "diablo2/d2common.h"
#include "diablo2/d2game.h"

#include "d2tweaks/common/asset_manager.h"

#include "d2tweaks/server/server.h"
#include "d2tweaks/server/damage_display/damage_display_server.h"

using namespace d2_tweaks;
using namespace d2_tweaks::server;
using namespace diablo2;
using namespace diablo2::structures;

namespace d2_tweaks {
namespace server {

MODULE_INIT(DamageDisplay)

static char(__fastcall* g_apply_attack_results_origin)(Game* game,
                                                       Unit* attacker,
                                                       Unit* defender,
                                                       BOOL recalculateDamage,
                                                       Damage* dmg);

#define PRINT_DMG_DELIMITER(name, delimiter) \
  if (dmg->name > 0)                         \
  spdlog::debug(#name ": {0}", dmg->name / delimiter)
#define PRINT_DMG(name) PRINT_DMG_DELIMITER(name, 256)

DamageType::T get_damage_type(Damage* dmg) {
  uint32_t damage[common::DamageType::kCount]{0};

  if (dmg == nullptr || dmg->dmg_total == 0)
    return common::DamageType::kUnknown;

  damage[common::DamageType::kPhysical] = dmg->phys_damage;
  damage[common::DamageType::kCold] = dmg->cold_damage;
  damage[common::DamageType::kFire] = dmg->fire_damage;
  damage[common::DamageType::kLightning] = dmg->ltng_damage;
  damage[common::DamageType::kPoison] = dmg->pois_damage;
  damage[common::DamageType::kMagic] = dmg->mag_damage;

  auto result = common::DamageType::kUnknown;
  uint32_t damage_temp = 0;

  for (size_t i = 0; i < common::DamageType::kCount; i++) {
    if (damage[i] <= damage_temp)
      continue;

    result = static_cast<DamageType::T>(i);
    damage_temp = damage[i];
  }

  return result;
}

static void send_damage_data(Unit* defender,
  NetClient* client,
  Damage* dmg) {
  static auto& instance = Server::instance();
  static damage_info_sc packet;

  if (dmg->dmg_total <= 0)
    return;

  auto currentHp = d2_common::get_stat(defender, UNIT_STAT_HITPOINTS, 0);
  auto maxHp = d2_common::get_stat(defender, UNIT_STAT_MAXHP, 0);

  // if (defender->type == unit_type_t::UNIT_TYPE_PLAYER) {
  //   auto player = defender;
  //
  //   // spdlog player name
  //   spdlog::info("Player name: {0}", player->player_data->name);
  //
  //   d2_common::set_stat(player, UNIT_STAT_is_champion, 100, 0);
  //
  //   // if defender is a monster, get the monster data
  //   if (defender->type == unit_type_t::UNIT_TYPE_MONSTER) {
  //     auto monsterData = defender->monster_data;
  //     if (monsterData) {
  //       bool isChampion = monsterData->is_champion;
  //       bool isUnique = monsterData->is_unique;
  //       bool isSuperUnique = monsterData->is_super_unique;
  //
  //       spdlog::info("Monster isChampion: {0}", isChampion);
  //       spdlog::info("Monster isUnique: {0}", isUnique);
  //       spdlog::info("Monster isSuperUnique: {0}", isSuperUnique);
  //
  //       // You can add these details to the packet if needed
  //       packet.isChampion = isChampion;
  //       packet.isUnique = isUnique;
  //       packet.isSuperUnique = isSuperUnique;
  //
  //       // if currentHP is 0, if is_champion, then set_stat for player and
  //       // increment the player stat UNIT_STAT_is_champion by 1
  //       if (currentHp == 0) {
  //         if (isChampion) {
  //           // get the player is_champion stat
  //           auto isChampionValue =
  //               d2_common::get_stat(player, UNIT_STAT_is_champion, 0);
  //           d2_common::set_stat(
  //               player, UNIT_STAT_is_champion, isChampionValue + 1, 0);
  //           MessageBoxA(NULL, "Champion killed", "Champion killed", MB_OK);
  //           spdlog::info("Champion killed");
  //         }
  //         // do the if condition for isUnique and isSuperUnique here
  //         if (isUnique) {
  //           // get the player is_unique stat
  //           auto isUniqueValue =
  //               d2_common::get_stat(player, UNIT_STAT_is_unique, 0);
  //           d2_common::set_stat(
  //               player, UNIT_STAT_is_unique, isUniqueValue + 1, 0);
  //           MessageBoxA(NULL, "Unique killed", "Unique killed", MB_OK);
  //           spdlog::info("Unique killed");
  //         }
  //         if (isSuperUnique) {
  //           // get the player is_super_unique stat
  //           auto isSuperUniqueValue =
  //               d2_common::get_stat(player, UNIT_STAT_is_super_unique, 0);
  //           d2_common::set_stat(
  //               player, UNIT_STAT_is_super_unique, isSuperUniqueValue + 1, 0);
  //           MessageBoxA(
  //               NULL, "SuperUnique killed", "SuperUnique killed", MB_OK);
  //           spdlog::info("SuperUnique killed");
  //         }
  //       }
  //     }
  //   }
  // }

  packet.unit_type = static_cast<uint8_t>(defender->type);
  packet.guid = defender->guid;
  packet.damage_type = get_damage_type(dmg);
  packet.damage = dmg->dmg_total / 256;
  packet.currentHp = currentHp / 256;
  packet.maxHp = maxHp / 256;

  // spdlog::info("currentHp: {0}", packet.currentHp);
  // spdlog::info("maxHp: {0}", packet.maxHp);
  // spdlog::info("damage: {0}", packet.damage);

  if (packet.damage_type == common::DamageType::kUnknown)
    return;

  if (packet.damage <= 0)
    return;

  instance.send_packet(client, &packet, sizeof packet);
}

static bool has_players(Unit* attacker, Unit* defender) {
  return attacker->type == unit_type_t::UNIT_TYPE_PLAYER ||
         defender->type == unit_type_t::UNIT_TYPE_PLAYER;
}

static bool has_hirelings(Unit* attacker, Unit* defender) {
  return attacker && attacker->is_hireling() ||
         defender && defender->is_hireling();
}

static bool has_pets(Unit* attacker, Unit* defender) {
  return attacker && attacker->is_pet() || defender && defender->is_pet();
}

static Unit* get_hireling_owner(Game* game, Unit* pUnit) {
  static auto& instance = Server::instance();

  if (!pUnit)
    return nullptr;

  auto guid = d2_common::get_stat(pUnit, UNIT_STAT_UNUSED212, 0);

  if (guid != 0)
    return instance.get_server_unit(game, guid, unit_type_t::UNIT_TYPE_PLAYER);

  instance.iterate_server_units(
      game, unit_type_t::UNIT_TYPE_PLAYER, [&](Unit* player) {
        const auto pet = d2_game::get_player_pet(game, player, 7, 0);

        if (pet == pUnit) {
          guid = player->guid;
          return false;  // stop iterator
        }

        return true;
      });

  if (guid == 0)
    return nullptr;

  d2_common::set_stat(pUnit, UNIT_STAT_UNUSED212, guid, 0);

  return instance.get_server_unit(game, guid, unit_type_t::UNIT_TYPE_PLAYER);
}

static Unit* get_pet_owner(Game* pGame, Unit* pUnit) {
  static auto& instance = Server::instance();

  if (!pUnit)
    return nullptr;

  auto guid = d2_common::get_stat(pUnit, UNIT_STAT_UNUSED212, 0);

  if (guid != 0)
    return instance.get_server_unit(pGame, guid, unit_type_t::UNIT_TYPE_PLAYER);

  instance.iterate_server_units(
      pGame, unit_type_t::UNIT_TYPE_PLAYER, [&](Unit* player) {
        d2_game::iterate_unit_pets(pGame, player, [&](Game*, Unit*, Unit* u) {
          if (u != pUnit)
            return;

          guid = player->guid;
        });

        return guid == 0;
      });

  if (guid == 0)
    return nullptr;

  d2_common::set_stat(pUnit, UNIT_STAT_UNUSED212, guid, 0);

  return instance.get_server_unit(pGame, guid, unit_type_t::UNIT_TYPE_PLAYER);
}

static void process_players_damage(Unit* attacker,
                                   Unit* defender,
                                   Damage* dmg) {
  NetClient* client = nullptr;

  if (attacker->type == unit_type_t::UNIT_TYPE_PLAYER)
    client = attacker->player_data->net_client;
  else if (defender->type == unit_type_t::UNIT_TYPE_PLAYER)
    client = defender->player_data->net_client;

  send_damage_data(defender, client, dmg);
}

static void process_hireling_damage(Game* game,
                                    Unit* attacker,
                                    Unit* defender,
                                    Damage* dmg) {
  Unit* owner = nullptr;

  if (attacker->is_hireling()) {
    owner = get_hireling_owner(game, attacker);
  } else if (defender->is_hireling()) {
    owner = get_hireling_owner(game, defender);
  }

  if (!owner)
    return;

  send_damage_data(defender, owner->player_data->net_client, dmg);
}

static void process_pet_damage(Game* game,
                               Unit* attacker,
                               Unit* defender,
                               Damage* dmg) {
  Unit* owner = nullptr;

  if (attacker->is_pet()) {
    owner = get_pet_owner(game, attacker);
  } else if (defender->is_pet()) {
    owner = get_pet_owner(game, defender);
  }

  if (!owner)
    return;

  send_damage_data(defender, owner->player_data->net_client, dmg);
}

static char __fastcall apply_attack_results(Game* game,
                                            Unit* attacker,
                                            Unit* defender,
                                            BOOL recalculateDamage,
                                            Damage* dmg) {
  static auto& instance = Server::instance();

  static char result = 0;

  result = g_apply_attack_results_origin(
      game, attacker, defender, recalculateDamage, dmg);

  if (has_players(attacker, defender)) {
    process_players_damage(attacker, defender, dmg);
    return result;
  }

  if (has_hirelings(attacker, defender)) {
    process_hireling_damage(game, attacker, defender, dmg);
    return result;
  }

  if (has_pets(attacker, defender)) {
    process_pet_damage(game, attacker, defender, dmg);
    return result;
  }

  return result;
}

void DamageDisplay::init() {
  FileIni config(common::get_config_path());

  if (config.Int("modules", "DamageDisplay", 1)) {
    detour::hook(d2_game::get_base() + 0x8FE90,
                 apply_attack_results,
                 reinterpret_cast<void **>(&g_apply_attack_results_origin));
    Server::instance().register_packet_handler(MessageType::kDamageInfo, this);
  }
}

bool DamageDisplay::handle_packet(Game* game,
                                  Unit* player,
                                  packet_header* packet) {
  return true;
}

void DamageDisplay::tick(Game* game, Unit* unit) {}

}  // namespace server
}  // namespace d2_tweaks
