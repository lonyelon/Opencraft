#include <game/world/World.hpp>

#include <thread>
#include <filesystem>

#include <game/Player.hpp>
#include <game/world/WorldGenerator.hpp>

extern Player *p;

World::World(std::string name, int seed) {
    this->name = name;
    this->seed = seed;
    this->size = 15;
    this->chunkCount = 0;
    this->genThread = NULL;
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
    if (this->getChunk(x, y, z) != NULL) {
        return;
    }

    Chunk *c = new Chunk(this, x, y, z);
    this->chunks.push_back(c);
    this->chunkCount++;
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
    this->drawQueue.push_back(c);
}

/*
	Deletes a chunk from memory.
*/
void World::deleteChunk(Chunk *c) {
    for (int i = 0; i < this->chunkCount; i++) {
        int chunX = this->chunks[i]->getX();
        int chunY = this->chunks[i]->getY();
        int chunZ = this->chunks[i]->getZ();

        if (chunX == c->getX() && chunY == c->getY() && chunZ == c->getZ()) {
            delete (c);
            this->chunks.erase(this->chunks.begin() + i);
            this->chunkCount--;
            break;
        }
    }
}

void World::genChunks() {
    const int threadCount = 8;

    printf("Reserving memory for the world...\n");

    this->updateWorld = false;
    if (this->genThread != NULL) this->genThread->join();
    for (int i = 0; i < this->chunkCount; i++) {
        delete (this->chunks[i]);
    }
    this->chunks.clear();
    this->chunkCount = 0;

    this->chunks = std::vector<Chunk *>(size * size * size, NULL);

    printf("Generating world...\n");

    std::thread t[threadCount];
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
    int count = this->chunkCount;
    for (int i = 0; i < count; i++) {
        printf("%d/%d\n", i, count);
        this->chunks[i]->getVisibleCubes();
        this->chunks[i]->genVao();
    }

    this->updateWorld = true;
    this->genThread = new std::thread(WorldGenerator::worldUpdate, this, p);

    printf("Complete!\n");
}

void World::draw() {
    for (int i = 0; i < this->drawQueue.size(); i++) {
        this->drawQueue[i]->genVao();
    }
    this->drawQueue.clear();

    for (int k = 0; k < this->chunks.size(); k++) {
        //this->chunks[k]->genVao();
        this->chunks[k]->draw();
    }
}

int sign(int x) {
    if (x < 0) {
        return -1;
    } else {
        return 1;
    }
}

/*
	Returns a cube by it's coordinates.
*/
Cube *World::getCube(int x, int y, int z) {
    this->getCube(FixedPosition(x, y, z));
}

Cube *World::getCube(FixedPosition pos) {
    Chunk *c = NULL;

    int chunkX = floor((float) pos.getX() / Chunk::W);
    int chunkY = floor((float) pos.getY() / Chunk::H);
    int chunkZ = floor((float) pos.getZ() / Chunk::Z);

    c = this->getChunk(chunkX, chunkY, chunkZ);

    if (c == NULL) {
        return NULL;
    }

    return c->getCube(pos.getX() - c->getX() * 16, pos.getY() - c->getY() * 16, pos.getZ() - c->getZ() * 16);
}

/*
	Returns a cube by it's coordinates, but first it checks it's own chunk.
*/
Cube *World::getCube(Chunk *k, int x, int y, int z) {
    return this->getCube(k, FixedPosition(x, y, z));
}

Cube *World::getCube(Chunk *k, FixedPosition pos) {
    Chunk *c = NULL;

    int chunkX = floor((float) pos.getX() / 16);
    int chunkY = floor((float) pos.getY() / 16);
    int chunkZ = floor((float) pos.getZ() / 16);

    if (k != NULL && k->getX() == chunkX && k->getY() == chunkY && k->getZ() == chunkZ) {
        Cube *cube = k->getCube(pos.getX() - k->getX() * Chunk::W, pos.getY() - k->getY() * Chunk::H,
                                pos.getZ() - k->getZ() * Chunk::Z);
        return cube;
    }

    return this->getCube(pos);
}

void World::setSeed(int seed) {
    this->seed = seed;
}

int World::getSeed() {
    return this->seed;
}

int World::getChunkCount() {
    return this->chunkCount;
}

std::vector<Chunk *> World::getChunks() {
    return this->chunks;
}

/*
	Get's a chunk by it's coordinates.
*/
Chunk *World::getChunk(int x, int y, int z) {
    for (int i = 0; i < this->chunkCount; i++) {
        if (this->chunks[i] == NULL) {
            continue;
        }
        if (this->chunks[i]->getX() == x && this->chunks[i]->getY() == y && this->chunks[i]->getZ() == z) {
            return this->chunks[i];
        }
    }
    return NULL;
}

World::~World() {
    if (this->genThread != NULL) {
        this->updateWorld = false;
        this->genThread->join();
    }

    this->saveWorld();
}

void World::saveWorld() {
    std::ofstream file("saves/" + this->name + "/playerdata.txt");

    file << p->getCam()->getX() << "\t" << p->getCam()->getY() << "\t" << p->getCam()->getZ();

    file.close();

    for (int i = 0; i < this->chunks.size(); i++) {
        delete (this->chunks[i]);
    }
}

int World::getCubesDrawn() {
    int c = 0;
    for (int i = 0; i < this->chunks.size(); i++) {
        c += this->chunks[i]->getCubeCount();
    }
    return c;
}

bool World::isWorldUpdating() {
    return this->updateWorld;
};

std::string World::getName() {
    return this->name;
};

void World::setCube(Cube *c, FixedPosition pos) {
    int x = floor((float) pos.getX() / Chunk::W);
    int y = floor((float) pos.getY() / Chunk::H);
    int z = floor((float) pos.getZ() / Chunk::Z);

    Chunk *chunk = this->getChunk(x, y, z);
    if (chunk != NULL) {
        FixedPosition newPos = pos.move(FixedPosition(-1*x*Chunk::W, -1*y*Chunk::H, -1*z*Chunk::Z));
        chunk->setCube(c, newPos);
    }
};