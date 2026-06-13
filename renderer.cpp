#include "renderer.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "constants.h"

// вспомогательная функция: печатает строку внутри рамки, выравнивая по левому
// краю
void printPaddedLine(const std::string& text) {
  std::cout << "| " << text;
  int padding = 48 - static_cast<int>(text.length());
  if (padding > 0) std::cout << std::string(padding, ' ');
  std::cout << " |\n";
}

// рисует две колонки информации в одной строке (например, "Этаж: Подвал | Кофе:
// 27")
void printTwoColumns(const std::string& left, const std::string& right) {
  std::string line = left;
  while (line.length() < 24) line += " ";
  line += right;
  while (line.length() < 48) line += " ";
  std::cout << "| " << line << " |\n";
}

// разбивает длинный текст на кусочки, чтобы он не вылезал за рамки
std::vector<std::string> wrapText(const std::string& text, int max_width) {
  std::vector<std::string> lines;
  std::istringstream words(text);
  std::string word;
  std::string current_line;

  while (words >> word) {
    if (static_cast<int>(word.length()) > max_width) {
      if (!current_line.empty()) {
        lines.push_back(current_line);
        current_line.clear();
      }
      for (size_t i = 0; i < word.length(); i += max_width) {
        lines.push_back(word.substr(i, max_width));
      }
      continue;
    }
    if (current_line.empty()) {
      current_line = word;
    } else if (static_cast<int>(current_line.length() + 1 + word.length()) <=
               max_width) {
      current_line += " " + word;
    } else {
      lines.push_back(current_line);
      current_line = word;
    }
  }
  if (!current_line.empty()) lines.push_back(current_line);
  return lines;
}

Renderer::Renderer() {}

// очищает экран консоли (только для Windows)
void Renderer::clearScreen() { system("cls"); }

// главный метод: рисует всё на экране, когда игрок не в бою
void Renderer::drawUI(std::string level_name, int coffee, int max_coffee,
                      int team_size, bool has_key, int current_point,
                      std::string plane_name, std::string event_text,
                      const std::vector<char>& markers, bool has_collectable,
                      bool collectable_taken) {
  clearScreen();

  std::string key_str = has_key ? "Yes" : "No";

  // статус секретного предмета на уровне
  std::string collectable_str = "None";
  if (has_collectable && !collectable_taken)
    collectable_str = "Not found";
  else if (has_collectable && collectable_taken)
    collectable_str = "Found";

  // верхняя панель с информацией
  std::cout << "+--------------------------------------------------+\n";

  printTwoColumns("FLOOR: " + level_name, "Coffee: " + std::to_string(coffee));
  printTwoColumns("Plane: " + plane_name, "Key: " + key_str);
  printTwoColumns("Band Size: " + std::to_string(team_size) + "/4",
                  "Collectable: " + collectable_str);

  std::cout << "+--------------------------------------------------+\n";
  std::cout << "|                                                  |\n";

  // рисуем карту: игрок отмечен звёздочкой, остальные точки - маркеры
  char n = (current_point == kPointNorthEdge) ? '*' : markers[kPointNorthEdge];
  char e = (current_point == kPointEastEdge) ? '*' : markers[kPointEastEdge];
  char s = (current_point == kPointSouthEdge) ? '*' : markers[kPointSouthEdge];
  char w = (current_point == kPointWestEdge) ? '*' : markers[kPointWestEdge];
  char c = (current_point == kPointCenter) ? '*' : markers[kPointCenter];

  printPaddedLine("                     " + std::string(1, n));
  printPaddedLine("                 " + std::string(1, w) + "   " +
                  std::string(1, c) + "   " + std::string(1, e));
  printPaddedLine("                     " + std::string(1, s));

  std::cout << "|                                                  |\n";

  // события и сообщения игроку (переносим длинные строки)
  std::vector<std::string> wrapped = wrapText(event_text, 44);
  for (const auto& line : wrapped) {
    printPaddedLine("  (" + line + ")");
  }
  if (wrapped.empty()) {
    printPaddedLine("  ()");
  }

  std::cout << "|                                                  |\n";
  std::cout << "+--------------------------------------------------+\n";

  // подсказки по управлению
  printPaddedLine("[WASD] Move         [F] Interact");
  printPaddedLine("[C] Switch roach    [X] View spells & notes");
  std::cout << "+--------------------------------------------------+\n";
}

// рисует экран боя с врагом
void Renderer::drawCombatUI(std::string level_name, int coffee, int max_coffee,
                            Enemy* enemy, std::string event_text) {
  clearScreen();

  std::cout << "+--------------------------------------------------+\n";
  printTwoColumns("FLOOR: " + level_name,
                  "Coffee(HP): " + std::to_string(coffee));
  std::cout << "+--------------------------------------------------+\n";
  std::cout << "|                                                  |\n";

  // информация о текущем враге
  printPaddedLine("           ENEMY: " + enemy->getType());
  printPaddedLine("           HP: " + std::to_string(enemy->getHealth()));
  printPaddedLine("           DMG: " + std::to_string(enemy->getDamage()));
  std::cout << "|                                                  |\n";

  // лог боевых событий
  std::vector<std::string> wrapped = wrapText(event_text, 44);
  for (const auto& line : wrapped) {
    printPaddedLine("  (" + line + ")");
  }
  if (wrapped.empty()) {
    printPaddedLine("  ()");
  }

  std::cout << "|                                                  |\n";
  std::cout << "+--------------------------------------------------+\n";

  // боевые подсказки
  printPaddedLine("[E] Atk / Ally Skill  [Q] Strong Atk (-2 coffee)");
  printPaddedLine("[C] Switch roach      [X] View spells & notes");
  printPaddedLine("Notes (RTY, GGHH) Cast Spell (Leader only)");
  std::cout << "+--------------------------------------------------+\n";
}
