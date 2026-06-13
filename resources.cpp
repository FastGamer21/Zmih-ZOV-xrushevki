#include "resources.h"

ResourceManager::ResourceManager(int initial_amount, int max_amount) {
  max_coffee = max_amount;
  current_coffee = initial_amount;
  // лимит максимального кофе больше не ограничивает игрока
}

// добавляем кофе, можно копить бесконечно
void ResourceManager::addCoffee(int amount) { current_coffee += amount; }

// тратим кофе, если не хватает - обнуляем и возвращаем false
bool ResourceManager::spendCoffee(int amount) {
  if (current_coffee >= amount) {
    current_coffee -= amount;
    return true;
  }
  current_coffee = 0;
  return false;
}

int ResourceManager::getCurrentCoffee() const { return current_coffee; }

int ResourceManager::getMaxCoffee() const { return max_coffee; }

bool ResourceManager::isDead() const { return current_coffee <= 0; }

// эти методы оставлены для совместимости, но на практике не используются
void ResourceManager::increaseMaxCoffee(int amount) {
  max_coffee += amount;
  current_coffee += amount;
}

void ResourceManager::setMaxCoffee(int new_max) {
  if (new_max > 0) {
    max_coffee = new_max;
  }
}
