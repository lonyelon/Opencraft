#ifndef GAME_WORLD_HPP
#define GAME_WORLD_HPP

#include <vector>

#include "Chunk.hpp"

class World {
private:
    int seed;
    int size;
    int chunkCount;

    std::vector<Chunk*> chunks;
    std::vector<Cube*> cubesToRender;
public:
    World(  );

    void genChunks(  );
    void setSize(const int size );
    Cube *getCube( int x, int y, int z );
    Cube *getCube( float x, float y, float z ) { return this->getCube((int)round(x), (int)round(y), (int)round(z)); };
    Cube *getCube( double x, double y, double z ) { return this->getCube((int)round(x), (int)round(y), (int)round(z)); };
    Cube *getCube( Chunk *c, int x, int y, int z );
    void setSeed( int seed );
    int getSeed(  );
    int getChunkCount(  );
    std::vector<Chunk*> getChunks();

    void draw(  );
};

#endif
