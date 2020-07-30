#ifndef _CHUNK_HPP_
#define _CHUNK_HPP_

#include <vector>

//#include "World.hpp"
#include "Chunk.hpp"
#include "cube/Cube.hpp"
#include "cube/Dirt.hpp"

class World;

class Chunk {
private:
    int x, y, z;
    unsigned int VAO;
    std::vector<Cube*> cubes;
    std::vector<Cube*> renderedCubes;
    bool generated;
    World *world;

public:
	// TODO Change Y to 16
    static const int W=16, H=256, Z=16;

    Chunk ( World *w, int posX, int posY, int posZ );

    void genTerrain();

	Cube *getCube(unsigned int x, int y, int z);
	void setCube(Cube *c, int x, int y, int z);

    std::vector<Cube*> getCubes();
    int isIllated(int x, int y, int z);
    void genVao();
    void getVisibleCubes();

    int getCubeCount() { return this->renderedCubes.size(); };

    int getX() { return this->x; };
    int getY() { return this->y; };
    int getZ() { return this->z; };
    int getVao() { return this->VAO; };

    void draw();

    ~Chunk();
};

#endif
