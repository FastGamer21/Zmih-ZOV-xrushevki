#ifndef BAND_MANAGER_H_
#define BAND_MANAGER_H_

#include <vector>

#include "entities.h"

// Менеджер нашей пати.
class BandManager {
 public:
  BandManager();
  ~BandManager();

  bool addRoach(Cockroach* roach);
  void switchNext();

  Cockroach* getActiveRoach() const;
  int getSize() const;
  int getActiveIndex() const;

  Cockroach* getRoachByIndex(int index) const;
  void setActiveIndex(int index);

 private:
  std::vector<Cockroach*> band;
  int active_index;
  static const int kMaxBandSize = 4;
};

#endif  // BAND_MANAGER_H_
