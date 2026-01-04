#include "../include/ConfigLoader.h"
#include <fstream>

using namespace std;

bool ConfigLoader::load(const char *filename, Config &config) {
    ifstream file(filename);
    if (!file.is_open())
        return false;

    file>>config.width;
    file>>config.height;
    file>>config.maxTicks;
    file>>config.maxStations;
    file>>config.clientsCount;
    file>>config.drones;
    file>>config.robots;
    file>>config.scooters;
    file>>config.totalPackages;
    file>>config.spawnFrequency;

    file.close();

    return true;
}
