#ifndef _CHUNK_HPP_
#define _CHUNK_HPP_

#include <vector>

//#include "World.hpp"
#include "Chunk.hpp"
#include "cube/Cube.hpp"

class World;

class Chunk {
private:
    int x, y, z;
    unsigned int VAO;
    std::vector<Cube*> cubes;
    std::vector<Cube*> renderedCubes;
    World *world;

public:
    static const int W=16, H=256, Z=16;

    Chunk ( World *w, int posX, int posY, int posZ );
    void genTerrain();
    Cube *getCube(int x, int y, int z);
    std::vector<Cube*> getCubes();
    bool isIllated(int x, int y, int z);
    void genVao();
    void getVisibleCubes();

    int getX() { return this->x; };
    int getY() { return this->y; };
    int getZ() { return this->z; };

    void draw();

    ~Chunk();
};

#endif