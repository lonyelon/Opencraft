#ifndef _GAME_WORLD_HPP_
#define _GAME_WORLD_HPP_

#include <vector>

#include "Chunk.hpp"

class World {
private:
    int seed;
    std::vector<Chunk> chunks;
    std::vector<Cube*> cubesToRender;
public:
    World();
    Chunk *getChunk(int x, int y, int z);
    void draw();
};

#endif
