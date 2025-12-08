#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

// Простейший логгер - выводит сообщения в консоль с разными уровнями важности

class Logger {
public:
    // Информационное сообщение (обычные события)
    static void info(const std::string& message) {
        std::cout << "[INFO] " << message << std::endl;
    }
    
    // Сообщение об ошибке (критические проблемы)
    static void error(const std::string& message) {
        std::cerr << "[ERROR] " << message << std::endl;
    }
    
    // Предупреждение (некритические проблемы)
    static void warning(const std::string& message) {
        std::cout << "[WARNING] " << message << std::endl;
    }
};

#endif