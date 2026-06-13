#include "note_inventory.h"

#include <cctype>

NoteInventory::NoteInventory() {}

// добавляет ноту в коллекцию, приводит к верхнему регистру
void NoteInventory::addNote(char note) {
  note = toupper(note);
  notes.insert(note);
}

// проверяет, есть ли у игрока все ноты для заклинания
bool NoteInventory::hasSequence(const std::string& sequence) const {
  for (char c : sequence) {
    char upper = toupper(c);
    if (notes.find(upper) == notes.end()) return false;
  }
  return true;
}

// ноты не расходуются, поэтому всегда возвращаем true
bool NoteInventory::spendSequence(const std::string& /*sequence*/) {
  return true;
}

// возвращает 1 если нота уже найдена, иначе 0
int NoteInventory::getNoteCount(char note) const {
  note = toupper(note);
  return (notes.find(note) != notes.end()) ? 1 : 0;
}

// красиво выводит список найденных нот в порядке R T Y U G H J K
std::string NoteInventory::toString() const {
  std::string result = "Notes found: ";
  const char order[] = {'R', 'T', 'Y', 'U', 'G', 'H', 'J', 'K'};
  bool first = true;
  for (char c : order) {
    if (notes.find(c) != notes.end()) {
      if (!first) result += " ";
      result += c;
      first = false;
    }
  }
  if (result == "Notes found: ") result += "none";
  return result;
}
