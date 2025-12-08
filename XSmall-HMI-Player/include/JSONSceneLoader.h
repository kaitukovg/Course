// #ifndef JSONSCENELOADER_H
// #define JSONSCENELOADER_H

// #include <vector>
// #include <memory>
// #include <string>
// #include <unordered_map>
// #include "VisualObject.h"
// #include "VariableDatabase.h"
// #include <SFML/Graphics.hpp>

// /**
//  * Загружает и создает объекты из JSON файла
//  */
// class JSONSceneLoader {
// public:
//     // Загружает объекты из JSON файла
//     static std::vector<std::unique_ptr<VisualObject>> loadFromFile(
//         const std::string& filename, 
//         VariableDatabase* db,
//         sf::Font* font);
    
//     // Создает файл objects.json с демо-сценой
//     static bool createDefaultJSONFile(const std::string& filename);
    
// private:
//     // Вспомогательные функции для создания объектов из JSON
//     static std::unique_ptr<VisualObject> createRectangle(
//         const std::unordered_map<std::string, std::string>& properties,
//         VariableDatabase* db,
//         sf::Font* font);
    
//     static std::unique_ptr<VisualObject> createText(
//         const std::unordered_map<std::string, std::string>& properties,
//         VariableDatabase* db,
//         sf::Font* font);
    
//     static std::unique_ptr<VisualObject> createLine(
//         const std::unordered_map<std::string, std::string>& properties,
//         VariableDatabase* db,
//         sf::Font* font);
    
//     static std::unique_ptr<VisualObject> createInputField(
//         const std::unordered_map<std::string, std::string>& properties,
//         VariableDatabase* db,
//         sf::Font* font);
    
//     static std::unique_ptr<VisualObject> createButton(
//         const std::unordered_map<std::string, std::string>& properties,
//         VariableDatabase* db,
//         sf::Font* font);
    
//     static std::unique_ptr<VisualObject> createHistoryGraph(
//         const std::unordered_map<std::string, std::string>& properties,
//         VariableDatabase* db,
//         sf::Font* font);
    
//     static std::unique_ptr<VisualObject> createImage(
//         const std::unordered_map<std::string, std::string>& properties,
//         VariableDatabase* db,
//         sf::Font* font);
    
//     static std::unique_ptr<VisualObject> createPolyline(
//         const std::unordered_map<std::string, std::string>& properties,
//         VariableDatabase* db,
//         sf::Font* font);
    
//     // Вспомогательные функции парсинга
//     static sf::Color parseColor(const std::string& colorStr);
//     static std::vector<double> parseDoubleArray(const std::string& arrayStr);
//     static std::vector<std::string> split(const std::string& str, char delimiter);
//     static std::string trim(const std::string& str);
//     static std::string unquote(const std::string& str);
//     static std::unordered_map<std::string, std::string> parseObject(const std::string& jsonStr);
//     static std::vector<std::string> parseArray(const std::string& jsonStr);
// };

// #endif

#ifndef JSONSCENELOADER_H
#define JSONSCENELOADER_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "VisualObject.h"
#include "VariableDatabase.h"
#include <SFML/Graphics.hpp>

/**
 * Загружает объекты из JSON файла
 */
class JSONSceneLoader {
public:
    // Загружает объекты из JSON файла
    static std::vector<std::unique_ptr<VisualObject>> loadFromFile(
        const std::string& filename, 
        VariableDatabase* db,
        sf::Font* font);
    
private:
    // Вспомогательные функции для создания объектов
    static std::unique_ptr<VisualObject> createRectangle(
        const std::unordered_map<std::string, std::string>& props,
        VariableDatabase* db,
        sf::Font* font);
    
    static std::unique_ptr<VisualObject> createText(
        const std::unordered_map<std::string, std::string>& props,
        VariableDatabase* db,
        sf::Font* font);
    
    static std::unique_ptr<VisualObject> createLine(
        const std::unordered_map<std::string, std::string>& props,
        VariableDatabase* db,
        sf::Font* font);
    
    static std::unique_ptr<VisualObject> createInputField(
        const std::unordered_map<std::string, std::string>& props,
        VariableDatabase* db,
        sf::Font* font);
    
    static std::unique_ptr<VisualObject> createButton(
        const std::unordered_map<std::string, std::string>& props,
        VariableDatabase* db,
        sf::Font* font);
    
    static std::unique_ptr<VisualObject> createHistoryGraph(
        const std::unordered_map<std::string, std::string>& props,
        VariableDatabase* db,
        sf::Font* font);
    
    static std::unique_ptr<VisualObject> createImage(
        const std::unordered_map<std::string, std::string>& props,
        VariableDatabase* db,
        sf::Font* font);
    
    // Вспомогательная функция для парсинга цвета
    static sf::Color parseColor(const std::string& colorStr);
};

#endif