#include "band_manager.h"

#include <iostream>

BandManager::BandManager() : active_index(0) {}

// Чистим память
BandManager::~BandManager() { band.clear(); }

// Добавляем нового таракана в пачку и не даём, если фулл пачка
bool BandManager::addRoach(Cockroach* roach) {
  if (band.size() >= kMaxBandSize) {
    std::cout << "Band is full (max " << kMaxBandSize << " roaches)!\n";
    return false;
  }
  band.push_back(roach);
  if (band.size() == 1) active_index = 0;
  std::cout << "New roach \"" << roach->getName() << "\" joined the band!\n";
  return true;
}

// Свапаем таракана на следующего по кругу
void BandManager::switchNext() {
  if (band.empty()) return;
  active_index = (active_index + 1) % band.size();
  std::cout << "Switched to active roach: " << band[active_index]->getName()
            << "\n";
}

Cockroach* BandManager::getActiveRoach() const {
  if (band.empty()) return nullptr;
  return band[active_index];
}

int BandManager::getSize() const { return static_cast<int>(band.size()); }

int BandManager::getActiveIndex() const { return active_index; }

Cockroach* BandManager::getRoachByIndex(int index) const {
  if (index < 0 || index >= static_cast<int>(band.size())) return nullptr;
  return band[index];
}

void BandManager::setActiveIndex(int index) {
  if (index >= 0 && index < static_cast<int>(band.size())) {
    active_index = index;
  }
}
