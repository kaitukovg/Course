#ifndef SCENEFACTORY_H
#define SCENEFACTORY_H

#include <vector>
#include <memory>
#include <string>

// Предварительные объявления (чтобы не включать заголовочные файлы)
class VariableDatabase;
class VisualObject;

// Пространство имен SFML
namespace sf {
    class Font;
}

// Фабрика сцен - создает наборы визуальных объектов
// Раньше использовалась для создания демо-сцены, сейчас для загрузки из JSON

class SceneFactory {
public:
    // Загружает сцену из файла (устаревший метод, теперь используется JSONSceneLoader)
    static std::vector<std::unique_ptr<VisualObject>> loadFromFile(
        const std::string& filename, 
        VariableDatabase* db,
        sf::Font* font);
    
    // Создает тестовую сцену для демонстрации возможностей
    static std::vector<std::unique_ptr<VisualObject>> createDemoScene(
        VariableDatabase* db,
        sf::Font* font);
};

#endif