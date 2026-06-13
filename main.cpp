#include <windows.h>

#include <cstdlib>
#include <ctime>

#include "game_engine.h"

int main() {
  SetConsoleCP(CP_UTF8);
  srand(static_cast<unsigned>(time(nullptr)));
  GameEngine engine;
  engine.run();
  return 0;
}
