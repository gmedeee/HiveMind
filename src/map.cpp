#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include "../include/Map.h"

using namespace std;

static bool walkable(char c)
{
    return c != '#'; // walkable daca e diferit de #
}

static bool validateMap(char **mat, int width, int height)
{
    int bx = -1, by = -1;
    int need = 0; // cate D + S trebuie atinse
    int total = width * height;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (mat[i][j] == 'B')
            {
                by = i;
                bx = j;
            }
            if (mat[i][j] == 'D' || mat[i][j] == 'S')
                need++;
        }
    }

    if (bx == -1 || by == -1)
        return false; // harta nu s-a generat bine, pt ca nu exista B

    if (need == 0)
        return false; // harta nu s-a generat bine, pentru ca nu exista niciun client sau statie

    // cream o matrice de marimea height x width doar cu bool false ca sa vedem ce celule am vizitat
    bool **vis = new bool *[height]; 
    for (int i = 0; i < height; i++)
    {
        vis[i] = new bool[width];
        for (int j = 0; j < width; j++)
            vis[i][j] = false; // la inceput toate sunt false, pt ca nu am vizitat niciuna
    }

    int *qi = new int[total];
    int *qj = new int[total];
    int head = 0;
    int tail = 0;

    vis[by][bx] = true;
    qi[tail] = by;
    qj[tail] = bx;
    tail++;

    int reached = 0;

    while (head < tail)
    {
        //scoatem din queue pozitia de analizat
        int ci = qi[head];
        int cj = qj[head];
        head++;

        if (mat[ci][cj] == 'D' || mat[ci][cj] == 'S')
            reached++;

        if (ci + 1 < height && vis[ci + 1][cj] == false && walkable(mat[ci + 1][cj]))
        {
            vis[ci + 1][cj] = true;
            qi[tail] = ci + 1;
            qj[tail] = cj;
            tail++;
        }
        if (ci - 1 >= 0 && vis[ci - 1][cj] == false && walkable(mat[ci - 1][cj]))
        {
            vis[ci - 1][cj] = true;
            qi[tail] = ci - 1;
            qj[tail] = cj;
            tail++;
        }
        if (cj + 1 < width && vis[ci][cj + 1] == false && walkable(mat[ci][cj + 1]))
        {
            vis[ci][cj + 1] = true;
            qi[tail] = ci;
            qj[tail] = cj + 1;
            tail++;
        }
        if (cj - 1 >= 0 && vis[ci][cj - 1] == false && walkable(mat[ci][cj - 1]))
        {
            vis[ci][cj - 1] = true;
            qi[tail] = ci;
            qj[tail] = cj - 1;
            tail++;
        }
    }

    delete[] qi;
    delete[] qj;
    for (int i = 0; i < height; i++)
        delete[] vis[i];
    delete[] vis;

    return reached == need; // daca numarul de reach-uri este egal cu cel de need-uri inseamna ca harta e valida, ca a ajuns la fiecare punct la care voiam
}

void ProceduralMapGenerator::generate(int width, int height)
{
    srand((unsigned)time(nullptr));

    const int D = 10, S = 3, B = 1;
    int total = width * height;
    
    char *bag = new char[total];

    char **mat = new char *[height];
    for (int i = 0; i < height; i++)
        mat[i] = new char[width];

    bool ok = false;
    int attempts = 0;

    while (!ok)
    {
        attempts++;

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
        
        while (k < total) { 
            bag[k++] = letters[rand() % 2]; 
        } 
        
        for (int i = total - 1; i > 0; i--) {
            int j = rand() % (i + 1); 
            char tmp = bag[i]; 
            bag[i] = bag[j]; 
            bag[j] = tmp; 
        } 
          
        k = 0; 
        for (int i = 0; i < height; i++) 
            for (int j = 0; j < width; j++) 
                mat[i][j] = bag[k++];

        ok = validateMap(mat, width, height);
    }

    if (!ok)
    {
        delete[] bag;
        for (int i = 0; i < height; i++)
            delete[] mat[i];
        delete[] mat;
        return;
    }

    cout << "Generated map after " << attempts << " attempts\n";

    // scriem fisierul cand e valid
    ofstream mapSimulator("mapSimulator.txt");
    if (mapSimulator.is_open())
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
                mapSimulator << mat[i][j];
            mapSimulator << '\n';
        }
        mapSimulator.close();
    }
    else
    {
        cout << "Error opening file!\n";
    }

    delete[] bag;
    for (int i = 0; i < height; i++)
        delete[] mat[i];
    delete[] mat;
}

void ProceduralMapGenerator::load() {}

void FileMapLoader::load()
{
    string buffer;
    ifstream mapSimulator("mapSimulator.txt");

    if (mapSimulator.is_open())
    {
        while (getline(mapSimulator, buffer))
            cout << buffer << endl;

        mapSimulator.close();
    }
    else
    {
        cout << "Error opening file!" << endl;
    }
}

void FileMapLoader::generate(int width, int height) {}

void MapGeneratorContext::setStrategy(IMapGenerator *strategy)
{
    this->strategy = strategy;
}

void MapGeneratorContext::executeGenerate(int width, int height)
{
    strategy->generate(width, height);
}

void MapGeneratorContext::executeLoad()
{
    strategy->load();
}
