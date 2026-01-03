#pragma once

class Package
{
    int id;
    int spawnTick;
    int dueTick;
    int reward;
    int client;
    int agent;
    char status; // W - waiting la baza, T- trece de la o celula la alta cu ajutorul agentului, D- delivered, F -  failed
    int pickedTick;
    int deliveredTick;

public:
    Package() : id(-1), spawnTick(0), dueTick(0), reward(0), client(-1), agent(-1), status('W'), pickedTick(-1), deliveredTick(-1) {}

    // la spawn-ul pachetului
    void init(int nid, int tick, int deadlineTicks, int rew, int cIndex)
    {
        id = nid;
        spawnTick = tick;
        dueTick = tick + deadlineTicks;
        reward = rew;
        client = cIndex;
        agent= -1;
        status = 'W';
        pickedTick = -1;
        deliveredTick = -1;
    }

    int getId() const
    {
        return id;
    }
    int getSpawnTick() const
    {
        return spawnTick;
    }
    int getDueTick() const
    {
        return dueTick;
    }
    int getReward() const
    {
        return reward;
    }
    int getClient() const
    {
        return client;
    }

    int getAgent() const
    {

        return agent;
    }
    void setAgent(int aid)
    {
        agent = aid;
    }

    char getStatus() const
    {
        return status;
    }
    void setStatus(char s)
    {
        status = s;
    }

    int getPickedTick() const
    {
        return pickedTick;
    }
    void setPickedTick(int t)
    {
        pickedTick = t;
    }

    int getDeliveredTick() const
    {
        return deliveredTick;
    }
    void setDeliveredTick(int t)
    {
        deliveredTick = t;
    }
    bool isWaiting() const
    {
        return status == 'W';
    }
    bool isTransit() const
    {
        return status == 'T';
    }
    bool isDelivered() const
    {
        return status == 'D';
    }
    bool isFailed() const
    {
        return status == 'F';
    }
};
