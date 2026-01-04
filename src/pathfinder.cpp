#include <iostream>
#include <limits>
#include "../include/Pathfinder.h"

using namespace std;

// fctie pentru distante
static int abs(int x)
{
    if (x < 0)
        return -x;
    else
        return x;
}

Pathfinder::Pathfinder(MapRuntime *m) : map(m)
{
    w = map->getWidth();
    h = map->getHeight();
    n = w * h;

    g = new int[n];
    f = new int[n];
    parent = new int[n];
    open = new char[n];
    closed = new char[n];
}

int Pathfinder::heuristic(int x1, int y1, int x2, int y2) const
{
    int s = abs(x1 - x2) + abs(y1 - y2);
    return s;
}

int Pathfinder::extractMinOpen() const
{
    int best = -1;
    int bestF = INFINITY;
    for (int i = 0; i < n; i++)
    {
        if (open[i])
        {
            if (f[i] < bestF)
            {
                bestF = f[i];
                best = i;
            }
        }
    }
    return best;
}

bool Pathfinder::compute(Agent *a, int sx, int sy, int tx, int ty)
{
    if (!map->inside(sx, sy) || !map->inside(tx, ty))
        return false;
    if (!a->canTraverse(map->get(tx, ty)))
        return false;

    for (int i = 0; i < n; i++)
    {
        g[i] = INFINITY;
        f[i] = INFINITY;
        parent[i] = -1;
        open[i] = 0;
        closed[i] = 0;
    }

    int start = index(sx, sy);
    int end = index(tx, ty);

    g[start] = 0;
    f[start] = heuristic(sx, sy, tx, ty);
    open[start] = 1;

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    while (1)
    {
        int cur = extractMinOpen();
        if (cur == -1)
            return false;
        if (cur == end)
            break;

        open[cur] = 0;
        closed[cur] = 1;

        int cx, cy;
        xy(cur, cx, cy);

        for (int k = 0; k < 4; k++)
        {
            int nx = cx + dx[k];
            int ny = cy + dy[k];
            if (!map->inside(nx, ny))
                continue;
            if (!a->canTraverse(map->get(nx, ny)))
                continue;

            int ni = index(nx, ny);
            if (closed[ni])
                continue;

            int g2 = g[cur] + 1;
            if (!open[ni] ||g2 < g[ni])
            {
                parent[ni] = cur;
                g[ni] = g2;
                f[ni] = g2 + heuristic(nx, ny, tx, ty);
                open[ni] = 1;
            }
        }
    }

    // reconstructia drumului
    int length = 0;
    int p = end;
    while (p != -1 && p != start)
    {
        length++;
        p = parent[p];
    }
    if (p == -1)
        return false;

    int *px = new int[length];
    int *py = new int[length];

    int cur = end;
    for (int i = length - 1; i >= 0; i--)
    {
        int x, y;
        xy(cur, x, y);
        px[i] = x;
        py[i] = y;
        cur = parent[cur];
    }

    a->getPath().set(px, py, length);
    return true;
}

Pathfinder::~Pathfinder()
{
    delete[] g;
    delete[] f;
    delete[] parent;
    delete[] open;
    delete[] closed;
}