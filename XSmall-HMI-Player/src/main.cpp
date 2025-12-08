#include "HmiPlayer.h"
#include "logger.h"

// Главная функция, с которой начинается выполнение программы
int main() {
    Logger::info("LAUNCHING XSmall-HMI SCADA Player...");
    
    // Создаем главный объект приложения
    HmiPlayer player;
    
    // Пробуем инициализировать
    if (!player.initialize()) {
        Logger::error("FAILED TO INITIALIZED HMI Player");
        return -1;  // Возвращаем код ошибки
    }
    
    Logger::info("HMI Player SUCCESSFULLY INITIALIZED. CONGRATULATIONS!");
    player.run();  // Запускаем главный цикл
    
    return 0;  // Успешное завершение
}
