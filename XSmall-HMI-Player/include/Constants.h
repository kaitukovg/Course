#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

/**
 * Централизованное хранение констант проекта.
 * Упрощает изменение настроек и поддержку кода.
 */
namespace Constants {
    const int WINDOW_WIDTH = 1024;  // Ширина основное окна 
    const int WINDOW_HEIGHT = 768;  // Высота основного окна
    const std::string FONT_PATH = "assets/fonts/helveticabold.ttf";  // Основной шрифт

    // Для статической/динамической сборки
    #ifdef SFML_STATIC
        const bool IS_STATIC_BUILD = true;
    #else
        const bool IS_STATIC_BUILD = false;
    #endif
}

#endif

