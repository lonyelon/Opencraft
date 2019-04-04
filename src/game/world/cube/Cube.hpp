#ifndef _CUBE_HPP_
#define _CUBE_HPP_

#include <vector>

#include "Cube.hpp"
#include "../CubeTypes.hpp"
#include "../../../engine/glfw.hpp"

class Chunk;

class Cube {
private:
    Chunk *chunk;
    CubeType type;
    int x, y, z;

public:
    Cube (); 
    Cube ( Chunk *c, int xpos, int ypos, int zpos );

    void setPosition( int x, int y, int z );
    void setType( CubeType t );
    CubeType getType();

    void getVertex( std::vector<float> *v, std::vector<int> *i, int n );

    int getX() { return this->x; };
    int getY() { return this->y; };
    int getZ() { return this->z; };

    ~Cube();
};

#endif