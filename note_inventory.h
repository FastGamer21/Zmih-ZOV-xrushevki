#ifndef NOTE_INVENTORY_H_
#define NOTE_INVENTORY_H_

#include <set>
#include <string>

// инвентарь для хранения найденных нот
// всего нот 8 штук: R T Y U G H J K
// каждая нота встречается только один раз и не тратится при использовании
class NoteInventory {
 public:
  NoteInventory();
  void addNote(char note);  // добавляет ноту, если её ещё нет
  bool hasSequence(const std::string& sequence)
      const;  // проверяет, есть ли все ноты для заклинания
  bool spendSequence(
      const std::string& sequence);   // заглушка: ноты не тратятся
  int getNoteCount(char note) const;  // возвращает 1 если нота есть, 0 если нет
  std::string toString() const;       // выводит список найденных нот
 private:
  std::set<char> notes;  // множество найденных нот (без повторов)
};

#endif
