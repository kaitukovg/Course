#include "Rectangle.h"
#include "logger.h"

// Конструктор прямоугольника
Rectangle::Rectangle(float x, float y, float width, float height, 
                     const sf::Color& color, const std::string& name,
                     VariableDatabase* db, const std::string& varName)
    : VisualObject(x, y, name, db),  // Конструктор родителя
      width(width), height(height),  // Сохраняем размеры
      defaultColor(color),           // Цвет по умолчанию
      variableName(varName) {        // Имя переменной (может быть пустым)
    
    // Настраиваем SFML прямоугольник
    shape.setPosition(x, y);                         // Позиция
    shape.setSize(sf::Vector2f(width, height));      // Размер
    shape.setFillColor(color);                       // Цвет
    
    // Подписываемся на изменения переменной (если она указана)
    if (!variableName.empty() && database) {
        database->subscribe(variableName, [this](double value) {
            this->update();  // При изменении переменной обновляем цвет
        });
    }
}

// Рисуем прямоугольник
void Rectangle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

// Обновляем цвет прямоугольника в зависимости от значения переменной
void Rectangle::update() {
    if (!variableName.empty() && database) {
        // Получаем текущее значение переменной
        double value = database->getVariable(variableName);
        
        // Начинаем с цвета по умолчанию
        sf::Color newColor = defaultColor;
        
        // Проверяем все условия
        for (const auto& condition : conditions) {
            if (value == condition.value) {
                // Нашли подходящее условие - устанавливаем соответствующий цвет
                newColor = condition.color;
                break;  // Прерываем поиск (первое подходящее условие)
            }
        }
        
        // Применяем новый цвет
        shape.setFillColor(newColor);
    }
}

// Добавляет условие: при указанном значении переменной установить указанный цвет
void Rectangle::addCondition(double value, const sf::Color& color) {
    conditions.push_back({value, color});
}