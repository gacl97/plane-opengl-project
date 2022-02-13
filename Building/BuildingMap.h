#ifndef PLANE_PROJECT_BUILDING_H
#define PLANE_PROJECT_BUILDING_H

#include "../texture.h"

class Building {
  public:
    int x;
    int y;
    int height;
    Texture texture;
    Building(int i, int j, int h, Texture texture_1);
};

#endif