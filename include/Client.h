#pragma once
#include "Position.h"

class Client
{
    int id;
    Position pos;

public:
    Client() : id(-1) {}
    Client(int nid, int x, int y) : id(nid)
    {
        pos.set(x, y);
    }

    int getId() const
    {
        return id;
    }
    Position getPos() const
    {
        return pos;
    }
};
