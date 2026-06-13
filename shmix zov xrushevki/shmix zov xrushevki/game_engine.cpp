#include "game_engine.h"

#include <windows.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "combat_system.h"
#include "constants.h"
#include "map_system.h"
#include "merchant.h"

void delay(int ms) { Sleep(ms); }

void printSlow(const std::string& text, int delay_ms = 30) {
  for (char c : text) {
    std::cout << c << std::flush;
    delay(delay_ms);
  }
  std::cout << std::endl;
  delay(200);
}

void printLinesSlow(const std::vector<std::string>& lines,
                    int line_delay_ms = 500) {
  for (const auto& line : lines) {
    printSlow(line, 30);
    delay(line_delay_ms);
  }
}

GameEngine::GameEngine() {
  player = nullptr;
  renderer = nullptr;
  resources = nullptr;
  main_cockroach = nullptr;
  collectable_count = 0;
  is_running = false;
  stunned = false;
  current_combat = nullptr;
  current_event = "";
}

GameEngine::~GameEngine() {
  delete player;
  delete renderer;
  delete resources;
  delete main_cockroach;
}

bool GameEngine::showMenu() {
  renderer = new Renderer();
  renderer->clearScreen();

  std::vector<std::string> title = {"=====================================",
                                    "  Z.H.M.Y.H.: CALL OF KHRUSHCHEVKA   ",
                                    "=====================================",
                                    "",
                                    "A roach with a dream...",
                                    "",
                                    "1. New Game",
                                    "2. Exit"};
  printLinesSlow(title, 400);

  std::cout << "> ";
  std::string choice_str;
  std::cin >> choice_str;
  return (choice_str == "1");
}

void GameEngine::showEnding() {
  renderer->clearScreen();

  std::vector<std::string> ending_lines;
  ending_lines.push_back("========== ENDING ==========");

  int unique_notes = 0;
  for (int i = 0; i < kNumNoteTypes; ++i) {
    if (note_inventory.getNoteCount(kNoteLetters[i]) > 0) {
      unique_notes++;
    }
  }

  bool all_secrets = (collectable_count >= 3);
  bool all_notes = (unique_notes >= kNumNoteTypes);
  bool full_band = (band_manager.getSize() == kMaxBandSize);

  if (all_secrets && all_notes && full_band) {
    ending_lines.push_back("THE BEST ENDING!!!!!");
    ending_lines.push_back(
        "You collected 3 secrets, 8 notes, and formed a full band!");
    ending_lines.push_back("The roach band becomes a legend.");
  } else if (all_secrets || all_notes || full_band) {
    ending_lines.push_back("NORMAL ENDING");
    ending_lines.push_back(
        "You made good progress, but didn't 100% everything.");
    ending_lines.push_back(
        "The concert was nice, but something was missing...");
  } else {
    ending_lines.push_back("BAD ENDING");
    ending_lines.push_back("You didn't complete your collections.");
    ending_lines.push_back("The roaches play an empty show.");
  }

  ending_lines.push_back("");
  ending_lines.push_back("Secrets: " + std::to_string(collectable_count) +
                         "/3 | Notes: " + std::to_string(unique_notes) + "/" +
                         std::to_string(kNumNoteTypes) + " | Band: " +
                         std::to_string(band_manager.getSize()) + "/4");
  ending_lines.push_back("============================");
  ending_lines.push_back("Press Enter to return to menu...");

  printLinesSlow(ending_lines, 500);

  std::cin.ignore(10000, '\n');
  std::cin.get();
}

void GameEngine::newGame() {
  delete player;
  delete renderer;
  delete resources;
  delete main_cockroach;

  player = new PlayerController();
  renderer = new Renderer();

  resources = new ResourceManager(kStartingCoffee, 999);
  main_cockroach = new Cockroach("Leader", 4);
  collectable_count = 0;
  band_manager = BandManager();
  note_inventory = NoteInventory();
  band_manager.addRoach(main_cockroach);
  is_running = true;
  stunned = false;
  current_combat = nullptr;
  current_event = "Game Started! Find the boss and the pipe. Merchant is ($).";
}

int GameEngine::getCollectableCount() const { return collectable_count; }

void GameEngine::enterVent(Point* pt, CombatSystem& combat, int floor) {
  if (resources->getCurrentCoffee() < kVentOpenCost) {
    current_event = "Not enough coffee to open vent! Need " +
                    std::to_string(kVentOpenCost) + " coffee.";
    return;
  }
  resources->spendCoffee(kVentOpenCost);
  current_event = "You crawl into the vent... (-" +
                  std::to_string(kVentOpenCost) + " coffee)";

  int bonus = rand() % 4;
  std::string bonus_msg;
  switch (bonus) {
    case 0:
      resources->addCoffee(kCoffeePickupAmount);
      bonus_msg = "Found coffee! +" + std::to_string(kCoffeePickupAmount);
      break;
    case 1: {
      std::vector<char> floor_pool;
      if (floor == 0)
        floor_pool = {'R'};
      else if (floor == 1)
        floor_pool = {'T', 'Y', 'U'};
      else if (floor == 2)
        floor_pool = {'G', 'H'};
      else if (floor == 3)
        floor_pool = {'J', 'K'};
      else
        floor_pool = {'R'};

      std::vector<char> missing;
      for (char n : floor_pool) {
        if (note_inventory.getNoteCount(n) == 0) missing.push_back(n);
      }

      if (!missing.empty()) {
        char note = missing[rand() % missing.size()];
        note_inventory.addNote(note);
        bonus_msg = std::string("Found new note: ") + note;
      } else {
        char note = floor_pool[rand() % floor_pool.size()];
        resources->addCoffee(kCoffeePickupAmount);
        bonus_msg = std::string("Already had note ") + note +
                    ". Found coffee instead! +" +
                    std::to_string(kCoffeePickupAmount);
      }
      break;
    }
    case 2: {
      collectable_count++;
      bonus_msg = "Found a secret collectable! (" +
                  std::to_string(collectable_count) + " total)";
      break;
    }
    case 3: {
      Cockroach* ally = new Cockroach("VentRoach", 2 + (rand() % 4));
      band_manager.addRoach(ally);
      bonus_msg = "A lost roach joins your band from the vent!";
      break;
    }
  }
  current_event += " " + bonus_msg;
  pt->setVent(false);
}

// √Î‡‚Ì˚È ˆËÍÎ ‚ÒÂÈ Ë„˚ (√Œ—œŒƒ», ¡¿“»Õ ƒ¬»∆Œ  Œ“ NISSAN QASHQAI 2009 √Œƒ¿
// À≈√◊≈ —Œ¡–¿“‹)
void GameEngine::run() {
  while (true) {
    if (!showMenu()) break;
    newGame();

    CombatSystem combat(resources, band_manager.getActiveRoach(),
                        &note_inventory);
    current_combat = &combat;
    Merchant baryga;

    for (int floor = 0; floor <= 4; ++floor) {
      if (!is_running || resources->isDead()) break;

      Level current_level(floor);
      current_level.generateContent();

      if (floor == 0 || floor == 4)
        player->setAllowPlaneChange(false);
      else
        player->setAllowPlaneChange(true);

      player->setPosition(kPlaneFloor, kPointSouthEdge);

      Enemy* level_boss = nullptr;
      if (floor == 0)
        level_boss = new Enemy("Rat Boss", kBossRatHp, kBossRatDamage);
      else if (floor == 4)
        level_boss =
            new Enemy("Foot with Slipper", kBossFootHp, kBossFootDamage);
      else if (floor == 1)
        level_boss = new Enemy("Cat", kEnemyCatHp, kEnemyCatDamage);
      else if (floor == 2)
        level_boss = new Enemy("Dog", kEnemyDogHp, kEnemyDogDamage);
      else
        level_boss = new Enemy("Rival Roach", kEnemyRoachHp, kEnemyRoachDamage);

      Enemy* random_enemy = nullptr;
      bool has_key = false;
      bool level_completed = false;
      bool exit_confirm = false;
      bool exit_discovered = false;

      int enemies_remaining = 0;
      for (int i = 0; i < 6; ++i) {
        Plane* pl = current_level.getPlane(i);
        if (!pl) continue;
        for (int j = 0; j < 5; ++j) {
          Point* p = pl->getPoint(j);
          if (p && p->getHasEnemy()) enemies_remaining++;
        }
      }
      if (level_boss->isAlive()) enemies_remaining++;

      current_event = "Arrived at: " + current_level.getName();

      while (is_running && !resources->isDead() && !level_completed) {
        if (combat.isInCombat()) {
          renderer->drawCombatUI(current_level.getName(),
                                 resources->getCurrentCoffee(),
                                 resources->getMaxCoffee(),
                                 combat.getCurrentEnemy(), current_event);
          std::cout << "\n> Combat Action: ";
          std::string input;
          std::cin >> input;

          if (input == "C" || input == "c") {
            if (band_manager.getSize() > 1) {
              band_manager.switchNext();
              combat.setActiveRoach(band_manager.getActiveRoach());
              current_event = "Switched to " +
                              band_manager.getActiveRoach()->getName() +
                              ". Still your turn!";
            } else {
              current_event = "You have no other roaches in the band!";
            }
          } else if (input == "X" || input == "x") {
            if (band_manager.getActiveIndex() != 0) {
              current_event = "Only the Leader cockroach can cast spells!";
              continue;
            }
            std::cout << "\n" << combat.getSpellList();
            std::cout << combat.getNotesList() << "\n\n";
            std::cout
                << "Enter spell sequence to cast out of combat (0 to cancel): ";
            std::string seq;
            std::cin >> seq;
            if (seq == "0") {
              current_event = "Spell casting cancelled.";
            } else {
              current_event = combat.applySpellOutOfCombat(seq);
            }
          } else {
            current_event = combat.processTurn(input);
          }

          if (!combat.isInCombat()) {
            Enemy* defeated = combat.getCurrentEnemy();
            bool is_boss = (defeated == level_boss);

            if (enemies_remaining > 0) enemies_remaining--;

            if (!has_key) {
              bool drop_key = false;
              if (is_boss && (floor == 0 || floor == 4)) {
                drop_key = true;
              } else if (enemies_remaining == 0) {
                drop_key = true;
              } else {
                drop_key = (rand() % 100 < kKeyDropChance);
              }
              if (drop_key) {
                has_key = true;
                current_event += " You found a key!";
              } else {
                current_event += " No key this time.";
              }
            }

            if (!is_boss && random_enemy == defeated) {
              delete random_enemy;
              random_enemy = nullptr;
            }
          }
        } else {
          stunned = false;
          int band_size = band_manager.getSize();

          std::vector<char> map_markers(5, 'o');
          Plane* cur_plane = current_level.getPlane(player->getCurrentPlane());
          if (cur_plane) {
            for (int i = 0; i < 5; ++i) {
              Point* p = cur_plane->getPoint(i);
              if (player->getCurrentPlane() == current_level.getBossPlane() &&
                  i == current_level.getBossPoint() && level_boss->isAlive()) {
                map_markers[i] = 'B';
              } else if (player->getCurrentPlane() ==
                             current_level.getExitPlane() &&
                         i == current_level.getExitPoint() && exit_discovered) {
                map_markers[i] = '^';
              } else if (p && p->hasMerchant()) {
                map_markers[i] = '$';
              }
            }
          }

          renderer->drawUI(
              current_level.getName(), resources->getCurrentCoffee(),
              resources->getMaxCoffee(), band_size, has_key,
              player->getCurrentPoint(), player->getPlaneName(), current_event,
              map_markers, current_level.hasCollectableOnLevel(),
              current_level.isCollectableTaken());

          std::cout << "\n> Action (WASD, F, Q, C, X): ";
          std::string input;
          std::cin >> input;

          if (input == "Q" || input == "q") {
            is_running = false;
            renderer->clearScreen();
          } else if (input == "C" || input == "c") {
            if (band_manager.getSize() <= 1) {
              current_event =
                  "You need more roaches to switch! Buy them from the Merchant "
                  "($).";
              continue;
            }
            std::string menu_text = "=== Your Band ===\n";
            for (int i = 0; i < band_manager.getSize(); ++i) {
              Cockroach* roach = band_manager.getRoachByIndex(i);
              if (roach) {
                menu_text +=
                    std::to_string(i + 1) + ". " + roach->getName() +
                    " (DMG: " + std::to_string(roach->getPhysicalDamage()) +
                    ")\n";
              }
            }
            menu_text += "Enter number (0 to cancel): ";
            std::cout << menu_text;

            std::string choice_str;
            std::cin >> choice_str;
            int choice = -1;
            if (choice_str.length() == 1 && choice_str[0] >= '0' &&
                choice_str[0] <= '9') {
              choice = choice_str[0] - '0';
            }

            if (choice >= 1 && choice <= band_manager.getSize()) {
              band_manager.setActiveIndex(choice - 1);
              combat.setActiveRoach(band_manager.getActiveRoach());
              current_event = "Switched to roach: " +
                              band_manager.getActiveRoach()->getName();
            } else if (choice == 0) {
              current_event = "Band switching cancelled.";
            } else {
              current_event = "Invalid choice. Enter a valid number.";
            }
          } else if (input == "X" || input == "x") {
            if (band_manager.getActiveIndex() != 0) {
              current_event = "Only the Leader cockroach can cast spells!";
              continue;
            }
            std::cout << "\n" << combat.getSpellList();
            std::cout << combat.getNotesList() << "\n\n";
            std::cout
                << "Enter spell sequence to cast out of combat (0 to cancel): ";
            std::string seq;
            std::cin >> seq;
            if (seq == "0") {
              current_event = "Spell casting cancelled.";
            } else {
              current_event = combat.applySpellOutOfCombat(seq);
            }
          } else if (input == "F" || input == "f") {
            int p_plane = player->getCurrentPlane();
            int p_point = player->getCurrentPoint();
            Plane* plane = current_level.getPlane(p_plane);
            if (!plane) {
              current_event = "ERROR: Invalid plane. Resetting to floor.";
              player->setPosition(kPlaneFloor, kPointCenter);
              continue;
            }
            Point* pt = plane->getPoint(p_point);
            if (!pt) {
              current_event = "ERROR: Invalid point. Resetting to center.";
              player->setPosition(p_plane, kPointCenter);
              continue;
            }

            if (pt->hasMerchant()) {
              current_event = baryga.getMenuText();
              renderer->drawUI(
                  current_level.getName(), resources->getCurrentCoffee(),
                  resources->getMaxCoffee(), band_manager.getSize(), has_key,
                  player->getCurrentPoint(), player->getPlaneName(),
                  current_event, map_markers,
                  current_level.hasCollectableOnLevel(),
                  current_level.isCollectableTaken());
              std::cout << "\n> Buy (1-4 or 0 to cancel): ";
              std::string buy_input;
              std::cin >> buy_input;

              if (buy_input == "1") {
                std::string res = baryga.buyItem("1", resources);
                if (res == "Bought") {
                  std::vector<char> floor_pool;
                  if (floor == 0)
                    floor_pool = {'R'};
                  else if (floor == 1)
                    floor_pool = {'T', 'Y', 'U'};
                  else if (floor == 2)
                    floor_pool = {'G', 'H'};
                  else if (floor == 3)
                    floor_pool = {'J', 'K'};
                  else
                    floor_pool = {'R'};

                  std::vector<char> missing;
                  for (char n : floor_pool) {
                    if (note_inventory.getNoteCount(n) == 0)
                      missing.push_back(n);
                  }

                  if (!missing.empty()) {
                    char note = missing[rand() % missing.size()];
                    note_inventory.addNote(note);
                    current_event = "Bought Note " + std::string(1, note) + "!";
                  } else {
                    char note = floor_pool[rand() % floor_pool.size()];
                    resources->addCoffee(5);
                    current_event = "Already have " + std::string(1, note) +
                                    "! Refunded 5 coffee.";
                  }
                } else {
                  current_event = res;
                }
              } else if (buy_input == "2") {
                current_event = baryga.buyItem("2", resources);
                if (current_event.find("joined") != std::string::npos) {
                  Cockroach* new_roach = new Cockroach("Ally", 3);
                  band_manager.addRoach(new_roach);
                }
              } else if (buy_input == "3") {
                current_event = baryga.buyUpgradeMaxCoffee(resources);
              } else if (buy_input == "4") {
                int current_attack = main_cockroach->getPhysicalDamage();
                current_event =
                    baryga.buyUpgradeAttack(resources, current_attack);
                main_cockroach->setPhysicalDamage(current_attack);
              } else {
                current_event = "Trade cancelled.";
              }
            } else if (p_plane == current_level.getBossPlane() &&
                       p_point == current_level.getBossPoint() &&
                       level_boss->isAlive()) {
              combat.startCombat(level_boss);
              current_event = "BOSS BATTLE INITIATED!";
            } else if (current_level.getExitPlane() != -1 &&
                       p_plane == current_level.getExitPlane() &&
                       p_point == current_level.getExitPoint()) {
              exit_discovered = true;
              if (has_key) {
                if (!exit_confirm) {
                  exit_confirm = true;
                  if (floor == 4) {
                    current_event =
                        "Jump out the window? Press F to confirm, or move to "
                        "cancel.";
                  } else {
                    current_event =
                        "Enter the pipe? Press F to confirm, or move to "
                        "cancel.";
                  }
                } else {
                  level_completed = true;
                  if (floor == 4) {
                    current_event = "You jumped out into the night...";
                  } else {
                    current_event = "Pipe unlocked! Going up...";
                  }
                }
              } else {
                if (floor == 4) {
                  current_event = "The window is locked. Find a key!";
                } else {
                  current_event = "The pipe is locked. Find a key!";
                }
              }
            } else if (pt->isVent()) {
              enterVent(pt, combat, floor);
            } else if (pt->getHasEnemy()) {
              if (random_enemy != nullptr) delete random_enemy;
              random_enemy =
                  new Enemy("Random Bug", kEnemyBugHp, kEnemyBugDamage);
              combat.startCombat(random_enemy);
              pt->setHasEnemy(false);
              current_event = "AMBUSHED!";
            } else if (pt->hasResource()) {
              std::string res = pt->getResource();
              if (res == kResourceCoffee) {
                resources->addCoffee(kCoffeePickupAmount);
                current_event =
                    "Found coffee! +" + std::to_string(kCoffeePickupAmount);
              } else if (res == kResourceNote) {
                char nc = pt->getNoteChar();
                if (note_inventory.getNoteCount(nc) > 0) {
                  resources->addCoffee(kCoffeePickupAmount);
                  current_event =
                      "You already know this note! Found coffee instead! +" +
                      std::to_string(kCoffeePickupAmount);
                } else {
                  note_inventory.addNote(nc);
                  current_event = std::string("Found new note: ") + nc +
                                  "! You can now use it in spells.";
                }
              } else if (res == kResourceRoach) {
                Cockroach* ally = new Cockroach("BandMate", 2 + (rand() % 4));
                band_manager.addRoach(ally);
                current_event = "A wild roach joins your band!";
              } else if (res == kResourceCollectable) {
                collectable_count++;
                current_level.setCollectableTaken(true);
                current_event = "Found secret collectable!";
              } else if (res == kResourceSpellScroll) {
                std::string seq = pt->getSpellSequence();
                if (!seq.empty() && current_combat != nullptr) {
                  Spell* ns = nullptr;
                  if (seq == "U")
                    ns = new Spell("Healing Note", "U", 8, 10, EFFECT_HEAL);
                  else if (seq == "JKL")
                    ns = new Spell("Lullaby", "JKL", 12, 0, EFFECT_SLEEP);
                  else if (seq == "GHJ")
                    ns =
                        new Spell("Summon Roach", "GHJ", 10, 15, EFFECT_SUMMON);
                  if (ns) {
                    current_combat->learnSpell(ns);
                    current_event = "Learned spell: " + ns->getName();
                  }
                }
              }
              pt->clearResource();
            } else {
              current_event = "Nothing here.";
            }
          } else if (input == "W" || input == "w" || input == "A" ||
                     input == "a" || input == "S" || input == "s" ||
                     input == "D" || input == "d") {
            exit_confirm = false;

            int old_plane = player->getCurrentPlane();
            int old_point = player->getCurrentPoint();
            player->move(input);

            if (floor == 1 && old_plane == kPlaneFloor &&
                player->getCurrentPlane() == kPlaneFloor &&
                old_point != kPointCenter &&
                player->getCurrentPoint() == kPointCenter) {
              bool outward = false;
              if (old_point == kPointNorthEdge &&
                  (input == "W" || input == "w"))
                outward = true;
              if (old_point == kPointSouthEdge &&
                  (input == "S" || input == "s"))
                outward = true;
              if (old_point == kPointEastEdge && (input == "D" || input == "d"))
                outward = true;
              if (old_point == kPointWestEdge && (input == "A" || input == "a"))
                outward = true;
              if (outward) {
                current_event =
                    "The wall is covered in dust. You cannot climb it.";
              }
            }

            Plane* new_plane =
                current_level.getPlane(player->getCurrentPlane());
            if (!new_plane) {
              current_event = "ERROR: Invalid plane after move. Resetting.";
              player->setPosition(kPlaneFloor, kPointCenter);
              continue;
            }
            Point* cp = new_plane->getPoint(player->getCurrentPoint());
            if (!cp) {
              current_event = "ERROR: Invalid point after move. Resetting.";
              player->setPosition(kPlaneFloor, kPointCenter);
              continue;
            }
            std::string trap = cp->getTrap();

            if (trap == kObstacleBoxes) {
              if (resources->spendCoffee(3)) {
                cp->clearTrap();
                current_event = "You smashed through the boxes! (-3 coffee)";
              } else {
                player->setPosition(old_plane, old_point);
                current_event =
                    "Boxes block the way! Need 3 coffee to smash them.";
              }
            } else if (trap == kTrapDichlorvos) {
              if (resources->spendCoffee(20)) {
                cp->clearTrap();
                current_event =
                    "GAS! You hold your breath and run through! (-20 coffee)";
              } else {
                resources->spendCoffee(999);
                current_event = "GAS! Your lungs burn... INSTANT DEATH!";
              }
            } else if (trap == kTrapStickyTape) {
              resources->spendCoffee(kCostStickyTape);
              cp->clearTrap();
              current_event = "Stuck in tape! (-" +
                              std::to_string(kCostStickyTape) + " coffee).";
            } else if (trap == kTrapCobweb) {
              if (resources->spendCoffee(kCostCobweb)) {
                cp->clearTrap();
                current_event = "Broke cobweb! (-" +
                                std::to_string(kCostCobweb) + " coffee)";
              } else {
                player->setPosition(old_plane, old_point);
                current_event = "Cobweb blocks way! Need coffee to break.";
              }
            } else if (cp->getHasEnemy()) {
              current_event = "Enemy here! Press F to fight.";
            } else if (cp->isVent()) {
              current_event = "You see a vent. Press F to enter it.";
            } else if (cp->hasResource()) {
              current_event = "Found something! Press F to pick up.";
            } else if (cp->hasMerchant()) {
              current_event = "Merchant here. Press F to open shop.";
            } else if (current_level.getExitPlane() != -1 &&
                       player->getCurrentPlane() ==
                           current_level.getExitPlane() &&
                       player->getCurrentPoint() ==
                           current_level.getExitPoint()) {
              exit_discovered = true;
              if (floor == 4) {
                current_event = "You found the window! Press F to interact.";
              } else {
                current_event = "You found the exit pipe! Press F to interact.";
              }
            } else {
              current_event = "Stepped on: " + player->getPlaneName();
            }
          }
        }
      }
      delete level_boss;
      if (random_enemy) delete random_enemy;
    }

    if (resources->isDead()) {
      renderer->clearScreen();
      std::cout << "GAME OVER. You ran out of coffee.\nPress Enter...";
      std::cin.ignore(10000, '\n');
      std::cin.get();
    } else if (is_running) {
      showEnding();
    }
  }
  renderer->clearScreen();
  std::cout << "Thanks for playing!\n";
}
