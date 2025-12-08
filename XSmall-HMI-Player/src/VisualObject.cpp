#include "VisualObject.h"

// Конструктор просто сохраняет переданные значения в поля класса
VisualObject::VisualObject(float x, float y, const std::string& name, VariableDatabase* db)
    : x(x), y(y), name(name), database(db) {
    // Здесь ничего особенного не делаем, только инициализируем поля
    // x(x) означает: поле x класса получает значение параметра x
}

// Меняем позицию объекта
void VisualObject::setPosition(float newX, float newY) {
    x = newX;  // Запоминаем новую X координату
    y = newY;  // Запоминаем новую Y координату
}

// Возвращаем имя объекта
std::string VisualObject::getName() const {
    return name;  // Просто возвращаем то, что хранится в поле name
}