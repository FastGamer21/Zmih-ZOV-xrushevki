#ifndef SPELL_H_
#define SPELL_H_

#include <string>

enum SpellEffectType {
  EFFECT_DAMAGE,
  EFFECT_HEAL,
  EFFECT_SLEEP,
  EFFECT_SUMMON
};
// Класс для спелла
class Spell {
 public:
  Spell(std::string spell_name, std::string note_seq, int cost, int value,
        SpellEffectType type = EFFECT_DAMAGE);
  std::string getName() const;
  std::string getSequence() const;
  int getCost() const;
  int getValue() const;
  SpellEffectType getEffectType() const;

 private:
  std::string name;
  std::string sequence;
  int coffee_cost;
  int value;
  SpellEffectType effect_type;
};

#endif  // SPELL_H_
