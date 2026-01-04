#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include "../include/Map.h"

using namespace std;

static bool walkable(char c)
{ // celula poate fi parcursa daca nu e zid
    return c != '#';
}

static bool validateMap(char **mat, int width, int height)
{
    // plecam de la niste valori default, pozitii care nu exista pt baza si 0 destinatii sau statii
    int bx = -1, by = -1;
    int need = 0;
    int total = width * height;

    for (int i = 0; i < height; i++)
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

    bool **vis = new bool *[height]; // cream o matrice care sa contina doar pointeri la val de tip boolean pentru a verifica daca am vizitat celula sau nu
    for (int i = 0; i < height; i++)
    {
        vis[i] = new bool[width];
        for (int j = 0; j < width; j++)
            vis[i][j] = false; // la inceput toate false
    }

    // cream o coada in care sa tinem vecinii pe care urmeaza sa-i vizitam
    int *qi = new int[total];
    int *qj = new int[total];
    int head = 0, tail = 0;

    vis[by][bx] = true; // plecam din baza pt a vedea daca exista drum spre S si D
    qi[tail] = by;
    qj[tail] = bx;
    tail++;

    int reached = 0;
    while (head < tail) // cat timp queue-ul exista
    {
        int ci = qi[head]; // luama prima valoare a unui vecin
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

    return reached == need; // daca am ajuns la toate S si D pe care le cautam true si harta e valida, daca nu refacem
}

void ProceduralMapGenerator::generate(int width, int height)
{
    srand((unsigned)time(0));

    char **mat = new char *[height];
    for (int i = 0; i < height; i++)
        mat[i] = new char[width];

    bool ok = false;
    int attempts = 0;

    while (!ok)
    {
        attempts++; // salvam si numarul de attemps pentru a testa daca merge si nu se blocheaza in while ( creare inf )

        const int D = this->destination;
        const int S = this->station;
        const int B = 1;
        int total = height * width;

        // am folosit Fisher–Yates shuffle
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
            bag[k++] = letters[rand() % 2];

        for (int i = total - 1; i > 0; i--)
        {
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
        delete[] bag;
    }

    ofstream out("mapSimulator.txt");
    if (out.is_open())
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
                out << mat[i][j];
            out << "\n";
        }
        out.close();
    }

    for (int i = 0; i < height; i++)
        delete[] mat[i];
    delete[] mat;
}

void ProceduralMapGenerator::load() {}

void FileMapLoader::load()
{
    ifstream in("mapSimulator.txt");
    if (!in.is_open())
        return;
    char line[2048];
    while (in.getline(line, 2048))
    {
        cout << line << "\n";
    }
    in.close();
}
void FileMapLoader::generate(int, int) {}

bool MapRuntime::inside(int x, int y) const
{
    if (x >= 0 && y >= 0 && x < width && y < height)
        return true;
    else
        return false;
}

char MapRuntime::get(int x, int y) const
{
    return mat[y][x];
}

bool MapRuntime::loadFromFile(const char *filename)
{
    ifstream f(filename);
    if (!f.is_open())
        return false;

    width = 0;
    height = 0;
    char line[2048];

    while (f.getline(line, 2048))
    {
        int len = (int)strlen(line);
        if (len > width)
            width = len;
        height++;
    }
    f.close();
    if (width <= 0 || height <= 0) 
        return false;

    mat = new char *[height];
    for (int i = 0; i < height; i++)
    {
        mat[i] = new char[width];
        for (int j = 0; j < width; j++)
            mat[i][j] = '.';
    }

    ifstream g(filename);
    if (!g.is_open())
        return false;

    int row = 0;
    while (g.getline(line, 2048) && row < height)
    {
        int len = (int)strlen(line);
        for (int col = 0; col < width; col++)
        {
            if (col < len)
                mat[row][col] = line[col];
            else
                mat[row][col] = '.';
        }
        row++;
    }
    g.close();
    return true;
}

bool MapRuntime::findFirst(char c, int &outX, int &outY) const
{
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            if (mat[y][x] == c)
            {
                outX = x;
                outY = y;
                return true;
            }
    return false;
}

MapRuntime::~MapRuntime()
{
    if (mat)
    {
        for (int i = 0; i < height; i++)
            delete[] mat[i];
        delete[] mat;
    }
}
