#include "HmiPlayer.h"
#include "logger.h"

// Главная функция, с которой начинается выполнение программы
int main() {
    Logger::info("Запускаю XSmall-HMI SCADA Player...");
    
    // Создаем главный объект приложения
    HmiPlayer player;
    
    // Пробуем инициализировать
    if (!player.initialize()) {
        Logger::error("Не удалось инициализировать HMI Player");
        return -1;  // Возвращаем код ошибки
    }
    
    Logger::info("HMI Player успешно инициализирован");
    player.run();  // Запускаем главный цикл
    
    return 0;  // Успешное завершение
}