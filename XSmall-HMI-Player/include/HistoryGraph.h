#ifndef HISTORYGRAPH_H
#define HISTORYGRAPH_H

#include "VisualObject.h"
#include "VariableDatabase.h"
#include <SFML/Graphics.hpp>
#include <vector>

// График, показывающий историю изменений переменной
// Например, как менялась температура за последние N секунд

class HistoryGraph : public VisualObject {
private:
    // Размеры графика
    float width, height;
    
    // Фон графика
    sf::RectangleShape background;
    
    // Имя переменной, историю которой показываем
    std::string variableName;
    
    // Максимальное количество точек на графике
    size_t maxHistorySize;
    
    // Цвет линии графика
    sf::Color lineColor;
    
    // Цвет сетки
    sf::Color gridColor;

public:
    // Конструктор
    HistoryGraph(float x, float y, float width, float height,
                 const std::string& name, VariableDatabase* db,
                 const std::string& varName = "", 
                 size_t maxHistory = 50,
                 const sf::Color& lineClr = sf::Color::Blue,
                 const sf::Color& gridClr = sf::Color(200, 200, 200, 100));
    
    // Методы родителя
    void draw(sf::RenderWindow& window) override;
    void update() override;
    
private:
    // Рисует сетку графика
    void drawGrid(sf::RenderWindow& window);
    
    // Рисует сам график (линию)
    void drawGraph(sf::RenderWindow& window);
};

#endif