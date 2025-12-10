#ifndef VARIABLEDATABASE_H
#define VARIABLEDATABASE_H

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <memory>

// Содкржит все значения переменных в системе
// Все объекты на экране могут читать эти значения и реагировать на их изменения

class VariableDatabase {
private:
    // Основное хранилище: имя переменной -> её значение
    // Например, "temperature" -> 72.5
    std::unordered_map<std::string, double> variables;
    
    // История изменений для каждой переменной
    // Нужно для графиков - чтобы показывать, как менялась температура за время
    std::unordered_map<std::string, std::vector<double>> historyVariables;
    
    // Подписчики на изменения переменных
    // Когда переменная меняется, мы вызываем все функции, которые на неё подписаны
    // Например, текст температуры подписан на переменную "temperature"
    // Когда температура меняется, текст автоматически обновляется
    std::unordered_map<std::string, std::vector<std::function<void(double)>>> subscribers;

public:
    // Конструктор - создает базу данных
    VariableDatabase();
    
    // Устанавливает значение переменной и уведомляет всех подписчиков
    void setVariable(const std::string& name, double value);
    
    // Возвращает значение переменной
    double getVariable(const std::string& name) const;
    
    // Проверяет, есть ли такая переменная в базе
    bool variableExists(const std::string& name) const;
    
    // Добавляет значение в историю (для графиков)
    void addToHistory(const std::string& name, double value);
    
    // Возвращает историю изменений переменной
    const std::vector<double>& getHistory(const std::string& name) const;
    
    // Подписывает функцию на изменения переменной
    // Когда переменная изменится, эта функция будет вызвана
    void subscribe(const std::string& variable, std::function<void(double)> callback);
    
    // Создает тестовые переменные для демо-режима
    void initializeDemoVariables();
};


#endif
