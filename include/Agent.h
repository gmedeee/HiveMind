#pragma once
#include <cstring>
#include "Position.h"
#include "Path.h"

class Agent
{
protected:
    int id;
    int battery;
    char *state;
    Position pos;
    int targetX, targetY;
    char hasTarget;
    char deadCounted;
    int package;
    Path path;

public:
    Agent() : id(-1), battery(0), state(0), targetX(0), targetY(0), hasTarget(0), deadCounted(0), package(-1) {}

    virtual const char getSymbol() const = 0;
    virtual int getSpeed() const = 0;
    virtual int getBatteryMax() const = 0;
    virtual int getConsumption() const = 0;
    virtual int getCost() const = 0;
    virtual int getCapacity() const = 0;
    virtual bool canTraverse(char cell) const = 0; // metoda pt a vedea daca poate trece peste # sau nu ( daca e drona sau nu )

    void setId(int nid)
    {
        id = nid;
    }
    int getId() const
    {
        return id;
    }

    void setBattery(int b)
    {
        battery = b;
    }
    int getBattery() const
    {
        return battery;
    }

    void setPos(int x, int y)
    {
        pos.set(x, y);
    }
    Position getPos() const
    {
        return pos;
    }

    void setState(const char *s)
    {
        if (state)
            delete[] state;
        state = new char[(int)strlen(s) + 1];
        strcpy(state, s);
    }
    const char *getState() const
    {
        if(state != NULL)
        return state;
        else 
        return "IDLE";
    }

    void setTarget(int x, int y)
    {
        targetX = x;
        targetY = y;
        hasTarget = 1;
    }
    void clearTarget()
    {
        hasTarget = 0;
    }
    char hasTargetSet() const
    {
        return hasTarget;
    }
    int getTargetX() const
    {
        return targetX;
    }
    int getTargetY() const
    {
        return targetY;
    }

    char isDeadCounted() const
    {
        return deadCounted;
    }
    void setDeadCounted(char v)
    {
        deadCounted = v;
    }

    int getPackage() const
    {
        return package;
    }
    void setPackage(int i)
    {
        package = i;
    }

    Path& getPath()
    {
        return path;
    }
    void clearPath()
    {
        path.clear();
    }

    virtual ~Agent()
    {
        if (state)
            delete[] state;
    }
};

class Drone : public Agent
{
public:
    //sunt statice deoarece atributele apartin intregii clase, nu doar obiectului
    static const char symbol; 
    static int speed, batteryMax, consumption, cost, capacity;

    const char getSymbol() const override;
    int getSpeed() const override;
    int getBatteryMax() const override;
    int getConsumption() const override;
    int getCost() const override;
    int getCapacity() const override;
    bool canTraverse(char cell) const override;
};

class Robot : public Agent
{
public:
    static const char symbol;
    static int speed, batteryMax, consumption, cost, capacity;

    const char getSymbol() const override;
    int getSpeed() const override;
    int getBatteryMax() const override;
    int getConsumption() const override;
    int getCost() const override;
    int getCapacity() const override;
    bool canTraverse(char cell) const override;
};

class Scooter : public Agent
{
public:
    static const char symbol;
    static int speed, batteryMax, consumption, cost, capacity;
    
    const char getSymbol() const override;
    int getSpeed() const override;
    int getBatteryMax() const override;
    int getConsumption() const override;
    int getCost() const override;
    int getCapacity() const override;
    bool canTraverse(char cell) const override;
};
