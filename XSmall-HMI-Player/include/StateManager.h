#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <logger.h>
#include "VariableDatabase.h"
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>

/**
 * Сохраняет и загружает состояние системы в формате JSON.
 * Автоматически вызывается из HmiPlayer при запуске и закрытии.
 */
class StateManager {
private:
    std::string filename;

public:
    StateManager(const std::string& file = "../saved_state.json") : filename(file) {}
    
     // Сохраняет ключевые переменные в JSON-файл
    void saveState(const VariableDatabase& db) {
        std::ofstream file(filename);
        if (file.is_open()) {
            // Получаем текущее время
            std::time_t now = std::time(nullptr);
            std::tm* localTime = std::localtime(&now);
            
            // Начинаем JSON
            file << "{\n";
            
            // Сохраняем переменные в формате JSON
            file << "  \"temperature_value\": " << std::fixed << std::setprecision(2) 
                 << db.getVariable("temperature_value") << ",\n";
            file << "  \"pressure_value\": " << std::fixed << std::setprecision(2) 
                 << db.getVariable("pressure_value") << ",\n";
            file << "  \"setpoint_value\": " << std::fixed << std::setprecision(2) 
                 << db.getVariable("setpoint_value") << ",\n";
            file << "  \"panel_status\": " << static_cast<int>(db.getVariable("panel_status")) << ",\n";
            
            // Сохраняем время в читаемом формате
            char timeStr[100];
            std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localTime);
            file << "  \"saved_at\": \"" << timeStr << "\",\n";
            
            // Сохраняем timestamp (секунды с 1970-01-01)
            file << "  \"timestamp\": " << now << "\n";
            
            // Заканчиваем JSON
            file << "}\n";
            
            file.close();
            Logger::info("State saved to " + filename);
        } else {
            Logger::error("Failed to save state to " + filename);
        }
    }
    
    void loadState(VariableDatabase& db) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            Logger::warning("State file not found: " + filename);
            return;
        }
        
        // Простой парсинг JSON
        std::string line;
        std::string content;
        while (std::getline(file, line)) {
            content += line;
        }
        file.close();
        
        // Ищем и парсим значения
        size_t pos;
        
        // Температура
        pos = content.find("\"temperature_value\":");
        if (pos != std::string::npos) {
            pos += 20;
            size_t end = content.find(",", pos);
            std::string valueStr = content.substr(pos, end - pos);
            try {
                double value = std::stod(valueStr);
                db.setVariable("temperature_value", value);
                Logger::info("Loaded temperature_value = " + std::to_string(value));
            } catch (...) {}
        }
        
        // Давление
        pos = content.find("\"pressure_value\":");
        if (pos != std::string::npos) {
            pos += 18;
            size_t end = content.find(",", pos);
            std::string valueStr = content.substr(pos, end - pos);
            try {
                double value = std::stod(valueStr);
                db.setVariable("pressure_value", value);
                Logger::info("Loaded pressure_value = " + std::to_string(value));
            } catch (...) {}
        }
        
        // Уставка
        pos = content.find("\"setpoint_value\":");
        if (pos != std::string::npos) {
            pos += 18;
            size_t end = content.find(",", pos);
            std::string valueStr = content.substr(pos, end - pos);
            try {
                double value = std::stod(valueStr);
                db.setVariable("setpoint_value", value);
                Logger::info("Loaded setpoint_value = " + std::to_string(value));
            } catch (...) {}
        }
        
        // Статус панели
        pos = content.find("\"panel_status\":");
        if (pos != std::string::npos) {
            pos += 15;
            size_t end = content.find(",", pos);
            std::string valueStr = content.substr(pos, end - pos);
            try {
                double value = std::stod(valueStr);
                db.setVariable("panel_status", value);
                Logger::info("Loaded panel_status = " + std::to_string(value));
            } catch (...) {}
        }
        
        // Читаем время сохранения (если есть)
        pos = content.find("\"saved_at\":");
        if (pos != std::string::npos) {
            pos += 11;
            size_t end = content.find("\"", pos + 1);
            std::string savedTime = content.substr(pos + 1, end - pos - 1);
            Logger::info("File was saved at: " + savedTime);
        }
        
        Logger::info("State loaded from " + filename);
    }
};

#endif