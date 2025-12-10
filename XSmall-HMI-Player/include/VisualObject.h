#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <VariableDatabase.h>

// Предварительное объявление, чтобы не включать весь заголовочный файл
class VariableDatabase;

// Графические объекты в программе
// Каждый объект на экране (кнопка, текст, прямоугольник) будет наследником этого класса

class VisualObject {
protected:
    // Координаты объекта на экране
    float x, y;                  
    
    // Имя объекта, для нахождения конкретного объекта среди многих
    std::string name;            
    
    // Указатель на базу данных переменных, для того, чтобы объект считаыва и менял значения
    VariableDatabase* database;  

public:
    // Конструктор - создает объект с указанными параметрами
    VisualObject(float x, float y, const std::string& name, VariableDatabase* db);
    
    // Виртуальный деструктор - нужен для правильного удаления наследников
    virtual ~VisualObject() = default;
    
    // Виртуальные методы - их реализуют все классы-наследники
    // Рисует объект в окне
    virtual void draw(sf::RenderWindow& window) = 0;
    
    // Обновляет состояние объекта (например, меняет цвет в зависимости от переменной)
    virtual void update() = 0;

    // Обрабатывает события (клики мыши, нажатия клавиш)
    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) {};
    
    // Вспомогательные методы
    
    // Перемещает объект в новые координаты
    void setPosition(float newX, float newY);
    
    // Возвращает имя объекта
    std::string getName() const;
};


#endif
