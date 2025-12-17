#include <iostream>

using namespace std;

class IMapGenerator
{
public:
    virtual void load() = 0;
    virtual void generate(int width, int height) = 0;
};

class ProceduralMapGenerator : public IMapGenerator
{
public:
    void generate(int width, int height) override;
    void load() override;
};

class FileMapLoader : public IMapGenerator
{
    void generate(int width, int height) override;
    void load() override;
};

class MapGeneratorContext
{
    IMapGenerator *strategy;
public:
    void setStrategy(IMapGenerator *strategy);
    void executeGenerate(int width, int height);
    void executeLoad();
};