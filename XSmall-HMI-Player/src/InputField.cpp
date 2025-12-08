#include "InputField.h"
#include "logger.h"

// Конструктор поля ввода
InputField::InputField(float x, float y, float width, float height,
                       sf::Font* font, unsigned int fontSize,
                       const std::string& name, VariableDatabase* db,
                       const std::string& varName)
    : VisualObject(x, y, name, db),  // Конструктор родителя
      font(font),                    // Шрифт
      variableName(varName),         // Имя переменной
      isActive(false),               // По умолчанию не активно
      inputText("") {                // Пустой текст
    
    // Настраиваем фон
    background.setPosition(x, y);                         // Позиция
    background.setSize(sf::Vector2f(width, height));      // Размер
    background.setFillColor(sf::Color::White);            // Белый фон
    background.setOutlineColor(sf::Color::Black);         // Черная рамка
    background.setOutlineThickness(1);                    // Толщина рамки
    
    // Настраиваем текст
    text.setPosition(x + 5, y + 5);  // Небольшой отступ от краев
    text.setFont(*font);             // Шрифт
    text.setCharacterSize(fontSize); // Размер шрифта
    text.setFillColor(sf::Color::Black);  // Черный текст
    text.setString(inputText);       // Пустая строка
    
    // Подписываемся на изменения переменной
    if (!variableName.empty() && database) {
        database->subscribe(variableName, [this](double value) {
            this->update();  // При изменении переменной обновляем текст
        });
    }
}

// Рисуем поле ввода
void InputField::draw(sf::RenderWindow& window) {
    window.draw(background);  // Сначала фон
    window.draw(text);        // Потом текст
}

// Обновляем поле ввода
void InputField::update() {
    // Обновляем текст из переменной только если поле не активно
    if (!variableName.empty() && database && !isActive) {
        double value = database->getVariable(variableName);
        inputText = std::to_string(value);  // Преобразуем число в строку
        text.setString(inputText);          // Устанавливаем текст
    }
}

// Обрабатываем события
void InputField::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        // Клик мыши
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Получаем позицию мыши
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            
            // Активируем поле, если кликнули по нему
            // Деактивируем, если кликнули мимо
            setActive(contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
        }
    }
    
    // Обработка ввода текста только для активного поля
    if (isActive && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b') { 
            // Backspace - удаляем последний символ
            if (!inputText.empty()) {
                inputText.pop_back();
            }
        } else if (event.text.unicode == '\r') {  
            // Enter - завершаем ввод
            setActive(false);
            
            // Сохраняем значение в переменную
            if (!inputText.empty() && !variableName.empty() && database) {
                try {
                    double value = std::stod(inputText);  // Пробуем преобразовать строку в число
                    database->setVariable(variableName, value);
                    Logger::info("Input field set variable '" + variableName + "' to: " + inputText);
                } catch (const std::exception& e) {
                    // Не удалось преобразовать (например, ввели буквы)
                    Logger::error("Invalid input: " + inputText);
                }
            }
        } else if (event.text.unicode < 128) { 
            // Только ASCII символы (буквы, цифры, знаки)
            inputText += static_cast<char>(event.text.unicode);
        }

        // Показываем курсор (|) в активном поле
        text.setString(inputText + (isActive ? "|" : ""));
    }
}

// Активирует/деактивирует поле ввода
void InputField::setActive(bool active) {
    isActive = active;
    
    if (isActive) {
        // Подсвечиваем активное поле
        background.setOutlineColor(sf::Color::Blue);  // Синяя рамка
        background.setOutlineThickness(2);            // Толще рамка
        text.setString(inputText + "|");              // Показываем курсор
    } else {
        // Возвращаем обычный вид
        background.setOutlineColor(sf::Color::Black);  // Черная рамка
        background.setOutlineThickness(1);             // Обычная толщина
        text.setString(inputText);                     // Без курсора
        
        // Сохраняем значение при деактивации
        if (!inputText.empty() && !variableName.empty() && database) {
            try {
                double value = std::stod(inputText);  // Преобразуем в число
                database->setVariable(variableName, value);
            } catch (const std::exception& e) {
                Logger::error("Invalid input in input field: " + inputText);
            }
        }
    }
}

// Проверяет, попадает ли точка в поле ввода
bool InputField::contains(float pointX, float pointY) const {
    // Проверяем, находится ли точка внутри прямоугольника фона
    return pointX >= x && pointX <= x + background.getSize().x &&
           pointY >= y && pointY <= y + background.getSize().y;

}
