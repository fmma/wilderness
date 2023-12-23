#include "Model.h"

#include <stdio.h>
#include <Math.h>
#include <cstdlib>

inline float randf() {
    return (float)rand()/(float)(RAND_MAX);
}

float normalf() {
    float x = 0.0f;
    for(int i = 0; i < 12; ++i) {
        x += randf();
    }
    return x - 6.0f;
}

void stencil() {
    
}

void Model::generateMap() {
    this->heightmap = new float[width * height];
    this->trees = new bool[width * height];
    srand(this->seed);
    
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            heightmap[x + y*width] = 0.9f * (waterHeight - heightMin) / (heightMax - heightMin) + (randf() - 0.5f) / 4.0f;
        }
    }
    for(int i = 0; i < 50; ++i) {
        int x = floorf(randf() * width);
        int y = floorf(randf() * height);
        heightmap[x + y*width] = 1.0f;
        for(int j = 0; j < 100; ++j) {
            int dx = floorf(normalf() * 10) - 5;
            int dy = floorf(normalf() * 10) - 5;
            int x0 = x + dx;
            int y0 = y + dy;
            setHeight(x0, y0, 1.0f);
        }
    }

    float stencil[] = 
        { 1.0f, 2.0f, 1.0f,
          2.0f, 4.0f, 2.0f,
          1.0f, 2.0f, 1.0f
        };
    smoothHeights(3, stencil, 3);

    // Rescale
    /*
    float minH = this->heightMax;
    float maxH = this->heightMin;
    
    for(int y = 1; y < height - 1; ++y) {
        for(int x = 1; x < width - 1; ++x) {
            minH = minH < map[x + y * width].height ? minH : map[x + y * width].height;
            maxH = maxH > map[x + y * width].height ? maxH : map[x + y * width].height;
        }
    }
    float range = maxH - minH;
    */
    for(int i = 0; i < this->width * this->height; ++i) {
        heightmap[i] = heightmap[i] * (this->heightMax - this->heightMin) + this->heightMin;
    }    

/*
    for(int i = 0; i < this->width * this->height; ++i) {
        if(this->map[i].height > this->waterHeight && this->map[i].height <= this->treeLimit) {
            this->map[i].tree = randf() <= this->treeProp ? 1 : 0;
        }
    }
    */

    /*
    //Random river
    int x = 100;
    int y = 0;
    int dx = 0;
    int dy = 1;
    int i = 0;
    for(i = 0; i < 1000 && x < this->width && y < this->height && x >= 0 && y >= 0; ++i) {
        heightmap[x + this->width * y] = 99.0f;
        x += dx;
        y += dy;
    }
    */
}


void Model::smoothHeights(int iterations, float *stencil, int stencilSize) {
    int center = stencilSize / 2;
    for(int k = 0; k < iterations; ++k) {
        float newHeights[width * height];
        for(int j = 0; j < height; ++j) {
            for(int i = 0; i < width; ++i) {
                float x = 0.0f;
                float w = 0.0f;
                for(int jj = 0; jj < stencilSize; ++jj) {
                    for(int ii = 0; ii < stencilSize; ++ii) {
                        int iii = i + ii - center;
                        int  jjj = j + jj - center;
                        if(iii >= 0 && iii < width && jjj >= 0 && jjj < height) {
                            float wij = stencil[ii + jj*stencilSize];
                            w += wij;
                            x += wij * heightmap[iii + (jjj) * width];
                        }
                    }
                }
                newHeights[i + j * width] = x / w;
            }
        }
        for(int i = 0; i < width * height; ++i) {
            heightmap[i] = newHeights[i];
        }
    }
}

void Model::renderMap(View *view, float slerp) {
    int tick = ((int)slerp) & 1;
    char shades[] = "  " // "~~"
    /*
  0000    1100    0011    1111    0000    1100    0011    1111    0000    1100    0011    1111    0000    1100    0011    1111  
 001100  001100  001100  001100  001100  001100  001100  001100  001100  001100  001100  001100  001100  001100  001100  001100 
  0000    0000    0000    0000    1100    1100    1100    1100    0011    0011    0011    0011    1111    1111    1111    1111  */
  "=="    "))"    "(("    "\\/"   "))"    "))"    "//"    ")/"    "(("    "\\\\"  "(("    "\\("   "/\\"   ")\\"   "/("    ")("  
/*
  0000    1100    0011    1111    0000    1100    0011    1111    0000    1100    0011    1111    0000    1100    0011    1111  
 111100  111100  111100  111100  111100  111100  111100  111100  111100  111100  111100  111100  111100  111100  111100  111100 
  0000    0000    0000    0000    1100    1100    1100    1100    0011    0011    0011    0011    1111    1111    1111    1111  */
  "=="    "_)"    "=/"    "_/"    "`)"    "~)"    "`/"    "~/"    "=\\"   "_\\"   "=("    "_\\"   "`\\"   "~\\"   "`/"    "~("  
/*
  0000    1100    0011    1111    0000    1100    0011    1111    0000    1100    0011    1111    0000    1100    0011    1111  
 001111  001111  001111  001111  001111  001111  001111  001111  001111  001111  001111  001111  001111  001111  001111  001111 
  0000    0000    0000    0000    1100    1100    1100    1100    0011    0011    0011    0011    1111    1111    1111    1111  */
  "=="    "\\="   "(_"    "\\_"   "/="    ")="    "/_"    "/_"    "(`"    "\\`"   "(~"    "\\~"   "/`"    "\\`"   "/~"    ")~"  
/*
  0000    1100    0011    1111    0000    1100    0011    1111    0000    1100    0011    1111    0000    1100    0011    1111  
 111111  111111  111111  111111  111111  111111  111111  111111  111111  111111  111111  111111  111111  111111  111111  111111 
  0000    0000    0000    0000    1100    1100    1100    1100    0011    0011    0011    0011    1111    1111    1111    1111  */
  "=="    "_="    "=_"    "__"    "`="    "~="    "`_"    "~_"    "=`"    "_`"    "=~"    "_~"    "``"    "~`"    "`~"    "~~"  
;

/*
 ab
c d
 ef

*/
    int nShades = (sizeof(shades) / sizeof(char) - 1) / 2;
    int x = viewX; // roundf(player.x) - view->width / 2;
    int y = viewY; // roundf(player.y) - view->height / 2;
    for(int r = 1; r < view->height - 1; ++r) {
        for(int c = 0; c < view->width - 2; c += 2) {
            int ry = r + y;
            int cx = c / 2 + x;
            int odd = ry&1;
            if(odd && c >= view->width - 3) {
                break;
            }
            if(ry >= 0 && cx >= 0 && ry < this->width && cx < this->height) {
                int i = cx + ry * width;
                int shadeIndex = heightmap[i] <= waterHeight
                ? 0
                : heightmap[i] <= treeLimit
                ? 1
                : 2;

                shadeIndex = 
                    heightmap[i] < waterHeight ? 0 : 
                    1 +
                    ((heightmap[i - (!odd) - width] >= waterHeight) << 0) +
                    ((heightmap[i + odd    - width] >= waterHeight) << 1) +
                    ((heightmap[i - (!odd) + width] >= waterHeight) << 2) +
                    ((heightmap[i + odd    + width] >= waterHeight) << 3) +
                    ((heightmap[i - 1             ] >= waterHeight) << 4) +
                    ((heightmap[i + 1             ] >= waterHeight) << 5);

                // int shadeIndex = (int)((cell.height - heightMin) / (heightMax - heightMin) * (nShades - 1));
                if(shadeIndex > nShades || shadeIndex < 0)
                {
                    printf("Error shade index: %d, height:%f\n", shadeIndex, heightmap[i]);
                    exit(1);
                }
                
                if(c + odd > 0) {
                    char shade = shades[2*shadeIndex];
                    view->writeChar(c + odd, r, !tick && shade == '~' ? ' ' : shade);
                }
                if(c + 1 + odd < view->width - 2) {
                    char shade = shades[2*shadeIndex+1];
                    view->writeChar(c + 1 + odd, r, tick && shade == '~' ? ' ' : shade);
                }
                //view->writeChar(0,r, odd ? '1' : '0');
            }
            else {
                view->writeChar(c + odd, r, ' ');
                view->writeChar(c+1 + odd, r, ' ');
            }
        }
    }
    int playerX = ((int)player.x - x) * 2;
    int playerY = (int)player.y - y;
    view->writeChar(playerX, playerY, '&');
}
