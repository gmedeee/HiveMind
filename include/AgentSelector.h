#pragma once
#include <iostream>
#include <cstring>
#include <limits>
#include "Agent.h"
#include "Client.h"
#include "PackageManager.h"
#include "Map.h"
#include "Utils.h"

class AgentSelector
{
    // cat de departe este un agent de client
   int manhattan(int x1, int y1, int x2, int y2) const {
    return absT(x1 - x2) + absT(y1 - y2);
}

public:
    Agent *packageAgent(int tick, Package &p, Agent **agents, int agentCount, Client *clients, int clientCount);
};
