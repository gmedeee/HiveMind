#pragma once
#include <cstdlib>
#include "Package.h"

using namespace std;

class PackageManager
{
    Package *package;
    int count;
    int cap;
    int nextID;
    int spawnInterval;

public:
    PackageManager(int capacity, int interval) : package(0), count(0), cap(capacity), nextID(0), spawnInterval(interval)
    {
        package = new Package[cap];
    }

    int getCount() const
    {
        return count;
    }
    Package &get(int i)
    {
        return package[i];
    }
    const Package &get(int i) const
    {
        return package[i];
    }

    void trySpawn(int tick, int clientCount)
    {
        if (clientCount <= 0)
            return;
        if (count >= cap)
            return;
        if (spawnInterval <= 0)
            return;
        if (tick % spawnInterval != 0)
            return;

        int reward = 200 + (rand() % 601); // intre 200 si 800
        int deadline = 10 + (rand() % 11); // intre 10 si 20
        int cIndex = rand() % clientCount;

        package[count].init(nextID++, tick, deadline, reward, cIndex);
        count++;
    }

    ~PackageManager() { delete[] package; }
};
