#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// --- ПЛОСКОСТИ ---
const int kPlaneFloor = 0;
const int kPlaneCeiling = 1;
const int kPlaneNorthWall = 2;
const int kPlaneSouthWall = 3;
const int kPlaneWestWall = 4;
const int kPlaneEastWall = 5;

// --- ТОЧКИ ---
const int kPointCenter = 0;
const int kPointNorthEdge = 1;
const int kPointEastEdge = 2;
const int kPointSouthEdge = 3;
const int kPointWestEdge = 4;

// --- РЕСУРСЫ ---
const int kStartingCoffee = 27;
const int kCoffeePickupAmount = 7;

// --- БОЁВКА ---
const int kStrongAttackCost = 2;
const int kStrongAttackMultiplier = 2.5;
const int kKeyDropChance = 30;
const int kMaxBandSize = 4;


// --- ВРАГИ (ГДД) ---
const int kEnemyAntHp = 10;
const int kEnemyAntDamage = 5;
const int kEnemyRoachHp = 12;
const int kEnemyRoachDamage = 5;
const int kEnemySpiderHp = 15;
const int kEnemySpiderDamage = 6;
const int kEnemyCatHp = 30;
const int kEnemyCatDamage = 7;
const int kEnemyDogHp = 40;
const int kEnemyDogDamage = 10;
const int kBossRatHp = 20;
const int kBossRatDamage = 6;
const int kBossFootHp = 60;
const int kBossFootDamage = 15;
const int kEnemyBugHp = 8;
const int kEnemyBugDamage = 4;

// --- ЗАКЛИНАНИЯ ---
const int kSpellSpitCost = 5;
const int kSpellSpitDamage = 6;
const int kSpellFirecrackerCost = 11;
const int kSpellFirecrackerDamage = 12;

// --- БАРЫГА И ЛОВУШКИ ---
const int kPriceNote = 10;
const int kPriceRoach = 15;
const int kCostCobweb = 2;
const int kCostStickyTape = 3;

// --- УЛУЧШЕНИЯ БАРЫГИ ---
const int kUpgradeMaxCoffeePrice = 20;
const int kUpgradeMaxCoffeeAmount = 7;
const int kUpgradeAttackPrice = 15;
const int kUpgradeAttackAmount = 4;

// --- ГЕНЕРАЦИЯ РЕСУРСОВ ---
const int kResourceSpawnChance = 45;
const int kRoachAllyChance = 20;
const int kCollectableChance = 10;
const int kSpellScrollChance = 5;

// --- ВЕНТИЛЯЦИЯ ---
const int kVentOpenCost = 10;
const int kVentSpawnChance = 5;

// Типы ресурсов (строки)
const char kResourceCoffee[] = "Coffee";
const char kResourceNote[] = "Note";
const char kResourceRoach[] = "RoachAlly";
const char kResourceCollectable[] = "Collectable";
const char kResourceSpellScroll[] = "SpellScroll";
const char kResourceVent[] = "Vent";

// Типы препятствий
const char kTrapCobweb[] = "Cobweb";
const char kTrapStickyTape[] = "StickyTape";
const char kTrapDichlorvos[] = "Dichlorvos";
const char kObstacleBoxes[] = "Boxes";

// Ноты
const char kNoteLetters[] = { 'R', 'T', 'Y', 'U', 'G', 'H', 'J', 'K' };
const int kNumNoteTypes = 8;

#endif  // CONSTANTS_H_