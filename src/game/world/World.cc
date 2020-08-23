#include <game/world/World.hpp>

#include <thread>
#include <filesystem>
#include <memory>

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

    if (!std::filesystem::is_directory("saves/" + name)) {
        std::filesystem::create_directories("saves/" + name + "/world");
    }
}

void World::setSize(const int size) {
    this->size = size;
}

/*
	Creates a chunk at the specified position. If draw == true the chunk is
	drawn when generated.
*/
void World::genChunkAt(bool draw, int x, int y, int z) {
    if (this->getChunk(x, y, z) != nullptr) {
        return;
    }

    Chunk *c = new Chunk(game->getWorld(), x, y, z);

    chunkMutex.lock();
    this->chunks.insert(std::make_pair(Position(x, y, z), c));
    this->chunkCount++;
    chunkMutex.unlock();

    c->genTerrain();

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
    for (auto mapPair: this->chunks) {
        delete (mapPair.second);
    }
    this->chunks.clear();
    this->chunkCount = 0;

    printf("Generating world...\n");

    std::array<std::thread, threadCount> t;
    for (int i = 0; i < threadCount; i++) {
        t[i] = std::thread(WorldGenerator::genChunk, &(this->chunks), &(this->chunkCount), this->size, this, i,
                           threadCount);
    }

    for (int i = 0; i < threadCount; i++) {
        t[i].join();
    }

    printf("Rendering chunks...\n");

    /*for (int i = 0; i < threadCount; i++) {
		t[i] = boost::thread( genVAOs, &(this->chunks), i, threadCount );
	}

	for (int i = 0; i < threadCount; i++) {
		t[i].join();
    }*/

    for (int i = 0; i < threadCount; i++) {
        t[i] = std::thread(WorldGenerator::genVAOs, &(this->chunks), i, threadCount);
    }

    for (int i = 0; i < threadCount; i++) {
        printf("%d/%d\n", i, threadCount);
        t[i].join();
    }

    /*for (auto c: this->chunks) {
        printf("%d/%d: %p\n", i, size, c.second);
        //c.second->getVisibleCubes();
        //this->chunks[i]->genVao();
        if (++i == size) {
            break;
        }
    }*/

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

    int chunkX = floor((float) pos.getX() / Chunk::W);
    int chunkY = floor((float) pos.getY() / Chunk::H);
    int chunkZ = floor((float) pos.getZ() / Chunk::Z);

    c = this->getChunk(chunkX, chunkY, chunkZ);

    if (c == nullptr) {
        return nullptr;
    }

    auto newPos = Position(pos.getX() - c->getX() * Chunk::W, pos.getY() - c->getY() * Chunk::H,
                           pos.getZ() - c->getZ() * Chunk::Z);

    return c->getCube(newPos);
}

/*
	Returns a cube by it's coordinates, but first it checks it's own chunk.
*/
std::shared_ptr<Cube> World::getCube(Chunk *k, int x, int y, int z) {
    return this->getCube(k, Position(x, y, z));
}

std::shared_ptr<Cube> World::getCube(Chunk *k, Position<int> pos) {
    int chunkX = floor((float) pos.getX() / 16);
    int chunkY = floor((float) pos.getY() / 16);
    int chunkZ = floor((float) pos.getZ() / 16);

    if (k != nullptr && k->getX() == chunkX && k->getY() == chunkY && k->getZ() == chunkZ) {
        auto cubePos = Position(pos.getX() - k->getX() * Chunk::W, pos.getY() - k->getY() * Chunk::H,
                                pos.getZ() - k->getZ() * Chunk::Z);
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
    std::ofstream file("saves/" + this->name + "/playerdata.txt");

    file << game->getPlayer()->getCam()->getX() << "\t" << game->getPlayer()->getCam()->getY() << "\t"
         << game->getPlayer()->getCam()->getZ();

    for (auto c: this->chunks) {
        c.second->save();
    }

    file.close();
}

bool World::isWorldUpdating() const {
    return this->updateWorld;
};

std::string World::getName() {
    return this->name;
};

void World::setCube(std::shared_ptr<Cube> c, Position<int> pos) {
    int x = floor((float) pos.getX() / Chunk::W);
    int y = floor((float) pos.getY() / Chunk::H);
    int z = floor((float) pos.getZ() / Chunk::Z);

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
