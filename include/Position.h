#pragma once

class Position
{
    int x, y;

public:
    Position() : x(0), y(0) {} //daca nu exista niciun input de creare, default sunt 0 0 
    Position(int nx, int ny) : x(nx), y(ny) {}

    int getX() const
    {
        return x;
    }
    int getY() const
    {
        return y;
    }

    void set(int nx, int ny)
    {
        x = nx;
        y = ny;
    }
};
