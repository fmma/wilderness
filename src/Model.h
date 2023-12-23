#ifndef __MODEL_HEADER__
#define __MODEL_HEADER__

#include "View.h"
#include "linalg.h"

struct Player {
    char name[15];
    float x; 
    float y;
    float lookX;
    float lookY;
    float lookZ;
};

struct Cell {
    float height;
    bool tree;
};

enum GameMode {
    FirstPerson,
    Strategic
};

struct Model {
    int width = 200;
    int height = 200;
    Player player = Player{"Bob Herring", width / 2.0f, height / 2.0f, 0.0f, 1.0f, 0.0f};
    int difficulty = 1;
    int seed;
    float heightMin = -100.0f;
    float heightMax = 150.0f;
    float waterHeight = 0.0f;
    float treeLimit = 50.0f;
    float treeProp = 0.33f;
    float *heightmap;
    float *watermap;
    float *suspendedSediment;
    float *fluxUp;
    float *fluxDown;
    float *fluxLeft;
    float *fluxRight;
    vec2 *velocity;
    bool *trees;
    int viewX, viewY;
    GameMode currentGameMode = Strategic;

    void addHeight(int x, int y, float f) {
        if(x >= 0 && x < width && y >= 0 && y < height) {
            heightmap[x + y * width] += f;
        }
    }
    void setHeight(int x, int y, float f) {
        if(x >= 0 && x < width && y >= 0 && y < height) {
            heightmap[x + y * width] = f;
        }
    }

    void generateMap();
    void renderMap(View *view, float slerp);
    void smoothHeights(int iterations, float *stencil, int stencilSize);
};

#endif