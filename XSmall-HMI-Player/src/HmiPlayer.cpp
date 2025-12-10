#include "HmiPlayer.h"
#include "SceneFactory.h"
#include "JSONSceneLoader.h"
#include "StateManager.h"
#include "logger.h"
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>

// Конструктор - создает окно
HmiPlayer::HmiPlayer() 
    : window(sf::VideoMode(1024, 768), "XSmall-HMI SCADA Player") {
    
    window.setFramerateLimit(60);  // Ограничиваем FPS для экономии ресурсов
}

// Инициализация приложения
bool HmiPlayer::initialize() {
    // Пробуем загрузить шрифт из нескольких возможных мест
    std::vector<std::string> fontPaths = {
        "../assets/fonts/helveticabold.ttf",  // На уровень выше
        "assets/fonts/helveticabold.ttf",     // В папке assets
        "assets/fonts/arial.ttf",             // Запасной шрифт
        "../../assets/fonts/helveticabold.ttf" // На два уровня выше
    };
    
    bool fontLoaded = false;
    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            Logger::info("Шрифт загружен из: " + path);
            fontLoaded = true;
            break;
        }
    }
    
    if (!fontLoaded) {
        Logger::error("Не удалось загрузить шрифт ни из одного пути");
        return false;
    }
    
    // Загружаем сохраненное состояние переменных
    StateManager stateManager;
    stateManager.loadState(database);
    
    // Определяем путь к корню проекта
    std::string projectRoot;
    
    // Пробуем несколько путей для поиска корня проекта
    std::vector<std::string> possibleRoots = {
        ".",                               // Текущая директория
        "..",                              // На уровень выше
        "../..",                           // На два уровня выше
        "C:/projects/XSmall-HMI-Player"    // Абсолютный путь (пример)
    };
    
    for (const auto& root : possibleRoots) {
        if (std::filesystem::exists(root + "/CMakeLists.txt") || 
            std::filesystem::exists(root + "/saved_state.json")) {
            projectRoot = root;
            Logger::info("Корень проекта найден в: " + projectRoot);
            break;
        }
    }
    
    if (projectRoot.empty()) {
        Logger::warning("Корень проекта не найден, использую текущую директорию");
        projectRoot = ".";
    }
    
    // Путь к файлу конфигурации объектов
    std::string jsonPath = projectRoot + "/objects.json";
    
    // ПРОВЕРКА 1: Если файла objects.json не существует
    if (!std::filesystem::exists(jsonPath)) {
        Logger::info("FILW objects.json NOT FOUND. CREATING A NEW SCADA DEMO-SCENE...");
        
        // Создаем файл с демо-сценой
        if (!saveSceneToJSON(jsonPath)) {
            Logger::error("FAILED TO CREATE objects.json FILE");
            return false;
        }
        
        Logger::info("FILE objects.json SUCCESSFULLY CREATED IN: " + jsonPath);
    }
    
    // Загружаем объекты из JSON файла
    Logger::info("LOADING SCENE FROM JSON: " + jsonPath);
    objects = JSONSceneLoader::loadFromFile(jsonPath, &database, &font);
    
    // ПРОВЕРКА 2: Если файл существует, но пустой (не загружены объекты)
    if (objects.empty()) {
        Logger::error("FILE objects.json EXISTS, BUT DOES NOT CONTAIN ANY OBJECTS OR EMPTY.");
        Logger::error("EDIT FILE " + jsonPath + " AND ADD NEW OBJECTS.");
        
        // Очищаем окно и показываем сообщение об ошибке
        return true; // Продолжаем запуск с пустым окном
    }
    
    Logger::info("LOADED OBJECTS FROM JSON: " + std::to_string(objects.size()));
    
    return true;
}

// Главный цикл приложения
void HmiPlayer::run() {
    StateManager stateManager;
    
    // Главный цикл - работает пока открыто окно
    while (window.isOpen()) {
        handleEvents();  // Обрабатываем события (клики, закрытие окна)
        update();        // Обновляем состояние объектов
        render();        // Рисуем все объекты
        
        // Автосохранение каждые 30 секунд
        static sf::Clock saveClock;
        if (saveClock.getElapsedTime().asSeconds() > 30.0) {
            stateManager.saveState(database);
            saveClock.restart();
        }
        
        // Небольшая задержка для контроля FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    // Сохраняем состояние при закрытии приложения
    stateManager.saveState(database);
}

// Обработка событий (закрытие окна, клики мыши и т.д.)
void HmiPlayer::handleEvents() {
    sf::Event event;
    
    // Обрабатываем все события в очереди
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();  // Закрываем окно
        }
        
        // Передаем события всем объектам для обработки
        for (auto& obj : objects) {
            obj->handleEvent(event, window);
        }
    }
}

// Обновление состояния объектов
void HmiPlayer::update() {
    static sf::Clock updateClock;

    // Обновляем объекты каждые 100 мс (10 раз в секунду)
    if (updateClock.getElapsedTime().asMilliseconds() > 100) {
        for (auto& obj : objects) {
            obj->update();
        }
        updateClock.restart();
    }
    
    // Демо-логика: плавное изменение температуры к заданной уставке
    static sf::Clock demoClock;
    if (demoClock.getElapsedTime().asSeconds() > 0.2) {
        double currentTemp = database.getVariable("temperature_value");
        double setpoint = database.getVariable("setpoint_value");
        
        // Вычисляем разницу между текущей температурой и уставкой
        double difference = setpoint - currentTemp;
        double change = 0.0;
        
        // Адаптивный шаг изменения: чем больше разница, тем быстрее меняем
        if (std::abs(difference) > 1.0) {
            change = (difference > 0) ? 0.2 : -0.2;  // Большая разница - шаг 0.2
        } else if (std::abs(difference) > 0.2) {
            change = difference * 0.5;  // Средняя разница - шаг 0.5 от разницы
        } else {
            change = difference * 0.8;  // Малая разница - почти полностью компенсируем
        }
        
        // Применяем изменение
        double newTemp = currentTemp + change;
        database.setVariable("temperature_value", newTemp);
        
        // Добавляем в историю для графика
        database.addToHistory("temperature_history", newTemp);
        
        demoClock.restart();
    }
}

// Отрисовка всех объектов
void HmiPlayer::render() {
    // Очищаем окно темно-синим цветом
    window.clear(sf::Color(16, 41, 79));
    
    // Рисуем все объекты в порядке их добавления
    for (auto& obj : objects) {
        obj->draw(window);
    }
    
    // Показываем нарисованное
    window.display();
}

bool HmiPlayer::saveSceneToJSON(const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            Logger::error("UNABLE TO CREATE JSON FILE: " + filename);
            return false;
        }
        
        // Записываем полный JSON с демо-сценой
        file << R"({
    "objects": [
        {
            "type": "Rectangle",
            "name": "Status Panel",
            "x": 20,
            "y": 50,
            "width": 500,
            "height": 200,
            "color": [173, 216, 230],
            "variable": "panel_status",
            "conditions": [
                {"value": 0, "color": [124, 36, 179]},
                {"value": 1, "color": [199, 24, 88]},
                {"value": 2, "color": [72, 146, 163]},
                {"value": 3, "color": [176, 12, 160]},
                {"value": 5, "color": [176, 80, 12]},
                {"value": 6, "color": [22, 219, 219]},
                {"value": 7, "color": [255, 0, 255]},
                {"value": 8, "color": [0, 0, 0]},
                {"value": 9, "color": [255, 0, 0]}
            ]
        },
        {
            "type": "Text",
            "name": "Temperature Text",
            "x": 75,
            "y": 70,
            "content": "Temperature: ",
            "fontSize": 40,
            "color": [255, 255, 255],
            "variable": "temperature_value",
            "format": "Temperature: "
        },
        {
            "type": "Text",
            "name": "Pressure Text",
            "x": 150,
            "y": 125,
            "content": "Pressure: ",
            "fontSize": 25,
            "color": [255, 255, 255],
            "variable": "pressure_value",
            "format": "Pressure: "
        },
        {
            "type": "Text",
            "name": "Setpoint Text",
            "x": 170,
            "y": 160,
            "content": "Setpoint: ",
            "fontSize": 20,
            "color": [255, 255, 255],
            "variable": "setpoint_value",
            "format": "Setpoint: "
        },
        {
            "type": "InputField",
            "name": "Setpoint Input",
            "x": 40,
            "y": 200,
            "width": 200,
            "height": 40,
            "fontSize": 23,
            "variable": "setpoint_value"
        },
        {
            "type": "Button",
            "name": "Apply Button",
            "x": 250,
            "y": 200,
            "width": 100,
            "height": 40,
            "text": "Apply",
            "fontSize": 23,
            "color": [45, 15, 127],
            "textColor": [255, 255, 255],
            "action": "apply"
        },
        {
            "type": "Line",
            "name": "Separator Line 1",
            "x": 0,
            "y": 280,
            "x2": 1024,
            "y2": 280,
            "color": [255, 255, 255]
        },
        {
            "type": "Line",
            "name": "Separator Line 2",
            "x": 0,
            "y": 285,
            "x2": 1024,
            "y2": 285,
            "color": [255, 255, 255]
        },
        {
            "type": "Line",
            "name": "Separator Line 3",
            "x": 0,
            "y": 290,
            "x2": 1024,
            "y2": 290,
            "color": [255, 255, 255]
        },
        {
            "type": "Rectangle",
            "name": "Graph Panel",
            "x": 20,
            "y": 350,
            "width": 400,
            "height": 200,
            "color": [0, 0, 0],
            "variable": ""
        },
        {
            "type": "HistoryGraph",
            "name": "Temperature Graph",
            "x": 20,
            "y": 350,
            "width": 400,
            "height": 200,
            "variable": "temperature_history",
            "maxHistory": 50,
            "lineColor": [0, 255, 26],
            "gridColor": [200, 200, 200, 100]
        },
        {
            "type": "Image",
            "name": "Logo Image",
            "x": 720,
            "y": 330,
            "width": 175,
            "height": 218.5,
            "path": "assets/images/logo.png"
        },
        {
            "type": "Button",
            "name": "Change Color",
            "x": 450,
            "y": 360,
            "width": 180,
            "height": 50,
            "text": "Change Color",
            "fontSize": 28,
            "color": [231, 214, 191],
            "textColor": [10, 35, 79],
            "action": "change_color"
        },
        {
            "type": "Button",
            "name": "Temp Increase",
            "x": 450,
            "y": 420,
            "width": 80,
            "height": 30,
            "text": "Temp +",
            "fontSize": 22,
            "color": [217, 72, 28],
            "textColor": [255, 255, 255],
            "action": "increase_temp"
        },
        {
            "type": "Button",
            "name": "Temp Decrease",
            "x": 540,
            "y": 420,
            "width": 80,
            "height": 30,
            "text": "Temp -",
            "fontSize": 22,
            "color": [0, 178, 232],
            "textColor": [255, 255, 255],
            "action": "decrease_temp"
        },
        {
            "type": "Button",
            "name": "Pressure Increase",
            "x": 450,
            "y": 470,
            "width": 80,
            "height": 30,
            "text": "Press +",
            "fontSize": 14,
            "color": [143, 0, 232],
            "textColor": [255, 255, 255],
            "action": "increase_pressure"
        },
        {
            "type": "Button",
            "name": "Pressure Decrease",
            "x": 540,
            "y": 470,
            "width": 80,
            "height": 30,
            "text": "Press -",
            "fontSize": 14,
            "color": [179, 73, 245],
            "textColor": [255, 255, 255],
            "action": "decrease_pressure"
        },
        {
            "type": "Text",
            "name": "System Title",
            "x": 550,
            "y": 20,
            "content": "XSmall HMI Player",
            "fontSize": 60,
            "color": [249, 250, 197],
            "variable": "",
            "format": ""
        },
        {
            "type": "Text",
            "name": "System Subtitle",
            "x": 650,
            "y": 90,
            "content": "SCADA System",
            "fontSize": 40,
            "color": [255, 255, 255],
            "variable": "",
            "format": ""
        }
    ],
    "metadata": {
        "version": "1.0",
        "description": "XSmall-HMI SCADA Interface Configuration",
        "window": {
            "width": 1024,
            "height": 768,
            "title": "XSmall-HMI SCADA Player"
        }
    }
})";
        
        file.close();
        Logger::info("DEMO-SCENE IS SAVED IN: " + filename);
        Logger::info("THIS FILE CAN BE USED AS A TEMPLATE TO CREATE YOUR OWN SCENES.");
        return true;
        
    } catch (const std::exception& e) {
        Logger::error("ERROR WHILE SAVED JSON FILE: " + std::string(e.what()));
        return false;
    }
}

