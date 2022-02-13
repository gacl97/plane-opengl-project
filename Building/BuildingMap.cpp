#include "BuildingMap.h"

#include <GL/glu.h>
#include "../texture.h"

Building::Building(int i, int j, int h, Texture texture_1) {
  x = i;
  y = j;
  height = h;
  texture = texture_1;
}