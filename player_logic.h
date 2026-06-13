#ifndef PLAYER_LOGIC_H_
#define PLAYER_LOGIC_H_

#include <string>

// управляет перемещением игрока по карте
// хранит текущую плоскость (пол, стена, потолок) и точку на ней
class PlayerController {
 public:
  PlayerController();
  void move(std::string direction);  // перемещение WASD
  void changePlane();                // прыжок на другую плоскость (кнопка R)
  void setPosition(int plane, int point);
  void setAllowPlaneChange(bool allow);  // запрещает смену плоскости (нужно для
                                         // первого и последнего этажа)
  int getCurrentPlane() const;
  int getCurrentPoint() const;
  std::string getPlaneName() const;  // человекочитаемое название плоскости

 private:
  int current_plane;        // какая сейчас плоскость (пол/потолок/стена)
  int current_point;        // где именно на плоскости (центр/края)
  bool allow_plane_change;  // можно ли переходить на другие плоскости
};

#endif
