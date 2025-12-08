#include "Button.h"
#include "logger.h"

// Конструктор кнопки - инициализирует все поля
Button::Button(float x, float y, float width, float height,
               const std::string& buttonText, sf::Font* font, unsigned int fontSize,
               const sf::Color& color, const std::string& name, VariableDatabase* db,
               const std::string& varName, std::function<void()> onClickFunc,
               const sf::Color& textClr)  
    : VisualObject(x, y, name, db),  // Вызываем конструктор родителя
      font(font),                    // Сохраняем шрифт
      variableName(varName),         // Сохраняем имя переменной (может быть пустым)
      normalColor(color),            // Основной цвет
      // Автоматически создаем цвета для hover и pressed - делаем основной цвет темнее
      hoverColor(sf::Color(std::max(0, color.r - 30), 
                           std::max(0, color.g - 30), 
                           std::max(0, color.b - 30), 
                           color.a)),
      pressedColor(sf::Color(std::max(0, color.r - 50), 
                             std::max(0, color.g - 50), 
                             std::max(0, color.b - 50), 
                             color.a)),
      textColor(textClr),            // Цвет текста
      isHovered(false),              // Изначально мышь не над кнопкой
      isPressed(false),              // Изначально не нажата
      onClick(onClickFunc) {         // Функция при клике (может быть nullptr)
    
    // Настраиваем прямоугольник кнопки
    shape.setPosition(x, y);                         // Позиция
    shape.setSize(sf::Vector2f(width, height));      // Размер
    shape.setFillColor(normalColor);                 // Цвет заливки
    shape.setOutlineColor(sf::Color::Black);         // Цвет контура
    shape.setOutlineThickness(1);                    // Толщина контура
    
    // Настраиваем текст кнопки
    // Сначала ставим в центр кнопки, потом сдвинем
    text.setPosition(x + width / 2, y + height / 2);
    text.setFont(*font);                             // Устанавливаем шрифт
    text.setString(buttonText);                      // Текст кнопки
    text.setCharacterSize(fontSize);                 // Размер шрифта
    text.setFillColor(textColor);                    // Цвет текста
    
    // Центрируем текст относительно его центра
    sf::FloatRect textBounds = text.getLocalBounds();  // Получаем размеры текста
    text.setOrigin(textBounds.left + textBounds.width / 2, 
                   textBounds.top + textBounds.height / 2);
    
    // Если кнопка привязана к переменной, подписываемся на её изменения
    if (!variableName.empty() && database) {
        database->subscribe(variableName, [this](double value) {
            this->update();  // При изменении переменной обновляем кнопку
        });
    }
}

// Рисуем кнопку в окне
void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);  // Сначала рисуем прямоугольник
    window.draw(text);   // Потом текст поверх него
}

// Обновляем состояние кнопки (цвет в зависимости от состояния)
void Button::update() {
    if (!variableName.empty() && database) {
        // Кнопка привязана к переменной - цвет зависит от значения переменной
        double value = database->getVariable(variableName);
        if (value == 0) {
            shape.setFillColor(normalColor);    // Значение 0 - обычный цвет
        } else {
            shape.setFillColor(pressedColor);   // Другие значения - цвет как при нажатии
        }
    } else {
        // Обычная кнопка - цвет зависит от состояния мыши
        if (isPressed) {
            shape.setFillColor(pressedColor);   // Нажата - самый темный цвет
        } else if (isHovered) {
            shape.setFillColor(hoverColor);     // Мышь над кнопкой - средний цвет
        } else {
            shape.setFillColor(normalColor);    // Обычное состояние - обычный цвет
        }
    }
}

// Обрабатываем события (движение мыши, клики)
void Button::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Получаем текущую позицию мыши в окне
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    
    // Проверяем, находится ли мышь над кнопкой
    bool mouseOver = contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    
    if (event.type == sf::Event::MouseMoved) {
        // Мышь движется - обновляем состояние hover
        isHovered = mouseOver;
        update();  // Обновляем цвет
    }
    
    if (event.type == sf::Event::MouseButtonPressed) {
        // Кнопка мыши нажата
        if (event.mouseButton.button == sf::Mouse::Left && mouseOver) {
            isPressed = true;   // Если нажали на кнопку - запоминаем
            update();           // Обновляем цвет
        }
    }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        // Кнопка мыши отпущена
        if (event.mouseButton.button == sf::Mouse::Left && isPressed && mouseOver) {
            // Если отпустили кнопку мыши, и до этого она была нажата на этой кнопке
            if (onClick) {
                onClick();  // Вызываем функцию-обработчик
            }
            isPressed = false;  // Сбрасываем флаг нажатия
            update();           // Обновляем цвет
        }
    }
}

// Проверяет, находится ли точка (например, позиция мыши) внутри кнопки
bool Button::contains(float pointX, float pointY) const {
    // Точка внутри, если её X между левой и правой границей кнопки
    // И Y между верхней и нижней границей
    return pointX >= x && pointX <= x + shape.getSize().x &&
           pointY >= y && pointY <= y + shape.getSize().y;
}

// Устанавливает функцию, которая вызывается при клике на кнопку
void Button::setOnClick(std::function<void()> onClickFunc) {
    onClick = onClickFunc;
}

// Меняет цвет текста на кнопке
void Button::setTextColor(const sf::Color& color) {
    textColor = color;           // Запоминаем новый цвет
    text.setFillColor(textColor); // Применяем к тексту
}