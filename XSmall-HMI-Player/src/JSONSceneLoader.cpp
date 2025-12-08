#include "JSONSceneLoader.h"
#include "Rectangle.h"
#include "Text.h"
#include "Line.h"
#include "Polyline.h"
#include "InputField.h"
#include "Button.h"
#include "HistoryGraph.h"
#include "Image.h"
#include "logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace {
    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }
    
    std::string unquote(const std::string& str) {
        if (str.length() >= 2 && str[0] == '"' && str[str.length()-1] == '"') {
            return str.substr(1, str.length() - 2);
        }
        return str;
    }
    
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string item;
        
        while (std::getline(ss, item, delimiter)) {
            if (!item.empty()) {
                result.push_back(trim(item));
            }
        }
        
        return result;
    }
    
    // Вспомогательная функция для парсинга массива чисел
    std::vector<double> parseDoubleArray(const std::string& str) {
        std::vector<double> result;
        std::string content = trim(str);
        
        if (content.empty() || content[0] != '[' || content[content.length()-1] != ']') {
            return result;
        }
        
        // Убираем скобки
        content = content.substr(1, content.length() - 2);
        
        // Разделяем по запятым
        auto parts = split(content, ',');
        for (const auto& part : parts) {
            try {
                result.push_back(std::stod(trim(part)));
            } catch (const std::exception& e) {
                // Пропускаем некорректные значения
            }
        }
        
        return result;
    }
}

sf::Color JSONSceneLoader::parseColor(const std::string& colorStr) {
    auto numbers = parseDoubleArray(colorStr);
    if (numbers.size() >= 3) {
        if (numbers.size() >= 4) {
            return sf::Color(
                static_cast<sf::Uint8>(numbers[0]),
                static_cast<sf::Uint8>(numbers[1]),
                static_cast<sf::Uint8>(numbers[2]),
                static_cast<sf::Uint8>(numbers[3])
            );
        }
        return sf::Color(
            static_cast<sf::Uint8>(numbers[0]),
            static_cast<sf::Uint8>(numbers[1]),
            static_cast<sf::Uint8>(numbers[2])
        );
    }
    return sf::Color::White;
}

std::vector<std::unique_ptr<VisualObject>> JSONSceneLoader::loadFromFile(
    const std::string& filename, 
    VariableDatabase* db,
    sf::Font* font) {
    
    std::vector<std::unique_ptr<VisualObject>> objects;
    
    if (!std::filesystem::exists(filename)) {
        Logger::warning("JSON config file not found: " + filename);
        return objects;
    }
    
    try {
        // Открываем и читаем файл
        std::ifstream file(filename);
        if (!file.is_open()) {
            Logger::error("Cannot open JSON file: " + filename);
            return objects;
        }
        
        // Читаем весь файл
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();
        
        // Упрощенный парсинг JSON - ищем объекты вручную
        size_t pos = 0;
        while ((pos = content.find("\"type\"", pos)) != std::string::npos) {
            // Находим начало объекта
            size_t objStart = content.rfind('{', pos);
            if (objStart == std::string::npos) break;
            
            // Находим конец объекта
            size_t braceCount = 1;
            size_t objEnd = objStart + 1;
            while (objEnd < content.length() && braceCount > 0) {
                if (content[objEnd] == '{') braceCount++;
                else if (content[objEnd] == '}') braceCount--;
                objEnd++;
            }
            
            if (braceCount != 0) {
                Logger::error("Invalid JSON: unbalanced braces");
                break;
            }
            
            std::string objStr = content.substr(objStart, objEnd - objStart);
            
            // Извлекаем тип объекта
            size_t typePos = objStr.find("\"type\"");
            if (typePos == std::string::npos) {
                pos = objEnd;
                continue;
            }
            
            size_t colonPos = objStr.find(':', typePos);
            size_t quoteStart = objStr.find('"', colonPos);
            size_t quoteEnd = objStr.find('"', quoteStart + 1);
            
            if (quoteStart == std::string::npos || quoteEnd == std::string::npos) {
                pos = objEnd;
                continue;
            }
            
            std::string type = objStr.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
            
            // Парсим остальные свойства
            std::unordered_map<std::string, std::string> props;
            
            // Извлекаем свойства
            size_t propPos = 0;
            while ((propPos = objStr.find('"', propPos)) != std::string::npos) {
                size_t propNameEnd = objStr.find('"', propPos + 1);
                if (propNameEnd == std::string::npos) break;
                
                std::string propName = objStr.substr(propPos + 1, propNameEnd - propPos - 1);
                
                // Ищем значение
                size_t colonPos2 = objStr.find(':', propNameEnd);
                if (colonPos2 == std::string::npos) break;
                
                size_t valueStart = colonPos2 + 1;
                while (valueStart < objStr.length() && std::isspace(objStr[valueStart])) {
                    valueStart++;
                }
                
                size_t valueEnd = valueStart;
                if (objStr[valueStart] == '"') {
                    // Строковое значение
                    valueEnd = objStr.find('"', valueStart + 1);
                    if (valueEnd == std::string::npos) break;
                    valueEnd++;
                } else if (objStr[valueStart] == '[') {
                    // Массив
                    int bracketCount = 1;
                    valueEnd = valueStart + 1;
                    while (valueEnd < objStr.length() && bracketCount > 0) {
                        if (objStr[valueEnd] == '[') bracketCount++;
                        else if (objStr[valueEnd] == ']') bracketCount--;
                        valueEnd++;
                    }
                } else {
                    // Число или другое значение
                    while (valueEnd < objStr.length() && 
                           objStr[valueEnd] != ',' && 
                           objStr[valueEnd] != '}') {
                        valueEnd++;
                    }
                }
                
                std::string propValue = trim(objStr.substr(valueStart, valueEnd - valueStart));
                props[propName] = propValue;
                
                propPos = valueEnd;
            }
            
            // Создаем объект
            std::unique_ptr<VisualObject> obj;
            
            if (type == "Rectangle") {
                obj = createRectangle(props, db, font);
            }
            else if (type == "Text") {
                obj = createText(props, db, font);
            }
            else if (type == "Line") {
                obj = createLine(props, db, font);
            }
            else if (type == "InputField") {
                obj = createInputField(props, db, font);
            }
            else if (type == "Button") {
                obj = createButton(props, db, font);
            }
            else if (type == "HistoryGraph") {
                obj = createHistoryGraph(props, db, font);
            }
            else if (type == "Image") {
                obj = createImage(props, db, font);
            }
            
            if (obj) {
                objects.push_back(std::move(obj));
            }
            
            pos = objEnd;
        }
        
        Logger::info("Loaded " + std::to_string(objects.size()) + " objects from JSON");
        
    } catch (const std::exception& e) {
        Logger::error("Error loading JSON: " + std::string(e.what()));
    }
    

    if (objects.empty()) {
        Logger::warning("JSON file objects.json has not objects or has wrong type: " + filename);
    }
    return objects;
}

// Функции создания объектов
std::unique_ptr<VisualObject> JSONSceneLoader::createRectangle(
    const std::unordered_map<std::string, std::string>& props,
    VariableDatabase* db,
    sf::Font* font) {
    
    try {
        float x = std::stof(props.at("x"));
        float y = std::stof(props.at("y"));
        float width = std::stof(props.at("width"));
        float height = std::stof(props.at("height"));
        std::string name = unquote(props.at("name"));
        
        sf::Color color = parseColor(props.at("color"));
        std::string variable = props.count("variable") > 0 ? unquote(props.at("variable")) : "";
        
        auto rect = std::make_unique<Rectangle>(x, y, width, height, color, name, db, variable);
        
        // Обработка условий для прямоугольника Status Panel
        if (name == "Status Panel") {
            // Добавляем условия изменения цвета
            rect->addCondition(0, sf::Color(124, 36, 179));
            rect->addCondition(1, sf::Color(199, 24, 88));
            rect->addCondition(2, sf::Color(72, 146, 163));
            rect->addCondition(3, sf::Color(176, 12, 160));
            rect->addCondition(5, sf::Color(176, 80, 12));
            rect->addCondition(6, sf::Color(22, 219, 219));
            rect->addCondition(7, sf::Color::Magenta);
            rect->addCondition(8, sf::Color::Black);
            rect->addCondition(9, sf::Color::Red);
        }
        
        return rect;
    } catch (...) {
        return nullptr;
    }
}

std::unique_ptr<VisualObject> JSONSceneLoader::createText(
    const std::unordered_map<std::string, std::string>& props,
    VariableDatabase* db,
    sf::Font* font) {
    
    try {
        float x = std::stof(props.at("x"));
        float y = std::stof(props.at("y"));
        std::string content = unquote(props.at("content"));
        unsigned int fontSize = std::stoi(props.at("fontSize"));
        std::string name = unquote(props.at("name"));
        
        sf::Color color = parseColor(props.at("color"));
        std::string variable = props.count("variable") > 0 ? unquote(props.at("variable")) : "";
        std::string format = props.count("format") > 0 ? unquote(props.at("format")) : "";
        
        return std::make_unique<Text>(x, y, content, font, fontSize, color, name, db, variable, format);
    } catch (...) {
        return nullptr;
    }
}

std::unique_ptr<VisualObject> JSONSceneLoader::createLine(
    const std::unordered_map<std::string, std::string>& props,
    VariableDatabase* db,
    sf::Font* font) {
    
    try {
        float x = std::stof(props.at("x"));
        float y = std::stof(props.at("y"));
        float x2 = std::stof(props.at("x2"));
        float y2 = std::stof(props.at("y2"));
        std::string name = unquote(props.at("name"));
        
        sf::Color color = parseColor(props.at("color"));
        
        return std::make_unique<Line>(x, y, x2, y2, color, name, db);
    } catch (...) {
        return nullptr;
    }
}

std::unique_ptr<VisualObject> JSONSceneLoader::createInputField(
    const std::unordered_map<std::string, std::string>& props,
    VariableDatabase* db,
    sf::Font* font) {
    
    try {
        float x = std::stof(props.at("x"));
        float y = std::stof(props.at("y"));
        float width = std::stof(props.at("width"));
        float height = std::stof(props.at("height"));
        unsigned int fontSize = std::stoi(props.at("fontSize"));
        std::string name = unquote(props.at("name"));
        
        std::string variable = props.count("variable") > 0 ? unquote(props.at("variable")) : "";
        
        return std::make_unique<InputField>(x, y, width, height, font, fontSize, name, db, variable);
    } catch (...) {
        return nullptr;
    }
}

std::unique_ptr<VisualObject> JSONSceneLoader::createButton(
    const std::unordered_map<std::string, std::string>& props,
    VariableDatabase* db,
    sf::Font* font) {
    
    try {
        float x = std::stof(props.at("x"));
        float y = std::stof(props.at("y"));
        float width = std::stof(props.at("width"));
        float height = std::stof(props.at("height"));
        std::string text = unquote(props.at("text"));
        unsigned int fontSize = std::stoi(props.at("fontSize"));
        std::string name = unquote(props.at("name"));
        
        sf::Color color = parseColor(props.at("color"));
        sf::Color textColor = props.count("textColor") > 0 ? 
            parseColor(props.at("textColor")) : sf::Color::Black;
        
        std::string action = props.count("action") > 0 ? unquote(props.at("action")) : "";
        std::string variable = props.count("variable") > 0 ? unquote(props.at("variable")) : "";
        
        std::function<void()> onClick = nullptr;
        
        if (action == "apply") {
            onClick = []() { 
                Logger::info("Apply button clicked");
            };
        }
        else if (action == "change_color") {
            onClick = [db]() {
                static int status = 0;
                status = (status + 1) % 10;
                db->setVariable("panel_status", static_cast<double>(status));
                Logger::info("Change Color button clicked. Panel status toggled to: " + std::to_string(status));
            };
        }
        else if (action == "increase_temp") {
            onClick = [db]() {
                double current = db->getVariable("temperature_value");
                db->setVariable("temperature_value", current + 1.0);
                Logger::info("Temperature increased to: " + std::to_string(current + 1.0));
            };
        }
        else if (action == "decrease_temp") {
            onClick = [db]() {
                double current = db->getVariable("temperature_value");
                db->setVariable("temperature_value", current - 1.0);
                Logger::info("Temperature decreased to: " + std::to_string(current - 1.0));
            };
        }
        else if (action == "increase_pressure") {
            onClick = [db]() {
                double current = db->getVariable("pressure_value");
                db->setVariable("pressure_value", current + 0.5);
                Logger::info("Pressure increased to: " + std::to_string(current + 0.5));
            };
        }
        else if (action == "decrease_pressure") {
            onClick = [db]() {
                double current = db->getVariable("pressure_value");
                db->setVariable("pressure_value", current - 0.5);
                Logger::info("Pressure decreased to: " + std::to_string(current - 0.5));
            };
        }
        
        auto button = std::make_unique<Button>(x, y, width, height, text, font, fontSize, 
                                              color, name, db, variable, onClick, textColor);
        
        // Если это кнопка Change Color, убедимся, что она работает
        if (name == "Change Color") {
            Logger::info("Created Change Color button with action: " + action);
        }
        
        return button;
    } catch (const std::exception& e) {
        Logger::error("Error creating Button: " + std::string(e.what()));
        return nullptr;
    }
}

std::unique_ptr<VisualObject> JSONSceneLoader::createHistoryGraph(
    const std::unordered_map<std::string, std::string>& props,
    VariableDatabase* db,
    sf::Font* font) {
    
    try {
        float x = std::stof(props.at("x"));
        float y = std::stof(props.at("y"));
        float width = std::stof(props.at("width"));
        float height = std::stof(props.at("height"));
        std::string name = unquote(props.at("name"));
        
        std::string variable = props.count("variable") > 0 ? unquote(props.at("variable")) : "";
        size_t maxHistory = props.count("maxHistory") > 0 ? std::stoul(props.at("maxHistory")) : 50;
        sf::Color lineColor = props.count("lineColor") > 0 ? parseColor(props.at("lineColor")) : sf::Color::Blue;
        sf::Color gridColor = props.count("gridColor") > 0 ? parseColor(props.at("gridColor")) : sf::Color(200, 200, 200, 100);
        
        return std::make_unique<HistoryGraph>(x, y, width, height, name, db, variable, maxHistory, lineColor, gridColor);
    } catch (...) {
        return nullptr;
    }
}

std::unique_ptr<VisualObject> JSONSceneLoader::createImage(
    const std::unordered_map<std::string, std::string>& props,
    VariableDatabase* db,
    sf::Font* font) {
    
    try {
        float x = std::stof(props.at("x"));
        float y = std::stof(props.at("y"));
        float width = std::stof(props.at("width"));
        float height = std::stof(props.at("height"));
        std::string path = unquote(props.at("path"));
        std::string name = unquote(props.at("name"));
        
        return std::make_unique<Image>(x, y, width, height, path, name, db);
    } catch (...) {
        return nullptr;
    }

}
