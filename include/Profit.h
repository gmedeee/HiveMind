#pragma once

class Profit
{
    int total;
    int deadAgents;
    int latePackages;
    int undeliveredPackages;

public:
    Profit() : total(0), deadAgents(0), latePackages(0), undeliveredPackages(0) {}

    void addReward(int r)
    {
        total += r;
    }
    void addCost(int c)
    {
        total -= c;
    }

    void addDeadAgent()
    {
        total -= 500;
        deadAgents++;
    }
    void addLate()
    {
        total -= 50;
        latePackages++;
    }
    void addUndelivered()
    {
        total -= 200;
        undeliveredPackages++;
    }

    int getTotal() const
    {
        return total;
    }
    int getDeadAgents() const
    {
        return deadAgents;
    }
    int getLatePackages() const
    {
        return latePackages;
    }
    int getUndeliveredPackages() const
    {
        return undeliveredPackages;
    }
};
