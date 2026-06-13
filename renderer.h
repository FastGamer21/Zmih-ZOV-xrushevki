#ifndef RENDERER_H_
#define RENDERER_H_

#include <string>
#include <vector>

#include "entities.h"

// класс отвечает за отрисовку всего, что видит игрок на экране
class Renderer {
 public:
  Renderer();
  void clearScreen();

  // рисует основной экран игры: полоски с ресурсами, карту, события
  void drawUI(std::string level_name, int coffee, int max_coffee, int team_size,
              bool has_key, int current_point, std::string plane_name,
              std::string event_text, const std::vector<char>& markers,
              bool has_collectable, bool collectable_taken);

  // рисует экран боя: здоровье врага, наши ресурсы, лог событий
  void drawCombatUI(std::string level_name, int coffee, int max_coffee,
                    Enemy* enemy, std::string event_text);
};

#endif
