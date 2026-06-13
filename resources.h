#ifndef RESOURCES_H_
#define RESOURCES_H_

// управляет ресурсом "кофе"
// кофе используется как здоровье, мана и валюта одновременно
class ResourceManager {
 public:
  ResourceManager(int initial_amount, int max_amount);

  void addCoffee(int amount);    // прибавить кофе (например, нашли на карте)
  bool spendCoffee(int amount);  // потратить кофе, если не хватает - ставит 0 и
                                 // возвращает false

  int getCurrentCoffee() const;
  int getMaxCoffee() const;
  bool isDead() const;  // проверка: кофе закончился?

  // методы для улучшений (оставлены для совместимости, но лимит кофе больше не
  // ограничивает)
  void increaseMaxCoffee(int amount);
  void setMaxCoffee(int new_max);

 private:
  int current_coffee;  // сколько кофе сейчас
  int max_coffee;      // максимальный лимит (теоретический, но на практике не
                       // ограничивает)
};

#endif
