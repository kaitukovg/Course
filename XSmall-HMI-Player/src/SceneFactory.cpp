#include "SceneFactory.h"
#include "Rectangle.h"
#include "Text.h"
#include "Line.h"
#include "Polyline.h"
#include "InputField.h"
#include "Button.h"
#include "HistoryGraph.h"
#include "Image.h"
#include "logger.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

// Создаем демо-сцену (тестовый интерфейс)
std::vector<std::unique_ptr<VisualObject>> SceneFactory::createDemoScene(
    VariableDatabase* db,
    sf::Font* font) {
    
    // Вектор для хранения объектов сцены
    std::vector<std::unique_ptr<VisualObject>> objects;
    
    Logger::info("Создаю демо SCADA сцену");
    
    // 1. Панель статуса (прямоугольник, меняющий цвет)
    auto panel = std::make_unique<Rectangle>(20, 50, 500, 200, 
        sf::Color(173, 216, 230), "Status Panel", db, "panel_status");
    
    // Добавляем условия изменения цвета (при разных значениях переменной panel_status)
    panel->addCondition(0, sf::Color(124, 36, 179));   // Фиолетовый
    panel->addCondition(1, sf::Color(199, 24, 88));    // Красный
    panel->addCondition(2, sf::Color(72, 146, 163));   // Голубой
    panel->addCondition(3, sf::Color(176, 12, 160));   // Розовый
    panel->addCondition(5, sf::Color(176, 80, 12));    // Оранжевый
    panel->addCondition(6, sf::Color(22, 219, 219));   // Бирюзовый
    panel->addCondition(7, sf::Color::Magenta);        // Пурпурный
    panel->addCondition(8, sf::Color::Black);          // Черный
    panel->addCondition(9, sf::Color::Red);            // Красный
    objects.push_back(std::move(panel));
    
    // 2. Текст температуры (динамический, показывает значение переменной)
    auto tempText = std::make_unique<Text>(75, 70, "Temperature: ", 
        font, 40, sf::Color::White, "Temperature Text", db, 
        "temperature_value", "Temperature:  ");
    objects.push_back(std::move(tempText));
    
    // 3. Текст давления
    auto pressureText = std::make_unique<Text>(150, 125, "Pressure: ",
        font, 25, sf::Color::White, "Pressure Text", db,
        "pressure_value", "Pressure:  ");
    objects.push_back(std::move(pressureText));
    
    // 4. Текст уставки (желаемое значение)
    auto setpointText = std::make_unique<Text>(170, 160, "Setpoint: ",
        font, 20, sf::Color::White, "Setpoint Text", db,
        "setpoint_value", "Setpoint: ");
    objects.push_back(std::move(setpointText));
    
    // 5. Поле ввода для уставки
    auto inputField = std::make_unique<InputField>(40, 200, 200, 40,
        font, 23, "Setpoint Input", db, "setpoint_value");
    objects.push_back(std::move(inputField));
    
    // 6. Кнопка Apply (применить)
    auto applyButton = std::make_unique<Button>(250, 200, 100, 40,
        "Apply", font, 23, sf::Color(45, 15, 127), "Apply Button", db,
        "", []() { Logger::info("Нажата кнопка Apply"); },  // Обработчик клика
        sf::Color::White);  
    objects.push_back(std::move(applyButton));
    
    // 7. Линии-разделители (три белые линии подряд)
    auto line1 = std::make_unique<Line>(0, 280, 1024, 280, 
        sf::Color::White, "Separator Line 1", db);
    objects.push_back(std::move(line1));

    auto line2 = std::make_unique<Line>(0, 285, 1024, 285, 
        sf::Color::White, "Separator Line 2", db);
    objects.push_back(std::move(line2));

    auto line3 = std::make_unique<Line>(0, 290, 1024, 290, 
        sf::Color::White, "Separator Line 3", db);
    objects.push_back(std::move(line3));
    
    // 8. Панель графика (черный фон для графика)
    auto panel1 = std::make_unique<Rectangle>(20, 350, 400, 200, 
        sf::Color(0, 0, 0), "Graph Panel", db, "");
    objects.push_back(std::move(panel1));
    
    // 9. График истории температуры
    auto graph = std::make_unique<HistoryGraph>(20, 350, 400, 200,
        "Temperature Graph", db, "temperature_history", 50, sf::Color(0, 255, 26));
    objects.push_back(std::move(graph));
    
    // 10. Логотип (изображение)
    auto image = std::make_unique<Image>(720, 330, 175, 218.5,
        "assets/images/logo.png", "Logo Image", db);
    objects.push_back(std::move(image));
    
    // 11. Кнопка изменения цвета панели статуса
    auto statusButton = std::make_unique<Button>(450, 360, 180, 50,
        "Change Color", font, 28, sf::Color(231, 214, 191), "Change Color", db,
        "", [db]() {
            static int status = 0;  // Статическая переменная - запоминает состояние между вызовами
            status = (status + 1) % 10;  // Увеличиваем на 1, но не больше 9 (зацикливаем)
            db->setVariable("panel_status", static_cast<double>(status));
            Logger::info("Нажата кнопка Change Color. Статус панели изменен на: " + std::to_string(status));
        },
        sf::Color(10, 35, 79));  
    objects.push_back(std::move(statusButton));
    
    // 12. Кнопка увеличения температуры
    auto tempUpButton = std::make_unique<Button>(450, 420, 80, 30,
        "Temp +", font, 22, sf::Color(217, 72, 28), "Temp Increase", db,
        "", [db]() {
            double current = db->getVariable("temperature_value");
            db->setVariable("temperature_value", current + 1.0);
            Logger::info("Температура увеличена до: " + std::to_string(current + 1.0));
        },
        sf::Color::White);
    objects.push_back(std::move(tempUpButton));
    
    // 13. Кнопка уменьшения температуры
    auto tempDownButton = std::make_unique<Button>(540, 420, 80, 30,
        "Temp -", font, 22, sf::Color(0, 178, 232), "Temp Decrease", db,
        "", [db]() {
            double current = db->getVariable("temperature_value");
            db->setVariable("temperature_value", current - 1.0);
            Logger::info("Температура уменьшена до: " + std::to_string(current - 1.0));
        },
        sf::Color::White);  
    objects.push_back(std::move(tempDownButton));
    
    // 14. Кнопка увеличения давления
    auto pressureUpButton = std::make_unique<Button>(450, 470, 80, 30,
        "Press +", font, 14, sf::Color(143, 0, 232), "Pressure Increase", db,
        "", [db]() {
            double current = db->getVariable("pressure_value");
            db->setVariable("pressure_value", current + 0.5);
            Logger::info("Давление увеличено до: " + std::to_string(current + 0.5));
        },
        sf::Color::White);
    objects.push_back(std::move(pressureUpButton));

    // 15. Кнопка уменьшения давления
    auto pressureDownButton = std::make_unique<Button>(540, 470, 80, 30,
        "Press -", font, 14, sf::Color(179, 73, 245), "Pressure Decrease", db,
        "", [db]() {
            double current = db->getVariable("pressure_value");
            db->setVariable("pressure_value", current - 0.5);
            Logger::info("Давление уменьшено до: " + std::to_string(current - 0.5));
        },
        sf::Color::White);
    objects.push_back(std::move(pressureDownButton));
    
    // 16. Заголовок системы (статический текст)
    auto titleText = std::make_unique<Text>(550, 20, "XSmall HMI Player", 
        font, 60, sf::Color(249, 250, 197), "System Title", db);
    objects.push_back(std::move(titleText));
    
    // 17. Подзаголовок системы
    auto subtitleText = std::make_unique<Text>(650, 90, "SCADA System", 
        font, 40, sf::Color::White, "System Subtitle", db);
    objects.push_back(std::move(subtitleText));

    Logger::info("Демо сцена создана, объектов: " + std::to_string(objects.size()));
    
    return objects;
}

// Загрузка сцены из файла (устаревший метод)
std::vector<std::unique_ptr<VisualObject>> SceneFactory::loadFromFile(
    const std::string& filename, 
    VariableDatabase* db,
    sf::Font* font) {
    
    // Возвращаем пустой вектор - теперь загрузка через JSONSceneLoader
    // Этот метод оставлен для совместимости
    return std::vector<std::unique_ptr<VisualObject>>();
}