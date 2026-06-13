#include "merchant.h"

#include <iostream>

#include "constants.h"

Merchant::Merchant() {}

std::string Merchant::buyItem(std::string item_type,
                              ResourceManager* resources) {
  if (item_type == "1") {
    if (resources->spendCoffee(kPriceNote)) {
      return "Bought";
    } else {
      return "Not enough coffee for Note!";
    }
  } else if (item_type == "2") {
    if (resources->spendCoffee(kPriceRoach)) {
      return "New Roach joined the band! (-" + std::to_string(kPriceRoach) +
             " coffee)";
    } else {
      return "Not enough coffee for Ally!";
    }
  }
  return "Item out of stock.";
}

std::string Merchant::buyUpgradeMaxCoffee(ResourceManager* resources) {
  return "This item is no longer available.";
}

// Áאפאול האלאד Ëטהונא חא זלûץ
std::string Merchant::buyUpgradeAttack(ResourceManager* resources,
                                       int& attack_damage) {
  if (resources->getCurrentCoffee() >= kUpgradeAttackPrice) {
    resources->spendCoffee(kUpgradeAttackPrice);
    attack_damage += kUpgradeAttackAmount;
    return "Upgraded attack damage! (+" + std::to_string(kUpgradeAttackAmount) +
           ", cost " + std::to_string(kUpgradeAttackPrice) + " coffee)";
  } else {
    return "Not enough coffee for Attack upgrade! Need " +
           std::to_string(kUpgradeAttackPrice);
  }
}

std::string Merchant::getMenuText() const {
  return "MERCHANT: [1] Note(" + std::to_string(kPriceNote) + ") | " +
         "[2] Roach(" + std::to_string(kPriceRoach) + ") | " + "[4] Attack(" +
         std::to_string(kUpgradeAttackPrice) + ")";
}
