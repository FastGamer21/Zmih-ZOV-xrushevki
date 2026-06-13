#include "entities.h"

Cockroach::Cockroach(std::string roach_name, int phys_damage) {
  name = roach_name;
  physical_damage = phys_damage;
}

Cockroach::Cockroach() : name("Unknown"), physical_damage(2) {}

std::string Cockroach::getName() const { return name; }

int Cockroach::getPhysicalDamage() const { return physical_damage; }

void Cockroach::setPhysicalDamage(int dmg) { physical_damage = dmg; }

Enemy::Enemy(std::string enemy_type, int max_hp, int atk_damage) {
  type = enemy_type;
  health = max_hp;
  damage = atk_damage;
}

// Чекаем, чтоб ХП не ушло в минус при получении тычки.
void Enemy::takeDamage(int amount) {
  health -= amount;
  if (health < 0) health = 0;
}

int Enemy::getDamage() const { return damage; }

bool Enemy::isAlive() const { return health > 0; }

std::string Enemy::getType() const { return type; }

int Enemy::getHealth() const { return health; }
