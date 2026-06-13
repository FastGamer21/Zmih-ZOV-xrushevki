#ifndef GAME_ENGINE_H_
#define GAME_ENGINE_H_

#include "band_manager.h"
#include "entities.h"
#include "map_system.h"
#include "note_inventory.h"
#include "player_logic.h"
#include "renderer.h"
#include "resources.h"

class CombatSystem;

// THE ONLY REASON WHY THIS HEART BEATS IS 5 GRADE FOR PRACTICE WORK
class GameEngine {
 public:
  GameEngine();
  ~GameEngine();
  void run();

 private:
  bool showMenu();
  void showEnding();
  void newGame();
  int getCollectableCount() const;

  void enterVent(Point* pt, CombatSystem& combat, int floor);

  PlayerController* player;
  Renderer* renderer;
  ResourceManager* resources;
  Cockroach* main_cockroach;
  BandManager band_manager;
  NoteInventory note_inventory;
  int collectable_count;

  bool is_running;
  bool stunned;
  CombatSystem* current_combat;
  std::string current_event;
};

#endif  // GAME_ENGINE_H_
