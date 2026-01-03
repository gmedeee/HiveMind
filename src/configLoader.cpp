#include "../include/ConfigLoader.h"

void ConfigLoader::trim(char *s)
{
    int n = (int)strlen(s);
    while (n > 0 && (s[n - 1] == ' ' || s[n - 1] == '\t' || s[n - 1] == '\r' || s[n - 1] == '\n')) // separam fiecare cuvant in parte
        s[--n] = 0;

    int i = 0;
    while (s[i] == ' ' || s[i] == '\t')
        i++;

    if (i > 0)
    {
        int j = 0;
        while (s[i])
            s[j++] = s[i++];

        s[j] = 0;
    }
}

void ConfigLoader::parseLine(const char *line, char *keyOut, char *valOut)
{
    keyOut[0] = 0;
    valOut[0] = 0;

    // ignoram comentariile dupa //
    char buf[512];
    int k = 0;
    for (int i = 0; line[i] && k < 510; i++)
    {
        if (line[i] == '/' && line[i + 1] == '/')
            break;
        buf[k++] = line[i];
    }
    buf[k] = 0;

    // cautam :
    const char *p = strchr(buf, ':');
    if (!p)
        return;

    // key = stanga
    int keyLen = (int)(p - buf);
    if (keyLen <= 0)
        return;

    strncpy(keyOut, buf, keyLen);
    keyOut[keyLen] = 0;
    strcpy(valOut, p + 1);

    trim(keyOut);
    trim(valOut);
}

bool ConfigLoader::load(const char *filename, Config &config)
{
    std::ifstream in(filename);
    if (!in.is_open())
        return false;

    char line[512];
    while (in.getline(line, 512))
    {
        char key[128], val[128];
        parseLine(line, key, val);

        if (key[0] == 0)
            continue;

        if (strcmp(key, "MAP_SIZE") == 0)
        {
            // format: "20 20"
            int w = 0, h = 0;
            sscanf(val, "%d %d", &w, &h);
            if (w > 0 && h > 0)
            {
                config.mapW = w;
                config.mapH = h;
            }
        }
        else if (strcmp(key, "MAX_TICKS") == 0)
        {
            config.maxTicks = atoi(val);
        }
        else if (strcmp(key, "MAX_STATIONS") == 0)
        {
            config.maxStations = atoi(val);
        }
        else if (strcmp(key, "CLIENTS_COUNT") == 0)
        {
            config.clientsCount = atoi(val);
        }
        else if (strcmp(key, "DRONES") == 0)
        {
            config.drones = atoi(val);
        }
        else if (strcmp(key, "ROBOTS") == 0)
        {
            config.robots = atoi(val);
        }
        else if (strcmp(key, "SCOOTERS") == 0)
        {
            config.scooters = atoi(val);
        }
        else if (strcmp(key, "TOTAL_PACKAGES") == 0)
        {
            config.totalPackages = atoi(val);
        }
        else if (strcmp(key, "SPAWN_FREQUENCY") == 0)
        {
            config.spawnFrequency = atoi(val);
        }
    }

    in.close();
    return true;
}
