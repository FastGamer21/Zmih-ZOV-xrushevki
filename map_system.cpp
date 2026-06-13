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
        getPlane(kPlaneFloor
