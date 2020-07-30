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

void genChunk(std::vector<Chunk*> *chunks, int *chunkCount, int size, World *w, int threadNumber, int threadCount);
void genVAOs(std::vector<Chunk*> *chunks, int threadNumber, int threadCount);
void worldUpdate(World *world, Player *player);

#endif
