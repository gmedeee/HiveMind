#include <cstdlib>
#include <ctime>
#include <iostream>
#include "../include/Map.h"
#include "../include/Simulation.h"
#include "../include/Agent.h"
#include "../include/Config.h"
#include "../include/ConfigLoader.h"

using namespace std;

int main() {
    srand((unsigned)time(0));

    Config config;
    if (!ConfigLoader::load("simulation_setup.txt", config)) {
        cout <<"Error in opening file!"<<endl; // se vor folosi datele default din config.h
    }

    MapGeneratorContext map;
    ProceduralMapGenerator gen;
    gen.setCounts(config.clientsCount,config.maxStations);

    map.setStrategy(&gen);
    map.executeGenerate(config.mapW, config.mapH);

    MapRuntime runtimeMap;
    if (!runtimeMap.loadFromFile("mapSimulator.txt")) {
        cout << "Error in opening file!"<<endl;
        return 1;
    }

    int bx = 0, by = 0;
    if (!runtimeMap.findFirst('B', bx, by)) {
        cout << "No base found"<<endl;
        return 1;
    }

    Simulation sim(&runtimeMap, config);

    //spawnez angetii
    for (int i = 0; i < config.drones; i++) {
        Agent *a = new Drone();
        a->setPos(bx, by);
        sim.addAgent(a);
    }
    for (int i = 0; i < config.robots; i++) {
        Agent *a = new Robot();
        a->setPos(bx, by);
        sim.addAgent(a);
    }
    for (int i = 0; i < config.scooters; i++) {
        Agent *a = new Scooter();
        a->setPos(bx, by);
        sim.addAgent(a);
    }

    sim.run(config.maxTicks);
}
