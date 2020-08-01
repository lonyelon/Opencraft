/*
	This methods get called by the worldgen thread.
*/

#ifndef GAME_WORLDGENERATOR_HPP
#define GAME_WORLDGENERATOR_HPP

#include <vector>

#include "Chunk.hpp"
#include "cube/Cubes.hpp"

class World;
class Player;

class WorldGenerator {
public:
    static void genChunk(std::vector<Chunk *> *chunks, int *chunkCount, int size, World *w, int threadNumber, int threadCount);

    static void genVAOs(std::vector<Chunk *> *chunks, int threadNumber, int threadCount);

    static void worldUpdate(std::shared_ptr<World> world, std::shared_ptr<Player> player);
};

#endif
