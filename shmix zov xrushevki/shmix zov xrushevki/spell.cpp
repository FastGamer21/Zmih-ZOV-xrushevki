#include "spell.h"

Spell::Spell(std::string spell_name, std::string note_seq, int cost, int val,
             SpellEffectType type) {
  name = spell_name;
  sequence = note_seq;
  coffee_cost = cost;
  value = val;
  effect_type = type;
}

std::string Spell::getName() const { return name; }
std::string Spell::getSequence() const { return sequence; }
int Spell::getCost() const { return coffee_cost; }
int Spell::getValue() const { return value; }
SpellEffectType Spell::getEffectType() const { return effect_type; }
