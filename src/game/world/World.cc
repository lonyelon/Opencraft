#include <game/world/World.hpp>

#include <thread>
#include <filesystem>

#include <game/Player.hpp>
#include <game/world/WorldGenerator.hpp>

extern Player *p;

World::World( std::string name, int seed ) {
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

void World::setSize( const int size ) {
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

	printf("Generating chunk %d %d %d\n", x, y, z);

    Chunk *c = new Chunk(this, x, y, z);
    this->chunks.push_back(c);
    c->genTerrain();
    this->chunkCount++;

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

        if (chunX == c->getX() && chunY == c->getY() && chunZ == c->getZ() ) {
            delete(c);
            this->chunks.erase(this->chunks.begin()+i);
            this->chunkCount--;
            break;
        }
    }
}

void World::genChunks(  ) {
	const int threadCount = 8;

    printf("Reserving memory for the world...\n");

    this->updateWorld = false;
    if (this->genThread != NULL) this->genThread->join();
    for (int i = 0; i < this->chunkCount; i++) {
		delete(this->chunks[i]);
	}
	this->chunks.clear();
    this->chunkCount = 0;

	this->chunks = std::vector<Chunk*>(size*size*size, NULL);

    printf("Generating world...\n");

	std::thread t[threadCount];
	for (int i = 0; i < threadCount; i++) {
		t[i] = std::thread( genChunk, &(this->chunks), &(this->chunkCount), this->size, this, i, threadCount );
	}

	for (int i = 0; i < threadCount; i++) {
		t[i].join();
	}

    printf( "Rendering chunks...\n" );

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
    this->genThread = new std::thread( worldUpdate, this, p );

    printf("Complete!\n");
}

void World::draw() {
    for (int i = 0; i < this->drawQueue.size(); i++) {
        this->drawQueue[i]->genVao();
    }
    this->drawQueue.clear();

    for ( int k = 0; k < this->chunks.size(); k++ ) {
		//this->chunks[k]->genVao();
		this->chunks[k]->draw();
	}
}

int sign( int x ) {
    if ( x < 0 ) {
        return -1;
    } else {
        return 1;
    }
}

/*
	Returns a cube by it's coordinates.
*/
Cube *World::getCube( int x, int y, int z ) {
    Chunk *c = NULL;

    int chunkX = floor((float)x/Chunk::W);
	int chunkY = floor((float)y/Chunk::H);
    int chunkZ = floor((float)z/Chunk::Z);

    c = this->getChunk( chunkX, chunkY, chunkZ );

    if ( c == NULL ) {
        return NULL;
    }

	// TODO DELETE THIS
    /*if ( y >= Chunk::H ) {
        return NULL;
    }*/

    return c->getCube( x - c->getX()*16, y - c->getY()*16, z - c->getZ()*16 );
}

/*
	Returns a cube by it's coordinates, but first it checks it's own chunk.
*/
Cube *World::getCube( Chunk *k, int x, int y, int z ) {
    Chunk *c = NULL;

    int chunkX = floor((float)x/16);
	int chunkY = floor((float)y/16);
    int chunkZ = floor((float)z/16);

    if ( k != NULL && k->getX() == chunkX && k->getY() == chunkY && k->getZ() == chunkZ ) {
        Cube *cube = k->getCube( x - k->getX()*Chunk::W, y  - k->getY()*Chunk::H, z - k->getZ()*Chunk::Z );
        return cube;
    }

	// TODO DELETE THIS
    /*if ( y >= Chunk::H ) {
        return NULL;
    }*/

    return this->getCube(x, y, z);
}


void World::setSeed( int seed ) {
    this->seed = seed;
}

int World::getSeed(  ) {
    return this->seed;
}

int World::getChunkCount(  ) {
    return this->chunkCount;
}

std::vector<Chunk*> World::getChunks() {
    return this->chunks;
}

/*
	Get's a chunk by it's coordinates.
*/
Chunk *World::getChunk(int x, int y, int z) {
    for (int i = 0; i < this->chunkCount; i++) {
        if (this->chunks[i]->getX() == x && this->chunks[i]->getY() == y && this->chunks[i]->getZ() == z) {
            return this->chunks[i];
        }
    }
    return NULL;
}
