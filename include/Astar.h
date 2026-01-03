#pragma once
#include "../include/Map.h"
#include "../include/Agent.h"

class AStar
{
    MapRuntime *map; // aici aveam nevoie de harta la runtime ca sa separ logica de loading din fisier de cea pe care lucrez efectiv
    int w;
    int h;
    int n;
    int *g;
    int *f;
    int *parent;
    char *open;
    char *closed;

    int index(int x, int y) const
    {
        return y * w + x;
    }
    void xy(int index, int &x, int &y) const
    {
        y = index / w;
        x = index % w;
    }

    int heuristic(int x1, int y1, int x2, int y2) const;
    int extractMinOpen() const;

public:
    AStar(MapRuntime *m);
    bool compute(Agent *a, int sx, int sy, int tx, int ty);
    ~AStar();
};
