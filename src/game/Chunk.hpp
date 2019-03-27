#ifndef _CHUNK_HPP_
#define _CHUNK_HPP_

#include <vector>

//#include "World.hpp"
#include "Cube.hpp"

class Chunk {
private:
    int x, y, z;
    unsigned int VAO;
    Cube *cubes;
    std::vector<Cube*> renderedCubes;

public:
    static const int W=16, H=256, Z=16;

    Chunk ( int posX, int posY, int posZ );
    void genTerrain();
    Cube *getCube(int x, int y, int z);
    Cube *getCubes();
    bool isIllated(int x, int y, int z);
    void genVao();
    void getVisibleCubes();

    void draw();
};

#endif