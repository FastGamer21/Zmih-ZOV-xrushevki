#include "combat_system.h"

#include <iostream>

#include "constants.h"

CombatSystem::CombatSystem(ResourceManager* res_manager, Cockroach* main_roach,
                           NoteInventory* note_inv) {
  resources = res_manager;
  active_cockroach = main_roach;
  current_enemy = nullptr;
  notes = note_inv;
  in_combat = false;
  enemy_sleep = false;
  summon_used = false;

  known_spells.push_back(new Spell("Coffee Spit", "RTY", kSpellSpitCost,
                                   kSpellSpitDamage, EFFECT_DAMAGE));
  known_spells.push_back(new Spell("Coffee Firecracker", "GGHH",
                                   kSpellFirecrackerCost,
                                   kSpellFirecrackerDamage, EFFECT_DAMAGE));
}

CombatSystem::~CombatSystem() {
  for (size_t i = 0; i < known_spells.size(); ++i) delete known_spells[i];
}

void CombatSystem::startCombat(Enemy* enemy) {
  current_enemy = enemy;
  in_combat = true;
  enemy_sleep = false;
  summon_used = false;
}

bool CombatSystem::isInCombat() const { return in_combat; }
Enemy* CombatSystem::getCurrentEnemy() { return current_enemy; }
void CombatSystem::setActiveRoach(Cockroach* roach) {
  active_cockroach = roach;
}

void CombatSystem::learnSpell(Spell* new_spell) {
  if (!new_spell) return;
  for (size_t i = 0; i < known_spells.size(); ++i) {
    if (known_spells[i]->getSequence() == new_spell->getSequence()) return;
  }
  known_spells.push_back(new_spell);
}

bool CombatSystem::hasSummon() const { return summon_used; }
int CombatSystem::getSummonDamage() const { return 0; }

void CombatSystem::applySpellEffect(Spell* spell) {
  switch (spell->getEffectType()) {
    case EFFECT_DAMAGE:
      current_enemy->takeDamage(spell->getValue());
      break;
    case EFFECT_HEAL:
      resources->addCoffee(spell->getValue());
      break;
    case EFFECT_SLEEP:
      enemy_sleep = true;
      break;
    case EFFECT_SUMMON:
      current_enemy->takeDamage(spell->getValue());
      summon_used = true;
      break;
  }
}

// Основной луп боевки. Враг бьет, мы бьем. Если хп упало в ноль, то выходим из
// цикла
std::string CombatSystem::processTurn(std::string action) {
  if (!in_combat || current_enemy == nullptr) return "";

  std::string log = playerPhase(action);
  if (!current_enemy->isAlive()) {
    in_combat = false;
    return log + " Enemy defeated!";
  }
  return log + " | " + enemyPhase();
}

std::string CombatSystem::playerPhase(std::string action) {
  std::string roach_name = active_cockroach->getName();
  bool is_leader = (roach_name == "Leader");

  // Обработка физ. атак (не лимбус, конечно, но пойдёт)
  if (action == "E" || action == "e") {
    int dmg = active_cockroach->getPhysicalDamage();
    current_enemy->takeDamage(dmg);
    if (is_leader) {
      return "Weak Atk (-" + std::to_string(dmg) + " HP).";
    } else {
      return roach_name + " uses Bug Bite! (-" + std::to_string(dmg) + " HP).";
    }
  } else if (action == "Q" || action == "q") {
    if (!is_leader) {
      return roach_name + " can only use basic attacks (E)!";
    }
    if (resources->spendCoffee(kStrongAttackCost)) {
      int dmg = static_cast<int>(active_cockroach->getPhysicalDamage() *
                                 kStrongAttackMultiplier);
      current_enemy->takeDamage(dmg);
      return "Strong Atk (-" + std::to_string(dmg) + " HP).";
    }
    return "Not enough coffee!";
  } else {
    if (!is_leader) {
      return roach_name + " doesn't know how to cast spells!";
    }
    for (size_t i = 0; i < known_spells.size(); ++i) {
      if (action == known_spells[i]->getSequence()) {
        Spell* sp = known_spells[i];
        if (!notes->hasSequence(sp->getSequence())) {
          return "Missing notes for " + sp->getName() + "!";
        }
        if (!resources->spendCoffee(sp->getCost())) {
          return "Not enough coffee for " + sp->getName() + "!";
        }
        notes->spendSequence(sp->getSequence());
        applySpellEffect(sp);
        std::string result = "Cast " + sp->getName() + "! ";
        if (sp->getEffectType() == EFFECT_DAMAGE)
          result += "Dealt " + std::to_string(sp->getValue()) + " damage.";
        else if (sp->getEffectType() == EFFECT_HEAL)
          result += "Healed " + std::to_string(sp->getValue()) + " coffee.";
        else if (sp->getEffectType() == EFFECT_SLEEP)
          result += "Enemy falls asleep!";
        else if (sp->getEffectType() == EFFECT_SUMMON)
          result +=
              "Summon deals " + std::to_string(sp->getValue()) + " damage!";
        return result;
      }
    }
    return "Miss! Wrong note sequence.";
  }
}

std::string CombatSystem::enemyPhase() {
  if (enemy_sleep) {
    enemy_sleep = false;
    return "Enemy is asleep and does nothing!";
  }
  int dmg = current_enemy->getDamage();
  resources->spendCoffee(dmg);
  return "Enemy hits for " + std::to_string(dmg) + " dmg.";
}

Spell* CombatSystem::getSpellBySequence(const std::string& seq) const {
  for (size_t i = 0; i < known_spells.size(); ++i) {
    if (known_spells[i]->getSequence() == seq) return known_spells[i];
  }
  return nullptr;
}

std::string CombatSystem::applySpellOutOfCombat(const std::string& seq) {
  Spell* sp = getSpellBySequence(seq);
  if (!sp) return "Unknown spell sequence.";
  if (!notes->hasSequence(sp->getSequence())) {
    return "Missing notes for " + sp->getName() + "!";
  }
  if (!resources->spendCoffee(sp->getCost())) {
    return "Not enough coffee for " + sp->getName() + "!";
  }
  notes->spendSequence(sp->getSequence());
  if (sp->getEffectType() == EFFECT_HEAL) {
    resources->addCoffee(sp->getValue());
    return "Cast " + sp->getName() + "! Healed " +
           std::to_string(sp->getValue()) + " coffee.";
  } else if (sp->getEffectType() == EFFECT_DAMAGE) {
    return "Cannot use damage spell outside combat.";
  } else if (sp->getEffectType() == EFFECT_SLEEP) {
    return "Cannot use sleep spell outside combat.";
  } else if (sp->getEffectType() == EFFECT_SUMMON) {
    return "Cannot use summon spell outside combat.";
  }
  return "Spell cannot be used here.";
}

std::string CombatSystem::getSpellList() const {
  if (known_spells.empty()) {
    return "No spells learned.";
  }
  std::string result = "=== Learned spells ===\n";
  for (size_t i = 0; i < known_spells.size(); ++i) {
    Spell* s = known_spells[i];
    result += " - " + s->getName() + " (" + s->getSequence() + ")";
    result += " | cost: " + std::to_string(s->getCost()) + " coffee";
    if (s->getEffectType() == EFFECT_DAMAGE)
      result += " | damage: " + std::to_string(s->getValue());
    else if (s->getEffectType() == EFFECT_HEAL)
      result += " | heal: " + std::to_string(s->getValue());
    else if (s->getEffectType() == EFFECT_SLEEP)
      result += " | sleep: 1 turn";
    else if (s->getEffectType() == EFFECT_SUMMON)
      result += " | summon damage: " + std::to_string(s->getValue());
    result += "\n";
  }
  return result;
}

std::string CombatSystem::getNotesList() const { return notes->toString(); }
