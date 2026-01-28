#include <game/world/World.hpp>

#include <thread>
#include <filesystem>
#include <memory>
#include <iostream>

#include <game/world/WorldGenerator.hpp>

#include <game/Game.hpp>

extern std::unique_ptr<Game> game;

World::World(std::string name, int seed) {
    this->name = name;
    this->seed = seed;
    this->size = 15;
    this->chunkCount = 0;
    this->genThread = nullptr;
    this->updateWorld = false;

    this->terrainNoise.SetSeed(seed);
    this->terrainNoise.SetFrequency(1.5);
    this->terrainNoise.SetLacunarity(2.1f);

    this->caveNoise.SetSeed(3*seed);
    this->caveNoise.SetFrequency(1.0f);
    this->caveNoise.SetLacunarity(1.0f);

    this->sandNoise.SetSeed(2*seed);
    this->sandNoise.SetLacunarity(1);

    if (!std::filesystem::is_directory("saves/" + name) && !game->debug_mode)
        std::filesystem::create_directories("saves/" + name + "/world");
}

void World::setSize(const int size) {
    this->size = size;
}

/// Creates a chunk at the specified position. If draw == true the chunk is
/// drawn when generated.
void World::genChunkAt(bool draw, int x, int y, int z) {
    if (this->getChunk(x, y, z) != nullptr)
        return;

    Chunk *c = new Chunk(game->getWorld().get(), x, y, z);

    chunkMutex.lock();
    this->chunks.insert(std::make_pair(Position(x, y, z), c));
    this->chunkCount++;
    chunkMutex.unlock();

    WorldGenerator::genChunkTerrain(c);

    if (draw) {
        c->getVisibleCubes();
        this->addChunkToQueue(c);
    }
}

/*
	Adds a chunk to the drawing queue so it can wait to get drawn.
*/
void World::addChunkToQueue(Chunk *c) {
    drawMutex.lock();
    this->drawQueue.push_back(c);
    drawMutex.unlock();
}

/*
	Deletes a chunk from memory.
*/
void World::deleteChunk(Chunk *c) {
    Chunk *k = this->chunks[c->getPos()];
    chunkMutex.lock();
    if (k != nullptr) {
        delete (c);
        this->chunks.erase(k->getPos());
        this->chunkCount--;
    }
    chunkMutex.unlock();
}

void World::genChunks() {
    const int threadCount = 8;

    printf("Reserving memory for the world...\n");

    this->updateWorld = false;
    if (this->genThread != nullptr) this->genThread->join();
    for (auto mapPair: this->chunks)
        delete (mapPair.second);
    this->chunks.clear();
    this->chunkCount = 0;

    printf("Generating world...\n");

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    this->genChunkAt(false, 0, 0, 0);
    auto free_space_y = Position(0, 0, 0);
    for (int y = 0;;y++) {
        if (this->getCube(Position(0, y + 1, 0)) == nullptr)
            this->genChunkAt(false, 0, (y + 1)/Chunk::H, 0);
        if (this->getCube(Position(0, y, 0))->getType() == CubeType::air && this->getCube(Position(0, y + 1, 0))->getType() == CubeType::air) {
            free_space_y = Position(0, y + 1, 0);
            break;
        }
    }

    game->getPlayer()->getCamera()->setPos(0, free_space_y.y/Cube::size_reduction, 0);
    Position<int> playerChunkPosition(
        (int)free_space_y.x/Chunk::W,
        (int)free_space_y.y/Chunk::H,
        (int)free_space_y.z/Chunk::Z
    );

    std::array<std::thread, threadCount> t;
    for (int i = 0; i < threadCount; i++)
        t[i] = std::thread(WorldGenerator::genChunksAroundPoint,
                           &(this->chunks),
                           &(this->chunkCount),
                           this->size,
                           this,
                           playerChunkPosition,
                           i,
                           threadCount);

    for (auto& thread: t)
        thread.join();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    printf("Rendering chunks...\n");

    begin = std::chrono::steady_clock::now();

    for (int i = 0; i < threadCount; i++)
        t[i] = std::thread(WorldGenerator::genVAOs, &(this->chunks), i, threadCount);

    for (int i = 0; i < threadCount; i++) {
        printf("Rendered chunk %d/%d\n", i + 1, threadCount);
        t[i].join();
    }

    end = std::chrono::steady_clock::now();

    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    this->updateWorld = true;
    this->genThread = std::make_unique<std::thread>(WorldGenerator::worldUpdate, game->getWorld(), game->getPlayer());

    printf("Complete!\n");
}

void World::draw() {
    chunkMutex.lock();
    for (auto c: this->chunks) {
        if (c.second != nullptr) { // TODO fix whatever is causing this

            if (c.first != c.second->getPos()) { // TODO fix whatever is also causing this
                //std::cout << "What!" << std::endl;
                continue;
            }

            c.second->draw();
        }
    }
    chunkMutex.unlock();
}

/*
	Returns a cube by it's coordinates.
*/
std::shared_ptr<Cube> World::getCube(int x, int y, int z) {
    return this->getCube(Position<int>(x, y, z));
}

std::shared_ptr<Cube> World::getCube(Position<int> pos) {
    Chunk *c = nullptr;

    int chunkX = floor((float) pos.x / Chunk::W);
    int chunkY = floor((float) pos.y / Chunk::H);
    int chunkZ = floor((float) pos.z / Chunk::Z);

    c = this->getChunk(chunkX, chunkY, chunkZ);

    if (c == nullptr)
        return nullptr;

    auto newPos = Position(pos.x - c->getX() * Chunk::W, pos.y - c->getY() * Chunk::H,
                           pos.z - c->getZ() * Chunk::Z);

    return c->getCube(newPos);
}

/*
	Returns a cube by it's coordinates, but first it checks it's own chunk.
*/
std::shared_ptr<Cube> World::getCube(Chunk *k, int x, int y, int z) {
    return this->getCube(k, Position(x, y, z));
}

std::shared_ptr<Cube> World::getCube(Chunk *k, Position<int> pos) {
    int chunkX = floor((float) pos.x / Chunk::W);
    int chunkY = floor((float) pos.y / Chunk::H);
    int chunkZ = floor((float) pos.z / Chunk::Z);

    if (k != nullptr && k->getX() == chunkX && k->getY() == chunkY && k->getZ() == chunkZ) {
        auto cubePos = Position(pos.x - k->getX() * Chunk::W, pos.y - k->getY() * Chunk::H,
                                pos.z - k->getZ() * Chunk::Z);
        std::shared_ptr<Cube> cube = k->getCube(cubePos);
        return cube;
    }

    return this->getCube(pos);
}

void World::setSeed(int seed) {
    this->seed = seed;
}

int World::getSeed() const {
    return this->seed;
}

int World::getChunkCount() const {
    return this->chunkCount;
}

std::map<Position<int>, Chunk *> World::getChunks() const {
    return this->chunks;
}

/*
	Get's a chunk by it's coordinates.
*/
Chunk *World::getChunk(int x, int y, int z) {
    try {
        return this->chunks.at(Position(x, y, z));
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

void World::saveWorld() {
    if (game->debug_mode)
        return;
    
    std::ofstream file("saves/" + this->name + "/playerData.txt");

    file << game->getPlayer()->getCam()->getX() << "\t" << game->getPlayer()->getCam()->getY() << "\t"
         << game->getPlayer()->getCam()->getZ();

    for (auto c: this->chunks)
        c.second->save();

    file.close();
}

bool World::isWorldUpdating() const {
    return this->updateWorld;
};

std::string World::getName() {
    return this->name;
};

void World::setCube(std::shared_ptr<Cube> c, Position<int> pos) {
    int x = floor((float) pos.x / Chunk::W);
    int y = floor((float) pos.y / Chunk::H);
    int z = floor((float) pos.z / Chunk::Z);

    Chunk *chunk = this->getChunk(x, y, z);
    if (chunk != nullptr) {
        Position<int> newPos = pos.move(Position(-1 * x * Chunk::W, -1 * y * Chunk::H, -1 * z * Chunk::Z));
        chunk->setCube(c, newPos);
    }
};

World::~World() {
    if (this->genThread != nullptr) {
        this->updateWorld = false;
        this->genThread->join();
    }

    this->saveWorld();
    this->chunks.clear();
}
