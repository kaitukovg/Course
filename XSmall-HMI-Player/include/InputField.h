#ifndef INPUTFIELD_H
#define INPUTFIELD_H

#include "VisualObject.h"
#include "VariableDatabase.h"
#include <SFML/Graphics.hpp>

// Поле ввода текста - пользователь может вводить числа
// Значение сохраняется в переменную базы данных

class InputField : public VisualObject {
private:
    // Фон поля ввода
    sf::RectangleShape background;
    
    // Текст, который вводит пользователь
    sf::Text text;
    
    // Указатель на шрифт
    sf::Font* font;
    
    // Имя переменной, в которую сохраняется значение
    std::string variableName;
    
    // Активно ли поле ввода (мигает курсор)
    bool isActive;
    
    // Текст, который ввел пользователь
    std::string inputText;

public:
    // Конструктор
    InputField(float x, float y, float width, float height,
               sf::Font* font, unsigned int fontSize,
               const std::string& name, VariableDatabase* db,
               const std::string& varName = "");
    
    // Методы родителя
    void draw(sf::RenderWindow& window) override;
    void update() override;
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    
    // Активирует/деактивирует поле ввода
    void setActive(bool active);
    
    // Проверяет, попадает ли точка в поле ввода
    bool contains(float pointX, float pointY) const;
};

#endif