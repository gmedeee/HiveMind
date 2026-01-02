#include <iostream>
#include "../include/Map.h"

using namespace std;


int main()
{
    MapGeneratorContext map;
    ProceduralMapGenerator generator;
    FileMapLoader loader;

    int width = 20;
    int height = 20;

    map.setStrategy(&generator);
    map.executeGenerate(width, height);

    map.setStrategy(&loader);
    map.executeLoad();

    return 0;
}