#ifndef COMBAT_SYSTEM_H_
#define COMBAT_SYSTEM_H_

#include <string>
#include <vector>

#include "entities.h"
#include "note_inventory.h"
#include "resources.h"
#include "spell.h"

// КОР ВСЕЙ БОЁВОЧКИ УФФФФФ, МОЯ ПРЕЛЕСТЬ ТЫ РОДНАЯ
class CombatSystem {
 public:
  CombatSystem(ResourceManager* res_manager, Cockroach* main_roach,
               NoteInventory* note_inv);
  ~CombatSystem();

  void startCombat(Enemy* enemy);
  bool isInCombat() const;
  Enemy* getCurrentEnemy();

  std::string processTurn(std::string action);
  void learnSpell(Spell* new_spell);

  bool hasSummon() const;
  int getSummonDamage() const;

  void setActiveRoach(Cockroach* roach);

  Spell* getSpellBySequence(const std::string& seq) const;
  std::string applySpellOutOfCombat(const std::string& seq);
  std::string getSpellList() const;
  std::string getNotesList() const;

 private:
  std::string playerPhase(std::string action);
  std::string enemyPhase();
  void applySpellEffect(Spell* spell);

  ResourceManager* resources;
  Cockroach* active_cockroach;
  Enemy* current_enemy;
  NoteInventory* notes;
  bool in_combat;
  bool enemy_sleep;
  bool summon_used;
  std::vector<Spell*> known_spells;
};

#endif  // COMBAT_SYSTEM_H_
