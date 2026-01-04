#pragma once
#include <iostream>
#include <cstring>
#include <limits>
#include "Agent.h"
#include "Client.h"
#include "PackageManager.h"
#include "Map.h"

class AgentSelector
{
    // cat de departe este un agent de client
    int manhattan(int x1, int y1, int x2, int y2) const 
    {
        int dx = x1 - x2;
        if (dx < 0)
            dx = -dx;
        int dy = y1 - y2;
        if (dy < 0)
            dy = -dy;
        return dx + dy;
    }

    // implementata pentru a vedea de cate tick-uri e nev (dist/speed = timp)
    int celDiv(int a, int b) const { 
        return (a + b - 1) / b; 
    }

public:
    Agent *packageAgent(int tick, Package &p, Agent **agents, int agentCount, Client *clients, int clientCount);
};
