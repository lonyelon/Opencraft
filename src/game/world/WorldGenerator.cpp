#include "WorldGenerator.hpp"

#include <iostream>

#include <game/world/World.hpp>
#include <game/Player.hpp>
#include <game/Game.hpp>

#include <tuple>

extern std::unique_ptr<Game> game;

void WorldGenerator::genChunkTerrain(Chunk* chunk) {
    const float xCoordRed = 750*Cube::size_reduction;
    const float yCoordRed = 750*Cube::size_reduction;
    const float zCoordRed = 750*Cube::size_reduction;
    const float ystrech = 50*Cube::size_reduction;
    const int heightIncrease = -60*Cube::size_reduction;

    const int waterHeight = 40*Cube::size_reduction + heightIncrease;

    Position<int> chunkPosition = chunk->position;

    chunk->load();
    if (chunk->generated)
        return;

    // Precalculate terrain height.
    int heights[Chunk::W * Chunk::Z];
    for (int x = 0; x < Chunk::W; x++) {
        float cX = static_cast<float>(chunkPosition.x * Chunk::W + x) / xCoordRed;
        for (int z = 0; z < Chunk::Z; z++) {
            float cZ = static_cast<float>(chunkPosition.z * Chunk::Z + z) / zCoordRed;
            heights[x + z*Chunk::W] = (int) ((game->getWorld()->terrainNoise.GetValue(cX, 0, cZ) + 1.1) * ystrech) + heightIncrease;
        }
    }

    // If the height is lower than the chunk size just fill it with Air and skip generation.
    bool skip = true;
    for (int i = 0; i < Chunk::W*Chunk::Z; i++) {
        if (heights[i] < chunkPosition.y) {
            skip = false;
            break;
        }
    }
    if (skip) {
        for (int x = 0; x < Chunk::W; x++)
            for (int z = 0; z < Chunk::Z; z++)
                for (int y = 0; y < Chunk::H; y++)
                    chunk->setCube(std::make_shared<Air>(), Position(x, y, z), false);
        return;
    }

    // Generate stone world and water.
    for (int x = 0; x < Chunk::W; x++) {
        float cX = static_cast<float>(chunkPosition.x * Chunk::W + x) / xCoordRed;
        for (int z = 0; z < Chunk::Z; z++) {
            float cZ = static_cast<float>(chunkPosition.z * Chunk::Z + z) / zCoordRed;
            int noiseValue = heights[x + z * Chunk::Z];
            for (int y = 0; y < Chunk::H; y++) {
                float cY = static_cast<float>(chunkPosition.y * Chunk::H + y) / yCoordRed;

                float caveProb = -(y + cY*Chunk::H + 50)/500-0.75;
                if (caveProb > 0.0)
                    caveProb = 0.0;
                if (caveProb < -1.0)
                    caveProb = -1.0;

                float noiseY = (float) (chunkPosition.y * Chunk::H + y) / yCoordRed * 10;

                float caveHeightRedux = y + chunkPosition.y * Chunk::H;
                if (caveHeightRedux < 1)
                    caveHeightRedux = 1;

                if ((game->getWorld()->caveNoise.GetValue(cX, cY, cZ) < caveProb))
                    chunk->setCube(std::make_shared<Air>(), Position(x, y, z), false);
                else {
                    if (y + chunkPosition.y * Chunk::H < noiseValue)
                        chunk->setCube(std::make_shared<Stone>(), Position(x, y, z), false);
                    else
                        chunk->setCube(std::make_shared<Air>(), Position(x, y, z), false);
                }
            }
        }
    }

    // Water and Dirt.
    for (int x = 0; x < Chunk::W; x++) {
        float noiseX = (float) (chunkPosition.x * Chunk::W + x) / xCoordRed * 10;
        for (int z = 0; z < Chunk::Z; z++) {
            float noiseZ = (float) (chunkPosition.z * Chunk::Z + z) / zCoordRed * 10;
            int dirtCount = 0;
            for (int y = Chunk::H - 1; y >= 0; y--) {
                auto cubePos = Position(x, y, z);
                std::shared_ptr<Cube> c = chunk->getCube(cubePos);

                float noiseY = (float) (chunkPosition.y * Chunk::H + z) / yCoordRed * 10;

                if (c->getType() == CubeType::air) {
                    if (y + chunkPosition.y * Chunk::H < waterHeight)
                        chunk->setCube(std::make_shared<Water>(), cubePos, false);
                    else
                        dirtCount = 0;
                } else {
                    if (dirtCount < 8*Cube::size_reduction) {
                        if (y + chunkPosition.y * Chunk::H < heights[x + z * Chunk::W] ||
                            heights[x + z * Chunk::W] == -1) {
                            heights[x + z * Chunk::W] = y + chunkPosition.y * Chunk::H;
                        }
                        if (Cube::size_reduction*(game->getWorld()->sandNoise.GetValue(noiseX, noiseY, noiseZ) * 5) / (waterHeight - y - chunkPosition.y * Chunk::H) > 1)
                            chunk->setCube(std::make_shared<Sand>(), cubePos, false);
                        else if (dirtCount == 0 && y + chunkPosition.y * Chunk::H >= waterHeight - 1)
                            chunk->setCube(std::make_shared<GrassyDirt>(), cubePos, false);
                        else
                            chunk->setCube(std::make_shared<Dirt>(), cubePos, false);
                        dirtCount++;
                    }
                }
            }
        }
    }

    // Magma.
    // Disabled since the world is now of infinite depth, I have to find a cool
    // way to re-implement this.
    /*for (int x = 0; x < Chunk::W; x++) {
        for (int z = 0; z < chunkPosition.z; z++) {
            int dirtCount = 0;
            for (int y = Chunk::H-1; y > 0; y--) {
                std::shared_ptr<Cube> c = chunk->getCube( x, y, z );

                if ( y < 12 && c->getType() == CubeType::air )
                    chunk->setCube(new Lava(), x, y, z);
            }
        }
    }*/

    chunk->generated = true;

    chunk->save();
}

void WorldGenerator::genChunksAroundPoint(std::map<Position<int>, Chunk *>* chunks,
                                          int* chunkCount,
                                          int size,
                                          World* w,
                                          Position<int> origin,
                                          int threadNumber,
                                          int threadCount) {
    for (int x = -size/2; x < size/2; x++) {
        for (int z = -size/2; z < size/2; z++) {
            for (int y = threadNumber - size/2; y < size/2; y += threadCount) {
                Position p = Position<int>(x, z, y) + origin;
                auto chunk = new Chunk(game->getWorld().get(), p);
                game->getWorld()->chunkMutex.lock();
                (*chunks)[p] = chunk;
                (*chunkCount)++;
                game->getWorld()->chunkMutex.unlock();
                WorldGenerator::genChunkTerrain((*chunks).at(p));
            }
        }
    }
}

void WorldGenerator::genVAOs(std::map<Position<int>, Chunk *> *chunks, int threadNumber, int threadCount) {
    auto c = std::next(chunks->begin(), threadNumber);
    std::size_t a = threadNumber;
    auto max = chunks->size();
    for (; a < max; std::advance(c, threadCount), a += threadCount) {
        c->second->getVisibleCubes();
        c->second->genVao();
    }
}


void _genChunkOrItsVAO(Chunk *ck, int x, int y, int z, std::shared_ptr<World> world) {
    Chunk *chunk = world->getChunk(ck->getX() + x, ck->getY() + y, ck->getZ() + z);
    if (chunk == nullptr) {
        world->genChunkAt(true, ck->getX() + x, ck->getY() + y, ck->getZ() + z);
    } else if (!chunk->isGenerated() || !chunk->isUpdated() || !chunk->isDrawn()) {
        chunk->getVisibleCubes();
        chunk->genVao();
        world->addChunkToQueue(chunk);
    }
};

/// Updates the world. Gets executed in a separated thread.
void WorldGenerator::worldUpdate(std::shared_ptr<World> world, std::shared_ptr<Player> player) {
    const int maxDist = round(game->renderDistance / 16);

    while (world->isWorldUpdating()) {
        std::shared_ptr<Cube> c = world->get_cube(Position(player->get_camera()->x,
                                                           player->get_camera()->y,
                                                           player->get_camera()->z));
        if (c == nullptr)
            continue;
        Chunk *ck = c->getChunk();

        // TODO what the fuck.
        if (ck == nullptr)
            continue;

        c = world->get_cube(Position(player->get_camera()->x,
                                     player->get_camera()->y,
                                     player->get_camera()->z));

        std::vector<std::tuple<int, int, int>> data;
        for (int radius = 0; radius < maxDist; radius++)
            for (int x = -radius; x < radius; x++)
                for (int y = -radius; y < radius; y++)
                    for (int z = -radius; z < radius; z++)
                        data.push_back({x, y, z});

        for (std::size_t i = 0; i < data.size(); i += 8) {
            std::shared_ptr<Cube> cubeBelowPlayer = world->get_cube(Position(player->get_camera()->x,
                                                                             player->get_camera()->y,
                                                                             player->get_camera()->z));
            Chunk *chunkBelowPlayer = cubeBelowPlayer->getChunk();

            // Chek if player has moved to a new chunk.
            if (chunkBelowPlayer->getX() != ck->getX() || chunkBelowPlayer->getY() != ck->getY() || chunkBelowPlayer->getZ() != ck->getZ()){
                break;
            }

            std::vector<std::thread> t;
            for (std::size_t k = 0; k < 16 && k + i < data.size(); k++)
                t.push_back(std::thread(_genChunkOrItsVAO, 
                                        ck,
                                        std::get<0>(data[i + k]),
                                        std::get<1>(data[i + k]),
                                        std::get<2>(data[i + k]),
                                        world));

            for (auto& thread: t)
                thread.join();
        }

        //Delete far away chunks.
        auto chunks = world->getChunks();
        std::vector<Position<int>> chunksToDelete;
        for (auto c: chunks) {
            // TODO fix whatever is causing this
            if (c.second == nullptr)
                break;

            float dist = pow(c.second->getX() - ck->getX(), 2);
            dist += pow(c.second->getY() - ck->getY(), 2);
            dist += pow(c.second->getZ() - ck->getZ(), 2);
            dist = sqrt(dist);

            if (dist >= (float) maxDist * 1.25)
                chunksToDelete.push_back(c.first);
        }
        for (auto c: chunksToDelete)
            chunks.erase(c);
    }
}
