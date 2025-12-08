#ifndef IMAGE_H
#define IMAGE_H

#include "VisualObject.h"
#include <SFML/Graphics.hpp>

// Класс для отображения изображений

class Image : public VisualObject {
private:
    // Текстура (само изображение)
    sf::Texture texture;
    
    // Спрайт для отображения текстуры
    sf::Sprite sprite;
    
    // Размеры изображения
    float imgWidth, imgHeight;
    
    // Путь к файлу изображения
    std::string imagePath;
    
    // Удалось ли загрузить текстуру
    bool textureLoaded;

public:
    // Конструктор
    Image(float x, float y, float width, float height,
          const std::string& path, const std::string& name,
          VariableDatabase* db);
    
    // Методы родителя
    void draw(sf::RenderWindow& window) override;
    void update() override {};  // Изображение не обновляется
    
    // Загружает текстуру из файла
    bool loadTexture(const std::string& path);
};

#endif