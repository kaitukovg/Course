#include "HistoryGraph.h"
#include "logger.h"
#include <algorithm>

// Конструктор графика
HistoryGraph::HistoryGraph(float x, float y, float width, float height,
                           const std::string& name, VariableDatabase* db,
                           const std::string& varName, size_t maxHistory,
                           const sf::Color& lineClr, const sf::Color& gridClr)
    : VisualObject(x, y, name, db),  // Конструктор родителя
      width(width), height(height),  // Сохраняем размеры
      variableName(varName),         // Имя переменной
      maxHistorySize(maxHistory),    // Максимальная история
      lineColor(lineClr),            // Цвет линии
      gridColor(gridClr) {           // Цвет сетки
    
    // Настраиваем фон
    background.setPosition(x, y);                         // Позиция
    background.setSize(sf::Vector2f(width, height));      // Размер
    background.setFillColor(sf::Color::Transparent);      // Прозрачный фон
    background.setOutlineColor(sf::Color::Black);         // Черная рамка
    background.setOutlineThickness(1);                    // Толщина рамки
    
    // Подписываемся на изменения переменной
    if (!variableName.empty() && database) {
        database->subscribe(variableName, [this](double value) {
            this->update();  // При изменении переменной обновляем график
        });
    }
}

// Рисуем график
void HistoryGraph::draw(sf::RenderWindow& window) {
    window.draw(background);  // Сначала фон
    drawGrid(window);         // Потом сетку
    drawGraph(window);        // И сам график
}

// Обновляем график
void HistoryGraph::update() {
    // Обновление данных происходит через подписку на VariableDatabase
    // Фактическое обновление графика (перерисовка) делается в drawGraph()
}

// Рисует сетку графика
void HistoryGraph::drawGrid(sf::RenderWindow& window) {
    // Рисуем вертикальные линии сетки (4 секции)
    for (int i = 1; i < 4; ++i) {
        // Создаем линию от верхнего края до нижнего
        sf::Vertex verticalLine[] = {
            sf::Vertex(sf::Vector2f(x + i * width / 4, y), gridColor),
            sf::Vertex(sf::Vector2f(x + i * width / 4, y + height), gridColor)
        };
        window.draw(verticalLine, 2, sf::Lines);
    }
    
    // Рисуем горизонтальные линии сетки (тоже 4 секции)
    for (int i = 1; i < 4; ++i) {
        sf::Vertex horizontalLine[] = {
            sf::Vertex(sf::Vector2f(x, y + i * height / 4), gridColor),
            sf::Vertex(sf::Vector2f(x + width, y + i * height / 4), gridColor)
        };
        window.draw(horizontalLine, 2, sf::Lines);
    }
}

// Рисует сам график (линию)
void HistoryGraph::drawGraph(sf::RenderWindow& window) {
    if (!variableName.empty() && database) {
        // Получаем историю изменений переменной
        const auto& history = database->getHistory(variableName);
        
        if (history.size() > 1) {
            // Массив вершин для рисования линии
            std::vector<sf::Vertex> lineVertices;
            
            // Находим минимальное и максимальное значения для масштабирования
            float maxVal = *std::max_element(history.begin(), history.end());
            float minVal = *std::min_element(history.begin(), history.end());
            float range = maxVal - minVal;
            
            if (range == 0) range = 1;  // Избегаем деления на ноль
            
            // Рассчитываем шаг по X (расстояние между точками)
            float xStep = width / (history.size() - 1);
            
            // Создаем вершины для всех точек истории
            for (size_t i = 0; i < history.size(); ++i) {
                // Преобразуем значение в координаты Y
                // Ось Y инвертирована (0 вверху), поэтому вычитаем из высоты
                float xPos = x + i * xStep;
                float yPos = y + height - ((history[i] - minVal) / range * height);
                
                lineVertices.push_back(sf::Vertex(sf::Vector2f(xPos, yPos), lineColor));
            }
            
            // Рисуем линию через все точки
            if (lineVertices.size() > 1) {
                window.draw(&lineVertices[0], lineVertices.size(), sf::LineStrip);
            }
        }
    }

}
