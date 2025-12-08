#include "Line.h"

// Конструктор линии
Line::Line(float x1, float y1, float x2, float y2, 
           const sf::Color& color, const std::string& name,
           VariableDatabase* db)
    : VisualObject(x1, y1, name, db),  // Конструктор родителя (используем x1,y1 как позицию)
      color(color) {                   // Сохраняем цвет
    
    // Настраиваем первую точку линии
    line[0] = sf::Vertex(sf::Vector2f(x1, y1), color);
    
    // Настраиваем вторую точку линии
    line[1] = sf::Vertex(sf::Vector2f(x2, y2), color);
}

// Рисуем линию
void Line::draw(sf::RenderWindow& window) {
    window.draw(line, 2, sf::Lines);  // Рисуем 2 вершины как линию
}