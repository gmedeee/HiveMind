#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>

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
    void generate(int width, int height) override
    {
        srand((unsigned)time(nullptr));

        const int D = 10, S = 3, B = 1;
        int total = height * width;

        char *bag = new char[total];
        int k = 0;

        for (int i = 0; i < D; i++)
            bag[k++] = 'D';
        for (int i = 0; i < S; i++)
            bag[k++] = 'S';
        for (int i = 0; i < B; i++)
            bag[k++] = 'B';

        const char letters[2] = {'.', '#'};
        while (k < total)
        {
            bag[k++] = letters[rand() % 2];
        }

        for (int i = total - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            char tmp = bag[i];
            bag[i] = bag[j];
            bag[j] = tmp;
        }

        char **mat = new char *[height];
        for (int i = 0; i < height; i++)
            mat[i] = new char[width];

        k = 0;
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                mat[i][j] = bag[k++];

        ofstream mapSimulator("mapSimulator.txt");

        if (mapSimulator.is_open())
        {
            cout << "File opened successfully." << endl;

            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                    mapSimulator << mat[i][j];
                mapSimulator << '\n';
            }

            mapSimulator.close();

            cout << "File closed." << endl;
        }
        else
        {

            cout << "Error opening file!" << endl;
        }
    }
    void load() override {}
};

class FileMapLoader : public IMapGenerator
{
public:
    void load() override
    {
        string buffer;
        ifstream mapSimulator("mapSimulator.txt");

        if (mapSimulator.is_open())
        {
            cout << "File opened successfully." << endl;

            while (getline(mapSimulator, buffer))
            {
                cout << buffer <<endl;
            }

            mapSimulator.close();

            cout << "File closed." << endl;
        }
        else
        {
            cout << "Error opening file!" << endl;
        }
    }

    void generate(int width, int height) override {}
};

class MapGeneratorContext {
private:
    IMapGenerator* strategy;

public:
    void setStrategy(IMapGenerator* strategy) {
        this->strategy = strategy;
    }

    void executeGenerate(int width, int height) {
        strategy->generate(width,height);
    }

    void executeLoad(){
        strategy->load();
    }
};

int main()
{
    MapGeneratorContext map;
    ProceduralMapGenerator generator;
    FileMapLoader loader;

    int width = 20;
    int height = 20;

    map.setStrategy(&generator);
    map.executeGenerate(width,height);

    map.setStrategy(&loader);
    map.executeLoad();

    return 0;
}
