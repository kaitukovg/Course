#include "Image.h"
#include "logger.h"
#include <iostream>

// Конструктор изображения
Image::Image(float x, float y, float width, float height,
             const std::string& path, const std::string& name,
             VariableDatabase* db)
    : VisualObject(x, y, name, db),  // Конструктор родителя
      imgWidth(width), imgHeight(height),  // Сохраняем размеры
      imagePath(path),                     // Путь к файлу
      textureLoaded(false) {               // Пока не загружено
    
    std::cout << "Try to load image from: " << path << std::endl;
    
    // Пытаемся загрузить текстуру
    textureLoaded = loadTexture(path);
    
    // Настраиваем спрайт
    sprite.setPosition(x, y);
    
    if (textureLoaded) {
        // Успешно загрузили - масштабируем под нужный размер
        float scaleX = width / sprite.getLocalBounds().width;
        float scaleY = height / sprite.getLocalBounds().height;
        sprite.setScale(scaleX, scaleY);
        
        std::cout << "IMAGE IS LOADED FROM: " << path << std::endl;
    } else {
        std::cout << "FAILED TO LOAD THE IMAGE: " << path << std::endl;
    }
}

// Рисуем изображение
void Image::draw(sf::RenderWindow& window) {
    if (textureLoaded) {
        // Если текстура загружена - рисуем её
        window.draw(sprite);
    } else {
        // Иначе рисуем заглушку (серый прямоугольник)
        sf::RectangleShape placeholder(sf::Vector2f(imgWidth, imgHeight));
        placeholder.setPosition(x, y);
        placeholder.setFillColor(sf::Color(200, 200, 200));  // Серый цвет
        placeholder.setOutlineColor(sf::Color::Black);       // Черная рамка
        placeholder.setOutlineThickness(2);
        window.draw(placeholder);
        
        // И текст "Image not found"
        sf::Font font;
        // Пробуем загрузить шрифт для сообщения об ошибке
        if (font.loadFromFile("../assets/fonts/arial.ttf")) {
            sf::Text errorText;
            errorText.setFont(font);
            errorText.setString("Image not found");
            errorText.setCharacterSize(16);
            errorText.setFillColor(sf::Color::Black);
            errorText.setPosition(x + 10, y + imgHeight / 2 - 10);
            window.draw(errorText);
        }
    }
}

// Пытается загрузить текстуру из файла
bool Image::loadTexture(const std::string& path) {
    // Пробуем несколько возможных путей (на случай, если файл в другой папке)
    std::vector<std::string> possiblePaths = {
        path,                       // Оригинальный путь
        "../" + path,               // На уровень выше
        "../../" + path,            // На два уровня выше
        "assets/images/logo.png",   // Просто от корня
        "../assets/images/logo.png" // От build папки
    };
    
    // Пробуем каждый путь
    for (const auto& testPath : possiblePaths) {
        std::cout << "Try to load from: " << testPath << std::endl;
        if (texture.loadFromFile(testPath)) {
            // Успешно - устанавливаем текстуру в спрайт
            sprite.setTexture(texture);
            std::cout << "SUCCESSFULL LOAD FROM: " << testPath << std::endl;
            return true;
        }
    }
    
    // Ни один путь не сработал
    Logger::warning("FAILED TO LOAD IMAGE FROM ANY PATH: " + path);
    return false;
}
