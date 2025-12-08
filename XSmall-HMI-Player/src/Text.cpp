#include "Text.h"
#include "logger.h"
#include <sstream>
#include <iomanip>

// Конструктор текста
Text::Text(float x, float y, const std::string& content, 
           sf::Font* font, unsigned int size, const sf::Color& color,
           const std::string& name, VariableDatabase* db, 
           const std::string& varName, const std::string& format)
    : VisualObject(x, y, name, db),  // Вызываем конструктор родителя
      formatString(format),          // Сохраняем формат
      variableName(varName),         // Сохраняем имя переменной
      font(font) {                   // Сохраняем шрифт
    
    // Настраиваем SFML текст
    text.setPosition(x, y);      // Позиция
    text.setFont(*font);         // Шрифт
    text.setCharacterSize(size); // Размер шрифта
    text.setFillColor(color);    // Цвет
    
    if (variableName.empty()) {
        // Статический текст - просто устанавливаем строку
        text.setString(content);
    } else {
        // Динамический текст - связан с переменной
        if (format.empty()) {
            // Формат не указан - просто используем переданный текст
            text.setString(content);
        } else {
            // Формат указан - сохраняем его для использования в update()
            formatString = format;
        }
        
        // Подписываемся на изменения переменной
        if (database) {
            database->subscribe(variableName, [this](double value) {
                this->update();  // При изменении переменной обновляем текст
            });
        }
        update();  // Первоначальное обновление текста
    }
}

// Рисуем текст
void Text::draw(sf::RenderWindow& window) {
    window.draw(text);
}

// Обновляем текст (для динамического текста)
void Text::update() {
    if (!variableName.empty() && database) {
        // Получаем текущее значение переменной
        double value = database->getVariable(variableName);
        
        if (!formatString.empty()) {
            // Есть формат - форматируем строку
            std::stringstream ss;  // Поток для сборки строки
            
            // Ищем в формате место для вставки значения
            size_t pos = formatString.find("%f");
            if (pos != std::string::npos) {
                // Нашли %f - заменяем его на значение с одним знаком после запятой
                std::string before = formatString.substr(0, pos);  // Текст до %f
                std::string after = formatString.substr(pos + 2);  // Текст после %f
                
                ss << before << std::fixed << std::setprecision(1) << value << after;
                text.setString(ss.str());
            } else {
                // %f не найден - просто добавляем значение в конец
                text.setString(formatString + std::to_string(value));
            }
        } else {
            // Без форматирования - просто преобразуем число в строку
            text.setString(std::to_string(value));
        }
    }
}

// Устанавливает текст вручную (только для статического текста)
void Text::setString(const std::string& str) {
    text.setString(str);
}