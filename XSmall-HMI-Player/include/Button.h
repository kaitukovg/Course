#ifndef BUTTON_H
#define BUTTON_H

#include "VisualObject.h"
#include "VariableDatabase.h"
#include <SFML/Graphics.hpp>
#include <functional>

// Класс кнопки - умеет показывать текст, реагировать на нажатия
// Может быть привязана к переменной (тогда её состояние зависит от значения переменной)
// Или может выполнять действие при нажатии

class Button : public VisualObject {
private:
    // Графическая форма кнопки (прямоугольник)
    sf::RectangleShape shape;
    
    // Текст на кнопке
    sf::Text text;
    
    // Указатель на шрифт (не владеем им, шрифт хранится в основном классе)
    sf::Font* font;
    
    // Имя переменной, к которой привязана кнопка (если привязана)
    std::string variableName;
    
    // Цвета для разных состояний кнопки
    sf::Color normalColor;    // Обычное состояние
    sf::Color hoverColor;     // Когда мышь над кнопкой
    sf::Color pressedColor;   // Когда кнопка нажата
    sf::Color textColor;      // Цвет текста
    
    // Флаги состояний
    bool isHovered;   // Мышь над кнопкой?
    bool isPressed;   // Кнопка нажата?
    
    // Функция, которая вызывается при клике на кнопку
    std::function<void()> onClick;

public:
    // Конструктор параметрами
    Button(float x, float y, float width, float height,
           const std::string& buttonText, sf::Font* font, unsigned int fontSize,
           const sf::Color& color, const std::string& name, VariableDatabase* db,
           const std::string& varName = "", std::function<void()> onClickFunc = nullptr,
           const sf::Color& textClr = sf::Color::Black);  
    
    // Методы, унаследованные от VisualObject
    void draw(sf::RenderWindow& window) override;
    void update() override;
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    
    // Свои методы
    bool contains(float pointX, float pointY) const;  // Проверяет, попадает ли точка в кнопку
    void setOnClick(std::function<void()> onClickFunc);  // Устанавливает обработчик клика
    void setTextColor(const sf::Color& color);  // Меняет цвет текста
};


#endif
