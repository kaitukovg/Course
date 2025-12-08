#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <VariableDatabase.h>

// Предварительное объявление, чтобы не включать весь заголовочный файл
class VariableDatabase;

// Это как чертеж для всех графических объектов в программе
// Каждый объект на экране (кнопка, текст, прямоугольник) будет наследником этого класса
// Это удобно - у всех объектов будет общий набор методов и свойств

class VisualObject {
protected:
    // Координаты объекта на экране - где он находится
    float x, y;                  
    
    // Имя объекта - чтобы мы могли найти конкретный объект среди многих
    std::string name;            
    
    // Указатель на базу данных переменных - чтобы объект мог читать и менять значения
    VariableDatabase* database;  

public:
    // Конструктор - создает объект с указанными параметрами
    VisualObject(float x, float y, const std::string& name, VariableDatabase* db);
    
    // Виртуальный деструктор - нужен для правильного удаления наследников
    virtual ~VisualObject() = default;
    
    // Чисто виртуальные методы - их ОБЯЗАНЫ реализовать все классы-наследники
    // Рисует объект в окне
    virtual void draw(sf::RenderWindow& window) = 0;
    
    // Обновляет состояние объекта (например, меняет цвет в зависимости от переменной)
    virtual void update() = 0;

    // Обрабатывает события (клики мыши, нажатия клавиш)
    // По умолчанию ничего не делает, но наследники могут переопределить
    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) {};
    
    // Вспомогательные методы
    
    // Перемещает объект в новые координаты
    void setPosition(float newX, float newY);
    
    // Возвращает имя объекта
    std::string getName() const;
};

#endif