#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "VisualObject.h"
#include "VariableDatabase.h"
#include <SFML/Graphics.hpp>

// Прямоугольник - может менять цвет в зависимости от значения переменной
// Используется для индикации состояния (например, зелёный - норма, красный - авария)

class Rectangle : public VisualObject {
private:
    // Размеры прямоугольника
    float width, height;
    
    // SFML объект для отрисовки
    sf::RectangleShape shape;
    
    // Цвет по умолчанию
    sf::Color defaultColor;
    
    // Имя связанной переменной (может быть пустым)
    std::string variableName;
    
    // Структура для условия изменения цвета
    struct ColorCondition {
        double value;     // При каком значении переменной
        sf::Color color;  // Какой цвет установить
    };
    
    // Список условий (если переменная = значению -> установить цвет)
    std::vector<ColorCondition> conditions;

public:
    // Конструктор
    Rectangle(float x, float y, float width, float height, 
              const sf::Color& color, const std::string& name,
              VariableDatabase* db, const std::string& varName = "");
    
    // Методы родителя
    void draw(sf::RenderWindow& window) override;
    void update() override;

    // Добавляет условие изменения цвета
    void addCondition(double value, const sf::Color& color);
};

#endif