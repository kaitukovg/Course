#ifndef HMIPLAYER_H
#define HMIPLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "VariableDatabase.h"  
#include "VisualObject.h"     

// Главный класс приложения - управляет окном, объектами, циклом обновления

class HmiPlayer {
private:
    sf::RenderWindow window;   // Главное окно приложения
    VariableDatabase database; // База данных переменных
    std::vector<std::unique_ptr<VisualObject>> objects;  // Все визуальные объекты
    sf::Font font;             // Основной шрифт
    
public:
    // Конструктор
    HmiPlayer();

    // Инициализация: загрузка шрифтов, создание объектов
    bool initialize();

    // Главный цикл приложения (вызывается из main)
    void run();

    // Методы главного цикла
    void handleEvents();  // Обработка событий (закрытие окна, клики и т.д.)
    void update();        // Обновление состояния объектов
    void render();        // Отрисовка объектов
    
    // Сохраняет демо-сцену в JSON файл
    bool saveSceneToJSON(const std::string& filename);
};


#endif
