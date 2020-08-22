#include "WorldGenerator.hpp"

#include "World.hpp"
#include <game/Player.hpp>

#include <game/Game.hpp>

extern std::unique_ptr<Game> game;

void WorldGenerator::genChunk(std::map<Position<int>, Chunk *> *chunks, int *chunkCount, int size, World *w,
                              int threadNumber, int threadCount) {
    for (int x = 0; x < size; x++) {
        for (int z = 0; z < size; z++) {
            for (int y = threadNumber; y < size; y += threadCount) {
                Position p(x - size / 2, z - size / 2, y - size / 2);
                (*chunks)[p] = new Chunk(game->getWorld(), x - size / 2, z - size / 2,
                                         y - size / 2);
                (*chunks).at(p)->genTerrain();
                (*chunkCount)++;
            }
        }
    }
}

void WorldGenerator::genVAOs(std::map<Position<int>, Chunk *> *chunks, int threadNumber, int threadCount) {
    for (auto c: *chunks) {
        c.second->getVisibleCubes();
        c.second->genVao();
    }
}

/*
	Updates the world. Gets executes in a separated thread.
*/
void WorldGenerator::worldUpdate(std::shared_ptr<World> world, std::shared_ptr<Player> player) {
    const int maxDist = round(game->renderDistance / 16);

    while (world->isWorldUpdating()) {

        std::shared_ptr<Cube> c = world->getCube(player->getCam()->getX(), player->getCam()->getY(),
                                                 player->getCam()->getZ());
        if (c == nullptr) continue;
        Chunk *ck = c->getChunk();

        for (int radius = 0; radius < maxDist; radius++) {
            for (int x = -radius; x < radius; x++) {
                for (int y = -radius; y < radius; y++) {
                    for (int z = -radius; z < radius; z++) {
                        if (world->getChunk(ck->getX() + x, ck->getY() + y, ck->getZ() + z) == nullptr) {
                            world->genChunkAt(true, ck->getX() + x, ck->getY() + y, ck->getZ() + z);
                        } else if (world->getChunk(ck->getX() + x, ck->getY() + y, ck->getZ() + z)->isGenerated() &&
                                   world->getChunk(ck->getX() + x, ck->getY() + y, ck->getZ() + z)->isDrawn()) {
                            Chunk *chunk = world->getChunk(ck->getX() + x, ck->getY() + y, ck->getZ() + z);
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
        auto chunks = world->getChunks();
        for (auto c: chunks) {
            float dist = pow(c.second->getX() - ck->getX(), 2);
            dist += pow(c.second->getY() - ck->getY(), 2);
            dist += pow(c.second->getZ() - ck->getZ(), 2);
            dist = sqrt(dist);

            if (dist >= (float) maxDist * 1.25) {
                world->deleteChunk(c.second);
            }
        }
    }
}
