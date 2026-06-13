#ifndef ENTITIES_H_
#define ENTITIES_H_

#include <string>

// Базовый класс для наших бойцов
class Cockroach {
 public:
  Cockroach(std::string roach_name, int phys_damage);
  Cockroach();
  std::string getName() const;
  int getPhysicalDamage() const;
  void setPhysicalDamage(int dmg);

 private:
  std::string name;
  int physical_damage;
};

// Класс для мобов. Держит статы и чекает статус хп
class Enemy {
 public:
  Enemy(std::string enemy_type, int max_hp, int atk_damage);
  void takeDamage(int amount);
  int getDamage() const;
  bool isAlive() const;
  std::string getType() const;
  int getHealth() const;

 private:
  std::string type;
  int health;
  int damage;
};

#endif  // ENTITIES_H_