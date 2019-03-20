#ifndef _CUBE_HPP_
#define _CUBE_HPP_

#include "CubeTypes.hpp"
#include "../engine/glfw.hpp"

class Cube {
private:
    CubeType type;
    int x, y, z;
    void *m;

    void recalcModelMatrix();

public:
    Cube (); 
    Cube ( int xpos, int ypos, int zpos );

    void setPosition(int x, int y, int z);
    void setType(CubeType t);
    CubeType getType();

    int getX() { return this->x; };
    int getY() { return this->y; };
    int getZ() { return this->z; };

    int draw();
};

#endif