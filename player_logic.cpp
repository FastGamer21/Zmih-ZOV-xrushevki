#include "player_logic.h"

#include "constants.h"

PlayerController::PlayerController() {
  current_plane = kPlaneFloor;   // начинаем на полу
  current_point = kPointCenter;  // в центре
  allow_plane_change = true;
}

void PlayerController::setAllowPlaneChange(bool allow) {
  allow_plane_change = allow;
}

// обрабатывает нажатия WASD
// если смена плоскостей запрещена, просто ходим по краям центра
// если разрешена - можно переходить на стены и потолок
void PlayerController::move(std::string direction) {
  if (!allow_plane_change) {
    if (current_point == kPointCenter) {
      if (direction == "W" || direction == "w")
        current_point = kPointNorthEdge;
      else if (direction == "D" || direction == "d")
        current_point = kPointEastEdge;
      else if (direction == "S" || direction == "s")
        current_point = kPointSouthEdge;
      else if (direction == "A" || direction == "a")
        current_point = kPointWestEdge;
    } else {
      current_point = kPointCenter;
    }
    return;
  }

  // если мы в центре, просто идём к краю
  if (current_point == kPointCenter) {
    if (direction == "W" || direction == "w")
      current_point = kPointNorthEdge;
    else if (direction == "D" || direction == "d")
      current_point = kPointEastEdge;
    else if (direction == "S" || direction == "s")
      current_point = kPointSouthEdge;
    else if (direction == "A" || direction == "a")
      current_point = kPointWestEdge;
    return;
  }

  // сложная логика: переходы между плоскостями
  bool transition = false;
  std::string d = "";
  if (direction == "W" || direction == "w") d = "W";
  if (direction == "A" || direction == "a") d = "A";
  if (direction == "S" || direction == "s") d = "S";
  if (direction == "D" || direction == "d") d = "D";

  // с пола на стены
  if (current_plane == kPlaneFloor) {
    if (current_point == kPointNorthEdge && d == "W") {
      current_plane = kPlaneNorthWall;
      current_point = kPointSouthEdge;
      transition = true;
    } else if (current_point == kPointSouthEdge && d == "S") {
      current_plane = kPlaneSouthWall;
      current_point = kPointSouthEdge;
      transition = true;
    } else if (current_point == kPointWestEdge && d == "A") {
      current_plane = kPlaneWestWall;
      current_point = kPointSouthEdge;
      transition = true;
    } else if (current_point == kPointEastEdge && d == "D") {
      current_plane = kPlaneEastWall;
      current_point = kPointSouthEdge;
      transition = true;
    }
  }
  // с северной стены
  else if (current_plane == kPlaneNorthWall) {
    if (current_point == kPointNorthEdge && d == "W") {
      current_plane = kPlaneCeiling;
      current_point = kPointSouthEdge;
      transition = true;
    } else if (current_point == kPointSouthEdge && d == "S") {
      current_plane = kPlaneFloor;
      current_point = kPointNorthEdge;
      transition = true;
    } else if (current_point == kPointWestEdge && d == "A") {
      current_plane = kPlaneWestWall;
      current_point = kPointEastEdge;
      transition = true;
    } else if (current_point == kPointEastEdge && d == "D") {
      current_plane = kPlaneEastWall;
      current_point = kPointWestEdge;
      transition = true;
    }
  }
  // с южной стены
  else if (current_plane == kPlaneSouthWall) {
    if (current_point == kPointNorthEdge && d == "W") {
      current_plane = kPlaneCeiling;
      current_point = kPointNorthEdge;
      transition = true;
    } else if (current_point == kPointSouthEdge && d == "S") {
      current_plane = kPlaneFloor;
      current_point = kPointSouthEdge;
      transition = true;
    } else if (current_point == kPointWestEdge && d == "A") {
      current_plane = kPlaneEastWall;
      current_point = kPointEastEdge;
      transition = true;
    } else if (current_point == kPointEastEdge && d == "D") {
      current_plane = kPlaneWestWall;
      current_point = kPointWestEdge;
      transition = true;
    }
  }
  // с западной стены
  else if (current_plane == kPlaneWestWall) {
    if (current_point == kPointNorthEdge && d == "W") {
      current_plane = kPlaneCeiling;
      current_point = kPointWestEdge;
      transition = true;
    } else if (current_point == kPointSouthEdge && d == "S") {
      current_plane = kPlaneFloor;
      current_point = kPointWestEdge;
      transition = true;
    } else if (current_point == kPointWestEdge && d == "A") {
      current_plane = kPlaneSouthWall;
      current_point = kPointEastEdge;
      transition = true;
    } else if (current_point == kPointEastEdge && d == "D") {
      current_plane = kPlaneNorthWall;
      current_point = kPointWestEdge;
      transition = true;
    }
  }
  // с восточной стены
  else if (current_plane == kPlaneEastWall) {
    if (current_point == kPointNorthEdge && d == "W") {
      current_plane = kPlaneCeiling;
      current_point = kPointEastEdge;
      transition = true;
    } else if (current_point == kPointSouthEdge && d == "S") {
      current_plane = kPlaneFloor;
      current_point = kPointEastEdge;
      transition = true;
    } else if (current_point == kPointWestEdge && d == "A") {
      current_plane = kPlaneNorthWall;
      current_point = kPointEastEdge;
      transition = true;
    } else if (current_point == kPointEastEdge && d == "D") {
      current_plane = kPlaneSouthWall;
      current_point = kPointWestEdge;
      transition = true;
    }
  }
  // с потолка
  else if (current_plane == kPlaneCeiling) {
    if (current_point == kPointNorthEdge && d == "W") {
      current_plane = kPlaneNorthWall;
      current_point = kPointNorthEdge;
      transition = true;
    } else if (current_point == kPointSouthEdge && d == "S") {
      current_plane = kPlaneSouthWall;
      current_point = kPointNorthEdge;
      transition = true;
    } else if (current_point == kPointWestEdge && d == "A") {
      current_plane = kPlaneWestWall;
      current_point = kPointNorthEdge;
      transition = true;
    } else if (current_point == kPointEastEdge && d == "D") {
      current_plane = kPlaneEastWall;
      current_point = kPointNorthEdge;
      transition = true;
    }
  }

  // если не перешли на другую плоскость, просто идём в центр
  if (!transition) {
    if ((current_point == kPointNorthEdge && d == "S") ||
        (current_point == kPointSouthEdge && d == "W") ||
        (current_point == kPointEastEdge && d == "A") ||
        (current_point == kPointWestEdge && d == "D")) {
      current_point = kPointCenter;
    }
  }
}

// прыжок на соседнюю плоскость (кнопка R)
// пока не используется в игре
void PlayerController::changePlane() {
  if (!allow_plane_change || current_point == kPointCenter) return;

  if (current_plane == kPlaneFloor) {
    if (current_point == kPointNorthEdge) {
      current_plane = kPlaneNorthWall;
      current_point = kPointSouthEdge;
    } else if (current_point == kPointSouthEdge) {
      current_plane = kPlaneSouthWall;
      current_point = kPointSouthEdge;
    } else if (current_point == kPointWestEdge) {
      current_plane = kPlaneWestWall;
      current_point = kPointSouthEdge;
    } else if (current_point == kPointEastEdge) {
      current_plane = kPlaneEastWall;
      current_point = kPointSouthEdge;
    }
  } else if (current_plane == kPlaneNorthWall) {
    if (current_point == kPointNorthEdge) {
      current_plane = kPlaneCeiling;
      current_point = kPointSouthEdge;
    } else if (current_point == kPointSouthEdge) {
      current_plane = kPlaneFloor;
      current_point = kPointNorthEdge;
    } else if (current_point == kPointWestEdge) {
      current_plane = kPlaneWestWall;
      current_point = kPointEastEdge;
    } else if (current_point == kPointEastEdge) {
      current_plane = kPlaneEastWall;
      current_point = kPointWestEdge;
    }
  } else if (current_plane == kPlaneSouthWall) {
    if (current_point == kPointNorthEdge) {
      current_plane = kPlaneCeiling;
      current_point = kPointNorthEdge;
    } else if (current_point == kPointSouthEdge) {
      current_plane = kPlaneFloor;
      current_point = kPointSouthEdge;
    } else if (current_point == kPointWestEdge) {
      current_plane = kPlaneEastWall;
      current_point = kPointEastEdge;
    } else if (current_point == kPointEastEdge) {
      current_plane = kPlaneWestWall;
      current_point = kPointWestEdge;
    }
  } else if (current_plane == kPlaneWestWall) {
    if (current_point == kPointNorthEdge) {
      current_plane = kPlaneCeiling;
      current_point = kPointWestEdge;
    } else if (current_point == kPointSouthEdge) {
      current_plane = kPlaneFloor;
      current_point = kPointWestEdge;
    } else if (current_point == kPointWestEdge) {
      current_plane = kPlaneSouthWall;
      current_point = kPointEastEdge;
    } else if (current_point == kPointEastEdge) {
      current_plane = kPlaneNorthWall;
      current_point = kPointWestEdge;
    }
  } else if (current_plane == kPlaneEastWall) {
    if (current_point == kPointNorthEdge) {
      current_plane = kPlaneCeiling;
      current_point = kPointEastEdge;
    } else if (current_point == kPointSouthEdge) {
      current_plane = kPlaneFloor;
      current_point = kPointEastEdge;
    } else if (current_point == kPointWestEdge) {
      current_plane = kPlaneNorthWall;
      current_point = kPointEastEdge;
    } else if (current_point == kPointEastEdge) {
      current_plane = kPlaneSouthWall;
      current_point = kPointWestEdge;
    }
  } else if (current_plane == kPlaneCeiling) {
    if (current_point == kPointNorthEdge) {
      current_plane = kPlaneNorthWall;
      current_point = kPointNorthEdge;
    } else if (current_point == kPointSouthEdge) {
      current_plane = kPlaneSouthWall;
      current_point = kPointNorthEdge;
    } else if (current_point == kPointWestEdge) {
      current_plane = kPlaneWestWall;
      current_point = kPointNorthEdge;
    } else if (current_point == kPointEastEdge) {
      current_plane = kPlaneEastWall;
      current_point = kPointNorthEdge;
    }
  }
}

// устанавливает позицию игрока вручную (используется при смене уровня)
void PlayerController::setPosition(int plane, int point) {
  current_plane = plane;
  current_point = point;
}

int PlayerController::getCurrentPlane() const { return current_plane; }
int PlayerController::getCurrentPoint() const { return current_point; }

// возвращает красивое название плоскости для отображения игроку
std::string PlayerController::getPlaneName() const {
  if (current_plane == kPlaneFloor) return "Floor";
  if (current_plane == kPlaneCeiling) return "Ceiling";
  if (current_plane == kPlaneNorthWall) return "North Wall";
  if (current_plane == kPlaneSouthWall) return "South Wall";
  if (current_plane == kPlaneWestWall) return "West Wall";
  if (current_plane == kPlaneEastWall) return "East Wall";
  return "Unknown";
}
