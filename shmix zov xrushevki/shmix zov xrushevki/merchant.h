#ifndef MERCHANT_H_
#define MERCHANT_H_

#include <string>

#include "resources.h"

// Класс барыги.
class Merchant {
 public:
  Merchant();

  std::string buyItem(std::string item_type, ResourceManager* resources);
  std::string buyUpgradeMaxCoffee(ResourceManager* resources);
  std::string buyUpgradeAttack(ResourceManager* resources, int& attack_damage);
  std::string getMenuText() const;
};

#endif  // MERCHANT_H_
