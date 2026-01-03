#pragma once
#include <iostream>

class IMapGenerator
{
public:
    virtual void load() = 0;
    virtual void generate(int width, int height) = 0;
    virtual ~IMapGenerator() {}
};

class ProceduralMapGenerator : public IMapGenerator
{
    int destination; 
    int station;

public:
    ProceduralMapGenerator() : destination(10), station(3) {}
    void setCounts(int clients, int stations)
    {
        destination = clients;
        station = stations;
    }

    void generate(int width, int height) override;
    void load() override;
};

class FileMapLoader : public IMapGenerator
{
public:
    void generate(int width, int height) override;
    void load() override;
};

class MapGeneratorContext
{
    IMapGenerator *strategy;

public:
    MapGeneratorContext() : strategy(0) {}
    void setStrategy(IMapGenerator *s)
    {
        strategy = s;
    }
    void executeGenerate(int w, int h)
    {
        if (strategy)
            strategy->generate(w, h);
    }
    void executeLoad()
    {
        if (strategy)
            strategy->load();
    }
};

class MapRuntime // am creat aceasta clasa ca sa fie salvata in memorie si folosita ulterior direct fara a fi nevoie mereu de citire din fisiere + metode de parcurgere pentru A*
{
    int width, height;
    char **mat;

public:
    MapRuntime() : width(0), height(0), mat(0) {}
    ~MapRuntime();

    bool loadFromFile(const char *filename);

    int getWidth() const
    {
        return width;
    }
    int getHeight() const
    {
        return height;
    }

    bool inside(int x, int y) const;
    char get(int x, int y) const;

    bool findFirst(char c, int &outX, int &outY) const;
};
