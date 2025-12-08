#include "VariableDatabase.h"
#include "logger.h"
#include <iostream>

// Конструктор - создает базу и заполняет её тестовыми данными
VariableDatabase::VariableDatabase() {
    initializeDemoVariables();  // Сразу создаем демо-переменные
}

// Установка значения переменной
void VariableDatabase::setVariable(const std::string& name, double value) {
    // Запоминаем старое значение (на случай, если нужно будет откатиться)
    double oldValue = variables[name];
    
    // Обновляем текущее значение
    variables[name] = value;
    
    // Добавляем в историю изменений (чтобы строить графики)
    addToHistory(name, value);
    
    // Проверяем, есть ли подписчики на эту переменную
    if (subscribers.find(name) != subscribers.end()) {
        // Если есть - вызываем все функции-подписчики
        for (auto& callback : subscribers[name]) {
            callback(value);  // Передаем новое значение в каждую функцию
        }
    }
    
    // Пишем в лог для отладки
    Logger::info("Variable '" + name + "' set to: " + std::to_string(value));
}

// Получение значения переменной
double VariableDatabase::getVariable(const std::string& name) const {
    // Ищем переменную в нашем хранилище
    auto it = variables.find(name);
    if (it != variables.end()) {
        return it->second;  // Нашли - возвращаем значение
    }
    return 0.0;  // Не нашли - возвращаем 0 (можно было бы кинуть ошибку)
}

// Проверка существования переменной
bool VariableDatabase::variableExists(const std::string& name) const {
    // Просто ищем в мапе - если нашли, значит существует
    return variables.find(name) != variables.end();
}

// Добавление значения в историю
void VariableDatabase::addToHistory(const std::string& name, double value) {
    // Добавляем значение в конец массива истории
    historyVariables[name].push_back(value);
    
    // Чтобы история не росла бесконечно, ограничиваем её 100 значениями
    if (historyVariables[name].size() > 100) {
        // Удаляем самое старое значение (первое в массиве)
        historyVariables[name].erase(historyVariables[name].begin());
    }
}

// Получение истории переменной
const std::vector<double>& VariableDatabase::getHistory(const std::string& name) const {
    // Статический пустой вектор - на случай, если истории нет
    static std::vector<double> emptyHistory;
    
    // Ищем историю для указанной переменной
    auto it = historyVariables.find(name);
    if (it != historyVariables.end()) {
        return it->second;  // Нашли - возвращаем историю
    }
    return emptyHistory;  // Не нашли - возвращаем пустой вектор
}

// Подписка на изменения переменной
void VariableDatabase::subscribe(const std::string& variable, std::function<void(double)> callback) {
    // Добавляем функцию в список подписчиков для указанной переменной
    subscribers[variable].push_back(callback);
}

// Инициализация демо-переменных
void VariableDatabase::initializeDemoVariables() {
    // Создаем несколько переменных для тестирования
    setVariable("panel_status", 0.0);       // Статус панели
    setVariable("temperature_value", 72.5); // Температура
    setVariable("setpoint_value", 65.0);    // Уставка (желаемая температура)
    setVariable("pressure_value", 1.2);     // Давление
    
    // Создаем тестовую историю температуры для графика
    for (int i = 0; i < 10; ++i) {
        setVariable("temperature_history", 70.0 + i * 0.5);
    }
}