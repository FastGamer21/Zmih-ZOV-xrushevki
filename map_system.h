#ifndef MAP_SYSTEM_H_
#define MAP_SYSTEM_H_

#include <string>
#include <vector>

#include "constants.h"

// одна точка на плоскости (центр или один из четырёх краёв)
class Point {
 public:
  Point(int point_id);
  int getId() const;
  void setTrap(std::string trap_name);
  std::string getTrap() const;
  void clearTrap();
  void setResource(std::string res);
  std::string getResource() const;
  bool hasResource() const;
  void clearResource();
  void setNoteChar(char c);
  char getNoteChar() const;
  void setHasEnemy(bool has);
  bool getHasEnemy() const;
  void setSpellSequence(std::string seq);
  std::string getSpellSequence() const;

  void setVent(bool vent);
  bool isVent() const;

  void setMerchant(bool has);
  bool hasMerchant() const;

 private:
  int id;
  std::string trap_type;      // ловушка на точке
  std::string resource_type;  // тип ресурса, лежащего на точке
  char note_char;             // какая именно нота, если ресурс - нота
  bool has_enemy;
  std::string spell_sequence;  // для свитка заклинания
  bool vent;                   // есть ли тут вентиляция
  bool merchant;               // стоит ли тут торговец
};

// плоскость: пол, потолок или одна из четырёх стен
class Plane {
 public:
  Plane(int plane_id, std::string plane_name);
  std::string getName() const;
  int getId() const;
  Point* getPoint(int point_id);  // получить точку на плоскости по индексу

 private:
  int id;
  std::string name;
  std::vector<Point> points;  // 5 точек на каждой плоскости
};

// уровень = один этаж дома (0-4)
class Level {
 public:
  Level(int floor_num);
  Plane* getPlane(int plane_id);
  std::string getName() const;
  int getFloorNumber() const;
  void generateContent();  // генерирует всё содержимое уровня случайным образом

  bool hasCollectableOnLevel() const;
  bool isCollectableTaken() const;
  void setCollectableTaken(bool taken);
  void setHasCollectableOnLevel(bool has);

  // позиция босса на уровне
  int getBossPlane() const;
  int getBossPoint() const;
  // позиция выхода с уровня
  int getExitPlane() const;
  int getExitPoint() const;

 private:
  void initializePlanes();  // создаёт нужные плоскости для этажа
  int floor_number;
  std::string name;
  std::vector<Plane> planes;
  bool has_collectable_on_level;  // есть ли на этом уровне секретный предмет
  bool collectable_taken;         // забрали ли его уже
  int boss_plane;                 // где сидит босс
  int boss_point;
  int exit_plane;  // где выход на следующий этаж
  int exit_point;
};

#endif
