#ifndef TEXT_H
#define TEXT_H

#include "VisualObject.h"
#include "VariableDatabase.h"
#include <SFML/Graphics.hpp>

// Класс текста - может быть статическим (просто текст) или динамическим
// Динамический текст отображает значение переменной и обновляется при её изменении

class Text : public VisualObject {
private:
    // SFML объект для отображения текста
    sf::Text text;
    
    // Формат строки (например, "Температура: %f °C")
    // %f будет заменено на значение переменной
    std::string formatString;
    
    // Имя переменной, значение которой отображаем (пустое для статического текста)
    std::string variableName;
    
    // Указатель на шрифт
    sf::Font* font;

public:
    // Конструктор
    Text(float x, float y, const std::string& content, 
         sf::Font* font, unsigned int size, const sf::Color& color,
         const std::string& name, VariableDatabase* db, 
         const std::string& varName = "", const std::string& format = "");
    
    // Методы родителя
    void draw(sf::RenderWindow& window) override;
    void update() override;
    
    // Устанавливает текст вручную (для статического текста)
    void setString(const std::string& str);
};

#endif