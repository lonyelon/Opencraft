#include "WorldGenerator.hpp"

#include "World.hpp"
#include <game/Player.hpp>

extern float renderDistance;

void WorldGenerator::genChunk( std::vector<Chunk*> *chunks, int *chunkCount, int size, World *w, int threadNumber, int threadCount) {
	for ( int x = 0; x < size; x++ ) {
		for (int z = 0; z < size; z++) {
			for ( int y = threadNumber; y < size; y += threadCount ) {
				//c->push_back(new Chunk(x - size/2, 0, y - size/2));
				(*chunks)[x*size*size + z*size + y] = new Chunk( w, x - size/2, z - size/2, y - size/2 );
				(*chunks)[x*size*size + z*size + y]->genTerrain();
				(*chunkCount)++;
			}
		}
	}
}

void WorldGenerator::genVAOs( std::vector<Chunk*> *chunks, int threadNumber, int threadCount ) {
    for ( int i = threadNumber; i < (*chunks).size(); i += threadCount ) {
        (*chunks)[i]->getVisibleCubes();
        (*chunks)[i]->genVao();
    }
}

/*
	Updates the world. Gets executes in a separated thread.
*/
void WorldGenerator::worldUpdate(World *world, Player *player) {
    const int maxDist = round(renderDistance/16);

    while (world->isWorldUpdating()) {
        Cube *c = world->getCube(player->getCam()->getX(), player->getCam()->getY(), player->getCam()->getZ());
        if (c == NULL) continue;
        Chunk *ck = c->getChunk();

        for (int radius = 0; radius < maxDist; radius++) {
            for (int x = -radius; x < radius; x++ ) {
				for (int y = -radius; y < radius; y++) {
	                for (int z = -radius; z < radius; z++ ) {
	                    if (world->getChunk(ck->getX()+x, ck->getY()+y, ck->getZ()+z) == NULL) {
	                        world->genChunkAt(true, ck->getX()+x, ck->getY()+y, ck->getZ()+z);
	                    } else if (world->getChunk(ck->getX()+x, ck->getY()+y, ck->getZ()+z)->getVao() == 0) {
	                        Chunk *chunk = world->getChunk(ck->getX()+x, ck->getY()+y, ck->getZ()+z);
	                        chunk->getVisibleCubes();
	                        chunk->genVao();
	                        world->addChunkToQueue(chunk);
	                    }
					}
                }
            }
        }

		/*
			Delete far away chunks.
		*/
        std::vector<Chunk*> chunks = world->getChunks();
        for (int i = 0; i < chunks.size(); i++) {
            float dist = pow(chunks[i]->getX()-ck->getX(), 2);
            dist += pow(chunks[i]->getY()-ck->getY(), 2);
            dist += pow(chunks[i]->getZ()-ck->getZ(), 2);
            dist = sqrt(dist);

            if (dist >= (float)maxDist*1.25) {
                world->deleteChunk(chunks[i]);
            }
        }
    }
}
