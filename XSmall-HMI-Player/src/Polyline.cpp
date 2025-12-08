#include "Polyline.h"
#include "logger.h"

// Конструктор ломаной
Polyline::Polyline(const std::vector<sf::Vector2f>& points, 
                   const sf::Color& color, const std::string& name,
                   VariableDatabase* db, const std::string& varName)
    : VisualObject(points.empty() ? 0 : points[0].x,  // X первой точки или 0
                   points.empty() ? 0 : points[0].y,  // Y первой точки или 0
                   name, db),                         // Имя и база данных
      color(color),                                   // Цвет
      variableName(varName) {                         // Имя переменной
    
    // Создаем вершины из переданных точек
    for (const auto& point : points) {
        this->points.push_back(sf::Vertex(point, color));
    }
    
    // Подписываемся на изменения переменной
    if (!variableName.empty() && database) {
        database->subscribe(variableName, [this](double value) {
            this->update();  // При изменении переменной обновляем ломаную
        });
    }
}

// Рисуем ломаную
void Polyline::draw(sf::RenderWindow& window) {
    if (points.size() > 1) {
        // Рисуем все точки как соединенную линию
        window.draw(&points[0], points.size(), sf::LineStrip);
    }
}

// Обновляем ломаную (если привязана к переменной)
void Polyline::update() {
    if (!variableName.empty() && database) {
        // Получаем историю переменной
        const auto& history = database->getHistory(variableName);
        
        if (history.size() > 1) {
            // Очищаем старые точки
            points.clear();
            
            // Находим диапазон значений для масштабирования
            float maxVal = *std::max_element(history.begin(), history.end());
            float minVal = *std::min_element(history.begin(), history.end());
            float range = maxVal - minVal;
            if (range == 0) range = 1;  // Избегаем деления на ноль
            
            // Фиксированные размеры для графика (можно было бы сделать параметрами)
            float xStep = 400.0f / (history.size() - 1);   // Шаг по X
            float yScale = 200.0f / range;                 // Масштаб по Y
            
            // Преобразуем значения истории в координаты
            for (size_t i = 0; i < history.size(); ++i) {
                float x = 20 + i * xStep;                    // X координата
                float y = 420 - (history[i] - minVal) * yScale;  // Y координата (инвертирована)
                points.push_back(sf::Vertex(sf::Vector2f(x, y), color));
            }
        }
    }
}

// Обновляет точки ломаной вручную
void Polyline::updatePoints(const std::vector<sf::Vector2f>& newPoints) {
    points.clear();  // Очищаем старые точки
    
    // Добавляем новые точки
    for (const auto& point : newPoints) {
        points.push_back(sf::Vertex(point, color));
    }
}