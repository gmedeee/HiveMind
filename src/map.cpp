#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

class ProceduralMapGenerator
{
    int width;
    int height;

public:
    ProceduralMapGenerator(int width, int height)
    {
        srand((unsigned)time(nullptr)); 

        this->width = width;
        this->height = height;

        const int D = 10, S = 3, B = 1;
        int total = this->height * this->width;

        char *bag = new char[total];
        int k = 0;

        for (int i = 0; i < D; i++) 
            bag[k++] = 'D';
        for (int i = 0; i < S; i++) 
            bag[k++] = 'S';
        for (int i = 0; i < B; i++) 
            bag[k++] = 'B';

        const char letters[2] = {'.','#'};
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

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
                cout << mat[i][j];
            cout << '\n';
        }
    }
};

int main()
{
    ProceduralMapGenerator map(20, 20);
    return 0;
}
