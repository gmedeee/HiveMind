#pragma once
#include <fstream>

class Report {
public:
    static void write(const char *filename,int ticks,int packagesTotal,int packagesDelivered,int agentsTotal,int agentsDead,int profitFinal);
};
