#pragma once
#include <fstream>
#include <cstring>
#include "Config.h"

class ConfigLoader {
public:
   
    static bool load(const char *filename, Config &config);

private:
    static void trim(char *s);
    static void parseLine(const char *line, char *keyOut, char *valOut);
};
