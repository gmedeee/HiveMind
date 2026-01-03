#include "../include/Report.h"

using namespace std;

void Report::write(const char *filename, int ticks, int packagesTotal, int packagesDelivered, int agentsTotal, int agentsDead, int profitFinal)
{
    ofstream out(filename);
    if (!out.is_open())
        return;
    out << "Ticks: " << ticks << endl;
    out << "Packages Total: " << packagesTotal << endl;
    out << "Packages Delivered: " << packagesDelivered << endl;
    out << "Agents Total: " << agentsTotal << endl;
    out << "Agents Lost (DEAD): " << agentsDead << endl;
    out << "Final Profit: " << profitFinal << endl;
    out.close();
}
