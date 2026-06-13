#include "map_system.h"

#include <cstdlib>
#include <iostream>
#include <vector>

#include "constants.h"

// ---------- Point ----------
Point::Point(int point_id) {
  id = point_id;
  trap_type = "None";
  has_enemy = false;
  resource_type = "";
  note_char = ' ';
  spell_sequence = "";
  vent = false;
  merchant = false;
}

int Point::getId() const { return id; }
void Point::setTrap(std::string trap_name) { trap_type = trap_name; }
std::string Point::getTrap() const { return trap_type; }
void Point::clearTrap() { trap_type = "None"; }
void Point::setHasEnemy(bool has) { has_enemy = has; }
bool Point::getHasEnemy() const { return has_enemy; }
void Point::setResource(std::string res) { resource_type = res; }
std::string Point::getResource() const { return resource_type; }
bool Point::hasResource() const { return !resource_type.empty(); }
void Point::clearResource() {
  resource_type = "";
  note_char = ' ';
  spell_sequence = "";
}
void Point::setNoteChar(char c) { note_char = c; }
char Point::getNoteChar() const { return note_char; }
void Point::setSpellSequence(std::string seq) { spell_sequence = seq; }
std::string Point::getSpellSequence() const { return spell_sequence; }
void Point::setVent(bool v) { vent = v; }
bool Point::isVent() const { return vent; }
void Point::setMerchant(bool has) { merchant = has; }
bool Point::hasMerchant() const { return merchant; }

// ---------- Plane ----------
Plane::Plane(int plane_id, std::string plane_name) {
  id = plane_id;
  name = plane_name;
  for (int i = 0; i < 5; ++i) points.push_back(Point(i));
}
std::string Plane::getName() const { return name; }
int Plane::getId() const { return id; }
Point* Plane::getPoint(int point_id) {
  if (point_id >= 0 && point_id < 5) return &points[point_id];
  return nullptr;
}

// ---------- Level ----------
Level::Level(int floor_num) : floor_number(floor_num) {
  // задаём название этажа
  if (floor_num == 0)
    name = "Basement";
  else if (floor_num == 1)
    name = "Grandma's Flat";
  else if (floor_num == 2)
    name = "Communal Flat";
  else if (floor_num == 3)
    name = "Party Flat";
  else if (floor_num == 4)
    name = "Attic";
  else
    name = "Unknown";

  has_collectable_on_level = false;
  collectable_taken = false;
  boss_plane = -1;
  boss_point = -1;
  exit_plane = -1;
  exit_point = -1;
  initializePlanes();
}

// создаёт плоскости для уровня
// на нулевом и четвёртом этаже только пол, на остальных - все 6 плоскостей
void Level::initializePlanes() {
  if (floor_number == 0 || floor_number == 4) {
    planes.push_back(Plane(kPlaneFloor, "Floor"));
    return;
  }
  planes.push_back(Plane(kPlaneFloor, "Floor"));
  planes.push_back(Plane(kPlaneCeiling, "Ceiling"));
  planes.push_back(Plane(kPlaneNorthWall, "North Wall"));
  planes.push_back(Plane(kPlaneSouthWall, "South Wall"));
  planes.push_back(Plane(kPlaneWestWall, "West Wall"));
  planes.push_back(Plane(kPlaneEastWall, "East Wall"));
}

Plane* Level::getPlane(int plane_id) {
  for (size_t i = 0; i < planes.size(); ++i)
    if (planes[i].getId() == plane_id) return &planes[i];
  return nullptr;
}
std::string Level::getName() const { return name; }
int Level::getFloorNumber() const { return floor_number; }

bool Level::hasCollectableOnLevel() const { return has_collectable_on_level; }
bool Level::isCollectableTaken() const { return collectable_taken; }
void Level::setCollectableTaken(bool taken) { collectable_taken = taken; }
void Level::setHasCollectableOnLevel(bool has) {
  has_collectable_on_level = has;
}

int Level::getBossPlane() const { return boss_plane; }
int Level::getBossPoint() const { return boss_point; }
int Level::getExitPlane() const { return exit_plane; }
int Level::getExitPoint() const { return exit_point; }

// главный метод генерации всего содержимого уровня
void Level::generateContent() {
  // нулевой этаж (подвал) генерируется по-простому
  if (floor_number == 0) {
    getPlane(kPlaneFloor)->getPoint(kPointSouthEdge)->setMerchant(true);
    boss_plane = kPlaneFloor;
    boss_point = kPointCenter;
    exit_plane = kPlaneFloor;
    exit_point = kPointNorthEdge;

    getPlane(kPlaneFloor)->getPoint(kPointWestEdge)->setResource(kResourceNote);
    getPlane(kPlaneFloor)->getPoint(kPointWestEdge)->setNoteChar('R');
    getPlane(kPlaneFloor)
        ->getPoint(kPointEastEdge)
        ->setResource(kResourceCoffee);
    return;
  }

  if (floor_number == 4) {
    getPlane(kPlaneFloor)->getPoint(kPointSouthEdge)->setMerchant(true);
  }

  // генерация ловушек, врагов и случайных ресурсов
  for (int i = 0; i < 6; ++i) {
    Plane* current_plane = getPlane(i);
    if (!current_plane) continue;

    for (int j = 0; j < 5; ++j) {
      if (i == kPlaneFloor && j == kPointCenter) continue;
      if (i == kPlaneFloor && j == kPointNorthEdge) continue;
      if (floor_number == 4 && i == kPlaneFloor && j == kPointSouthEdge)
        continue;

      int r = rand() % 100;

      // ловушки в зависимости от этажа
      if (r < 10) {
        if (floor_number == 1)
          current_plane->getPoint(j)->setTrap(kTrapCobweb);
        else if (floor_number == 2)
          current_plane->getPoint(j)->setTrap(kTrapStickyTape);
        else if (floor_number == 3)
          current_plane->getPoint(j)->setTrap(kTrapDichlorvos);
      } else if (r < 18 && floor_number >= 2) {
        current_plane->getPoint(j)->setTrap(kObstacleBoxes);
      } else if (r < 33 && !(floor_number == 3 && i == kPlaneFloor)) {
        current_plane->getPoint(j)->setHasEnemy(true);
      } else {
        if (rand() % 100 < kResourceSpawnChance) {
          int type = rand() % 100;
          if (type < kRoachAllyChance) {
            current_plane->getPoint(j)->setResource(kResourceRoach);
          } else if (type < kRoachAllyChance + kCollectableChance) {
            // секретки спавнятся отдельно
          } else if (type < kRoachAllyChance + kCollectableChance +
                                kSpellScrollChance) {
            current_plane->getPoint(j)->setResource(kResourceSpellScroll);
            const char* spells[] = {"U", "JKL", "GHJ"};
            current_plane->getPoint(j)->setSpellSequence(spells[rand() % 3]);
          } else {
            current_plane->getPoint(j)->setResource(kResourceCoffee);
          }
        }
      }
    }
  }

  // секретки (гарантированно 1 на уровень)
  if (floor_number >= 1 && floor_number <= 3) {
    bool placed = false;
    for (int attempt = 0; attempt < 100 && !placed; ++attempt) {
      int plane_idx = rand() % 6;
      int point_idx = rand() % 5;
      if (plane_idx == kPlaneFloor &&
          (point_idx == kPointCenter || point_idx == kPointNorthEdge))
        continue;
      Point* pt = getPlane(plane_idx)->getPoint(point_idx);
      if (pt->getTrap() == "None" && !pt->getHasEnemy() && !pt->hasResource() &&
          !pt->isVent() && !pt->hasMerchant()) {
        pt->setResource(kResourceCollectable);
        has_collectable_on_level = true;
        placed = true;
      }
    }
  }

  // вентиляция
  for (int i = 0; i < 6; ++i) {
    if (i == kPlaneFloor) continue;
    Plane* pl = getPlane(i);
    if (!pl) continue;
    for (int j = 0; j < 5; ++j) {
      Point* pt = pl->getPoint(j);
      if (pt->getTrap() != "None" || pt->getHasEnemy() || pt->hasResource())
        continue;
      if (rand() % 100 < kVentSpawnChance) pt->setVent(true);
    }
  }

  // спавн торговца
  if (floor_number >= 1 && floor_number <= 3) {
    while (true) {
      int m_plane = rand() % 6;
      int m_point = rand() % 5;
      if (m_plane == kPlaneFloor &&
          (m_point == kPointCenter || m_point == kPointNorthEdge))
        continue;
      Point* pt = getPlane(m_plane)->getPoint(m_point);
      if (pt->getTrap() == "None" && !pt->getHasEnemy() && !pt->hasResource() &&
          !pt->isVent()) {
        pt->setMerchant(true);
        break;
      }
    }
  }

  // босс и выход
  if (floor_number >= 1 && floor_number <= 3) {
    std::vector<int> free_centers;
    std::vector<std::pair<int, int>> free_spots;

    for (int pl = 0; pl < 6; ++pl) {
      for (int pt = 0; pt < 5; ++pt) {
        Point* p = getPlane(pl)->getPoint(pt);
        if (p->getTrap() == "None" && !p->getHasEnemy() && !p->hasResource() &&
            !p->isVent() && !p->hasMerchant()) {
          free_spots.push_back({pl, pt});
          if (pt == kPointCenter) {
            free_centers.push_back(pl);
          }
        }
      }
    }

    if (!free_centers.empty()) {
      boss_plane = free_centers[rand() % free_centers.size()];
      boss_point = kPointCenter;
      for (auto it = free_spots.begin(); it != free_spots.end(); ++it) {
        if (it->first == boss_plane && it->second == boss_point) {
          free_spots.erase(it);
          break;
        }
      }
    } else {
      boss_plane = kPlaneFloor;
      boss_point = kPointCenter;
    }

    if (!free_spots.empty()) {
      int idx = rand() % free_spots.size();
      exit_plane = free_spots[idx].first;
      exit_point = free_spots[idx].second;
    } else {
      exit_plane = kPlaneFloor;
      exit_point = kPointNorthEdge;
    }
  } else if (floor_number == 4) {
    boss_plane = kPlaneFloor;
    boss_point = kPointCenter;
    exit_plane = kPlaneFloor;
    exit_point = kPointNorthEdge;
  }

  // ноты на уровне
  if (floor_number >= 1 && floor_number <= 3) {
    std::vector<char> floor_notes;
    if (floor_number == 1)
      floor_notes = {'T', 'Y', 'U'};
    else if (floor_number == 2)
      floor_notes = {'G', 'H'};
    else if (floor_number == 3)
      floor_notes = {'J', 'K'};

    for (char note_char : floor_notes) {
      std::vector<std::pair<int, int>> free_spots_for_notes;
      for (int pl = 0; pl < 6; ++pl) {
        for (int pt = 0; pt < 5; ++pt) {
          Point* p = getPlane(pl)->getPoint(pt);
          if (p->getTrap() == "None" && !p->getHasEnemy() &&
              !p->hasResource() && !p->isVent() && !p->hasMerchant()) {
            bool is_boss = (pl == boss_plane && pt == boss_point);
            bool is_exit = (pl == exit_plane && pt == exit_point);
            if (!is_boss && !is_exit) {
              free_spots_for_notes.push_back({pl, pt});
            }
          }
        }
      }
      if (!free_spots_for_notes.empty()) {
        int idx = rand() % free_spots_for_notes.size();
        Point* p = getPlane(free_spots_for_notes[idx].first)
                       ->getPoint(free_spots_for_notes[idx].second);
        p->setResource(kResourceNote);
        p->setNoteChar(note_char);
      }
    }
  }

  // добиваем кофе на свободные места
  if (floor_number > 0) {
    for (int i = 0; i < 6; ++i) {
      Plane* pl = getPlane(i);
      if (!pl) continue;
      for (int j = 0; j < 5; ++j) {
        Point* pt = pl->getPoint(j);
        if (pt->getTrap() == "None" && !pt->getHasEnemy() &&
            !pt->hasResource() && !pt->isVent() && !pt->hasMerchant()) {
          bool is_boss = (i == boss_plane && j == boss_point);
          bool is_exit = (i == exit_plane && j == exit_point);
          if (!is_boss && !is_exit) {
            if (rand() % 100 < 20) {
              pt->setResource(kResourceCoffee);
            }
          }
        }
      }
    }
  }
}
