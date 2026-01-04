#pragma once
#include "Config.h"

class ConfigLoader {
public:
    static bool load(const char *filename, Config &config);
};
