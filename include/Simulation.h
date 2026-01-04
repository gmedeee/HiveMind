#pragma once
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "../include/Map.h"
#include "../include/Agent.h"
#include "../include/Pathfinder.h"
#include "../include/Client.h"
#include "../include/PackageManager.h"
#include "../include/Profit.h"
#include "../include/AgentSelector.h"
#include "../include/Config.h"
#include "../include/Report.h"

class Simulation
{
    MapRuntime *map;
    Config config;
    Agent **agents;
    int agentCount;
    int agentCap;
    Client *clients;
    int clientCount;
    int baseX, baseY;
    Position *chargers;
    int chargerCount;
    PackageManager *packageManager;
    Profit profit;
    AgentSelector agentSelector;
    Pathfinder *pathfinder;

    void agentsCapacity();
    void initClients();
    void initCharge();
    int abs(int x) const
    {
        if (x < 0)
            return -x;
        else
            return x;
    }
    int manhattan(int x1, int y1, int x2, int y2) const
    {
        int s = abs(x1 - x2) + abs(y1 - y2);
        return s;
    }

    char isChargingCell(char c) const
    {
        if (c == 'B' || c == 'S')
            return c;
    }

    void chargeAgent(Agent *a);

    void spawnPackages(int tick);
    void updates(int tick);
    void moveAgents(int tick);
    void delivery(int tick);
    void chargeDistance(Agent *a);
    int nearestCharger(Agent *a, int &outX, int &outY) const;
    void printFinal(int ticks);

public:
    Simulation(MapRuntime *m, const Config &c);
    void addAgent(Agent *a);
    void run(int ticks);
    ~Simulation();
};
