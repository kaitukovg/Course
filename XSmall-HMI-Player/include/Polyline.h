#ifndef POLYLINE_H
#define POLYLINE_H

#include "VisualObject.h"
#include "VariableDatabase.h"
#include <SFML/Graphics.hpp>
#include <vector>

// Ломаная линия (последовательность соединенных отрезков)
// Может обновляться динамически из истории переменной

class Polyline : public VisualObject {
private:
    // Массив точек (вершин) ломаной
    std::vector<sf::Vertex> points;
    
    // Цвет линии
    sf::Color color;
    
    // Имя переменной, из которой берутся данные
    std::string variableName;

public:
    // Конструктор
    Polyline(const std::vector<sf::Vector2f>& points, 
             const sf::Color& color, const std::string& name,
             VariableDatabase* db, const std::string& varName = "");
    
    // Методы родителя
    void draw(sf::RenderWindow& window) override;
    void update() override;
    
    // Обновляет точки ломаной (вручную)
    void updatePoints(const std::vector<sf::Vector2f>& newPoints);
};

#endif