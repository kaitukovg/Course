#ifndef LINE_H
#define LINE_H

#include "VisualObject.h"
#include <SFML/Graphics.hpp>

// Простая линия между двумя точками

class Line : public VisualObject {
private:
    // Две вершины линии (начало и конец)
    sf::Vertex line[2];
    
    // Цвет линии
    sf::Color color;

public:
    // Конструктор
    Line(float x1, float y1, float x2, float y2, 
         const sf::Color& color, const std::string& name,
         VariableDatabase* db);
    
    // Методы родителя
    void draw(sf::RenderWindow& window) override;
    void update() override {};  // Линия не обновляется
};

#endif