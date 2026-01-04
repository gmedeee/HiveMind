#include "../include/AgentSelector.h"
#include "../include/Utils.h"

using namespace std;

Agent *AgentSelector::packageAgent(int tick, Package &p, Agent **agents, int agentCount, Client *clients, int clientCount)
{
    if (!p.isWaiting())
        return 0;
    if (p.getAgent() != -1)
        return 0;
    int j = p.getClient();
    if (j < 0 || j >= clientCount)
        return 0;

    Position dest = clients[j].getPos();

    Agent *best = 0;
    int bestScore = -INFINITY;

    for (int i = 0; i < agentCount; i++)
    {
        //verificam agentii disponibili
        Agent *a = agents[i];
        if (strcmp(a->getState(), "DEAD") == 0)
            continue;
        if (a->getPackage() != -1)
            continue; 

        Position ap = a->getPos();
        int dist = manhattan(ap.getX(), ap.getY(), dest.getX(), dest.getY());
        int eta = cellDiv(dist, a->getSpeed());

        int score = p.getReward() - eta * a->getCost();
        if (tick + eta > p.getDueTick())
            score -= 200; // penalty pt intarziere

        if (score > bestScore)
        {
            bestScore = score;
            best = a;
        }
    }

    return best;
}
