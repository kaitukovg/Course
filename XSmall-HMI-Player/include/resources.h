#pragma once
#include <string>

/**
 * Вспомогательная функция для построения путей к ресурсам.
 * Гарантирует, что все пути начинаются с префикса assets/
 */
inline std::string assets_path(const std::string& p){
    return std::string("assets/") + p;
}
