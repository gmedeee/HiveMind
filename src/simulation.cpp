#include "../include/Simulation.h"
#include "../include/Utils.h"

Simulation::Simulation(MapRuntime *m, const Config &c) : map(m), config(c), agents(0), agentCount(0), agentCap(8), clients(0), clientCount(0), baseX(0), baseY(0), chargers(0), chargerCount(0), packageManager(0), pathfinder(0)
{
    agents = new Agent *[agentCap];
    for (int i = 0; i < agentCap; i++)
        agents[i] = 0;

    initClients();
    initCharge();

    packageManager = new PackageManager(config.totalPackages, config.spawnFrequency);

    pathfinder = new Pathfinder(map);
}

void Simulation::agentsCapacity()
{
    if (agentCount < agentCap)
        return;
    int capacity = agentCap * 2;
    Agent **agentsNew = new Agent *[capacity];
    for (int i = 0; i < capacity; i++)
        agentsNew[i] = 0;
    for (int i = 0; i < agentCap; i++)
        agentsNew[i] = agents[i];
    delete[] agents;
    agents = agentsNew;
    agentCap = capacity;
}

void Simulation::addAgent(Agent *a)
{
    agentsCapacity();
    a->setId(agentCount);
    a->setBattery(a->getBatteryMax());
    a->setState("IDLE");
    a->setDeadCounted(0);
    a->setPackage(-1);
    a->clearTarget();
    a->clearPath();
    agents[agentCount++] = a;
}

void Simulation::initClients()
{
    clientCount = 0;
    for (int y = 0; y < map->getHeight(); y++)
        for (int x = 0; x < map->getWidth(); x++)
            if (map->get(x, y) == 'D')
                clientCount++;

    if (clientCount > 0)
        clients = new Client[clientCount];
    else
        clients = 0;

    int k = 0;
    for (int y = 0; y < map->getHeight(); y++)
        for (int x = 0; x < map->getWidth(); x++)
            if (map->get(x, y) == 'D')
            {
                clients[k] = Client(k, x, y);
                k++;
            }
}

void Simulation::initCharge()
{
    // Baza
    if (!map->findFirst('B', baseX, baseY))
    {
        baseX = 0;
        baseY = 0;
    }

    // Chargers = B + toate S
    chargerCount = 1; // base
    for (int y = 0; y < map->getHeight(); y++)
        for (int x = 0; x < map->getWidth(); x++)
            if (map->get(x, y) == 'S')
                chargerCount++;

    chargers = new Position[chargerCount];
    chargers[0].set(baseX, baseY);

    int k = 1;
    for (int y = 0; y < map->getHeight(); y++)
        for (int x = 0; x < map->getWidth(); x++)
            if (map->get(x, y) == 'S')
            {
                chargers[k].set(x, y);
                k++;
            }
}

void Simulation::chargeAgent(Agent *a)
{
    int add = a->getBatteryMax() / 4; // +25%
    int nb = a->getBattery() + add;
    if (nb > a->getBatteryMax())
        nb = a->getBatteryMax();
    a->setBattery(nb);
}

int Simulation::nearestCharger(Agent *a, int &outX, int &outY) const
{
    Position p = a->getPos();
    int bestD = 100000;
    int bx = baseX, by = baseY;

    for (int i = 0; i < chargerCount; i++)
    {
        int cx = chargers[i].getX();
        int cy = chargers[i].getY();
        int d = manhattan(p.getX(), p.getY(), cx, cy);
        if (d < bestD)
        {
            bestD = d;
            bx = cx;
            by = cy;
        }
    }

    outX = bx;
    outY = by;
    return bestD;
}

void Simulation::chargeDistance(Agent *a)
{
    if (strcmp(a->getState(), "DEAD") == 0)
        return;

    Position p = a->getPos();
    char cell = map->get(p.getX(), p.getY());
    if (isChargingCell(cell))
        return;

    if (!a->hasTargetSet())
        return;

    int tx = a->getTargetX(), ty = a->getTargetY();
    int dist = manhattan(p.getX(), p.getY(), tx, ty);

    int sp = a->getSpeed();
    int etaTicks = cellDiv(dist, sp); // dist/speed = ticks

    int need = a->getConsumption() * (etaTicks + 3); // +3 buffer

    if (a->getBattery() < need)
    {
        int cx, cy;
        nearestCharger(a, cx, cy);
        a->setTarget(cx, cy);
        a->clearPath();
        a->setState("MOVING");
    }
}

void Simulation::spawnPackages(int tick)
{
    // spawn pachete (din config totalPackages + spawnFrequency)
    packageManager->trySpawn(tick, clientCount);

    // aloca pachete waiting
    for (int i = 0; i < packageManager->getCount(); i++)
    {
        Package &p = packageManager->get(i);
        if (!p.isWaiting())
            continue;
        if (p.getAgent() != -1)
            continue;

        Agent *a = agentSelector.packageAgent(tick, p, agents, agentCount, clients, clientCount);
        if (!a)
            continue;

        p.setAgent(a->getId());
        a->setPackage(i);

        // merge la baza pentru pickup
        a->setTarget(baseX, baseY);
        a->clearPath();
        a->setState("MOVING");
    }
}

void Simulation::updates(int tick)
{
    (void)tick;

    for (int i = 0; i < agentCount; i++)
    {
        Agent *a = agents[i];
        if (strcmp(a->getState(), "DEAD") == 0)
            continue;

        Position p = a->getPos();
        char cell = map->get(p.getX(), p.getY());

        if (isChargingCell(cell))
            a->setState("CHARGING");

        if (strcmp(a->getState(), "CHARGING") == 0)
        {
            chargeAgent(a);
            if (a->getBattery() >= a->getBatteryMax())
            {
                if (a->hasTargetSet())
                    a->setState("MOVING");
                else
                    a->setState("IDLE");
            }
        }
        else if (strcmp(a->getState(), "IDLE") == 0)
        {
            if (isChargingCell(cell))
                chargeAgent(a);
            if (a->hasTargetSet())
                a->setState("MOVING");
        }
        else if (strcmp(a->getState(), "MOVING") == 0)
        {
            int nb = a->getBattery() - a->getConsumption();
            if (nb < 0)
                nb = 0;
            a->setBattery(nb);

            if (a->getBattery() == 0 && !isChargingCell(cell))
            {
                a->setState("DEAD");
                if (!a->isDeadCounted())
                {
                    profit.addDeadAgent();
                    a->setDeadCounted(1);
                }
            }
        }

        if (strcmp(a->getState(), "DEAD") != 0)
            chargeDistance(a);
    }

    for (int i = 0; i < agentCount; i++)
    {
        const char *st = agents[i]->getState();
        if (strcmp(st, "MOVING") == 0 || strcmp(st, "CHARGING") == 0)
        {
            profit.addCost(agents[i]->getCost());
        }
    }
}

void Simulation::moveAgents(int tick)
{
    (void)tick;

    // fiecare agent se misca independent, fara coliziuni
    for (int i = 0; i < agentCount; i++)
    {
        Agent *a = agents[i];
        if (strcmp(a->getState(), "DEAD") == 0)
            continue;
        if (strcmp(a->getState(), "MOVING") != 0)
            continue;
        if (!a->hasTargetSet())
            continue;

        Position p = a->getPos();
        int tx = a->getTargetX();
        int ty = a->getTargetY();

        //daca e deja la target
        if (p.getX() == tx && p.getY() == ty)
            continue;

        //daca nu are path, calculeaza A*
        if (!a->getPath().hasNext())
        {
            a->clearPath();
            pathfinder->compute(a, p.getX(), p.getY(), tx, ty);
        }

        //muta pana la speed pasi pe tick
        int steps = a->getSpeed();
        while (steps > 0 && a->getPath().hasNext())
        {
            int nx, ny;
            a->getPath().peekNext(nx, ny);

            //drona poate traversa orice
            if (!map->inside(nx, ny))
            {
                a->clearPath();
                break;
            }
            if (!a->canTraverse(map->get(nx, ny)))
            {
                a->clearPath();
                break;
            }

            a->setPos(nx, ny);
            a->getPath().advance();

            steps--;
        }
    }
}

void Simulation::delivery(int tick)
{
    for (int i = 0; i < agentCount; i++)
    {
        Agent *a = agents[i];
        if (strcmp(a->getState(), "DEAD") == 0)
            continue;

        int j = a->getPackage();
        if (j < 0)
            continue;
        if (j >= packageManager->getCount())
        {
            a->setPackage(-1);
            continue;
        }

        Package &p = packageManager->get(j);

        if (p.isDelivered() || p.isFailed())
        {
            a->setPackage(-1);
            continue;
        }

        Position ap = a->getPos();

        // pickup in base
        if (p.isWaiting())
        {
            if (ap.getX() == baseX && ap.getY() == baseY)
            {
                p.setStatus('T');
                p.setPickedTick(tick);

                int ci = p.getClient();
                if (ci >= 0 && ci < clientCount)
                {
                    Position dp = clients[ci].getPos();
                    a->setTarget(dp.getX(), dp.getY());
                    a->clearPath();
                    a->setState("MOVING");
                }
            }
            else
            {
                a->setTarget(baseX, baseY);
                a->clearPath();
                a->setState("MOVING");
            }
        }
        // delivery
        else if (p.isTransit())
        {
            int ci = p.getClient();
            if (ci >= 0 && ci < clientCount)
            {
                Position dp = clients[ci].getPos();
                if (ap.getX() == dp.getX() && ap.getY() == dp.getY())
                {
                    p.setStatus('D');
                    p.setDeliveredTick(tick);

                    profit.addReward(p.getReward());
                    if (tick > p.getDueTick())
                        profit.addLate();

                    a->setPackage(-1);
                    a->clearTarget();
                    a->clearPath();
                    a->setState("IDLE");
                }
                else
                {
                    a->setTarget(dp.getX(), dp.getY());
                }
            }
        }
    }
}

void Simulation::printFinal(int ticks)
{
    // penalizare pentru nelivrate la final
    for (int i = 0; i < packageManager->getCount(); i++)
    {
        Package &p = packageManager->get(i);
        if (!p.isDelivered())
            profit.addUndelivered();
    }

    int delivered = 0;
    for (int i = 0; i < packageManager->getCount(); i++)
        if (packageManager->get(i).isDelivered())
            delivered++;

    int dead = 0;
    for (int i = 0; i < agentCount; i++)
        if (strcmp(agents[i]->getState(), "DEAD") == 0)
            dead++;

    Report::write("simulation.txt",
                  ticks,
                  packageManager->getCount(),
                  delivered,
                  agentCount,
                  dead,
                  profit.getTotal());
}

void Simulation::run(int ticks)
{
    for (int t = 1; t <= ticks; t++)
    {
        spawnPackages(t);
        updates(t);
        moveAgents(t);
        delivery(t);
    }

    printFinal(ticks);
}

Simulation::~Simulation()
{
    for (int i = 0; i < agentCount; i++)
        delete agents[i];
    delete[] agents;

    if (clients)
        delete[] clients;
    if (chargers)
        delete[] chargers;

    delete packageManager;
    delete pathfinder;
}