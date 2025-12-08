#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "VariableDatabase.h"
#include <string>
#include <map>

class ConfigLoader {
public:
    static bool loadFromJSON(const std::string& filename, VariableDatabase& database);
    static std::map<std::string, double> parseJSON(const std::string& filename);
};

#endif