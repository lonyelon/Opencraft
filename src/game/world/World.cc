#include "World.hpp"

#include <thread>

World::World() {
    this->seed = 0;
    this->size = 15;
    this->chunkCount = 0;
}

void genChunk( std::vector<Chunk*> *chunks, int *chunkCount, int size, World *w, int threadNumber, int threadCount) {
	for ( int x = 0; x < size; x++ ) {
		for ( int y = threadNumber; y < size; y += threadCount ) {
			//c->push_back(new Chunk(x - size/2, 0, y - size/2));
			(*chunks)[x*size+y] = new Chunk( w, x - size/2, 0, y - size/2 );
			(*chunks)[x*size+y]->genTerrain();
			(*chunkCount)++;
		}
	}
}

void genVAOs( std::vector<Chunk*> *chunks, int threadNumber, int threadCount ) {
    for ( int i = threadNumber; i < (*chunks).size(); i += threadCount ) {
        (*chunks)[i]->getVisibleCubes();
    }
}

void World::genChunks(  ) {
    printf("Reserving memory for the world...\n");
    for (int i = 0; i < this->chunks.size(); i++) {
		delete(this->chunks[i]);
	}
	this->chunks.clear();

	const int threadCount = 8;

	this->chunks = std::vector<Chunk*>(size*size, NULL);
	
    printf("Generating world...\n");

	std::thread t[threadCount];
	for (int i = 0; i < threadCount; i++) {
		t[i] = std::thread( genChunk, &(this->chunks), &(this->chunkCount), this->size, this, i, threadCount );
	}

	for (int i = 0; i < threadCount; i++) {
		t[i].join();
	}

    printf( "Rendering chunks...\n" );

    for (int i = 0; i < threadCount; i++) {
		t[i] = std::thread( genVAOs, &(this->chunks), i, threadCount );
	}

	for (int i = 0; i < threadCount; i++) {
		t[i].join();
	}
}

void World::draw() {
    for ( int k = 0; k < this->chunks.size(); k++ ) {
		this->chunks[k]->genVao();
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

Cube *World::getCube( int x, int y, int z ) {
    Chunk *c = NULL;

    int chunkX = x/16;
    int chunkZ = z/16;

    if ( x < 0 ) {
        chunkX = -((-x-1)/16 + 1);
    }

    if ( z < 0 ) {
        chunkZ = -((-z-1)/16 + 1);
    }

    for ( int i = 0; i < this->chunks.size(); i++ ) {
        if ( this->chunks[i]->getX() == chunkX && this->chunks[i]->getZ() == chunkZ ) {
            c = this->chunks[i];
            break;
        }
    }

    if ( c == NULL ) {
        return NULL;
    }

    if ( c->getZ() >= this->size || c->getX() >= this->size || y >= 256 || y <= 0  ) {
        return NULL;
    }
    
    return c->getCube( x - c->getX()*16, y, z - c->getZ()*16 );
}

Cube *World::getCube( Chunk *k, int x, int y, int z ) {
    Chunk *c = NULL;

    int chunkX = x/16;
    int chunkZ = z/16;

    if ( x < 0 ) {
        chunkX = -((-x-1)/16 + 1);
    }

    if ( z < 0 ) {
        chunkZ = -((-z-1)/16 + 1);
    }

    if ( y >= 256 || y <= 0  ) {
        return NULL;
    }
    
    if ( k->getX() == chunkX && k->getZ() == chunkZ ) {
        Cube *cube = k->getCube( x - k->getX()*16, y, z - k->getZ()*16 );
        return cube;
    }

    for ( int i = 0; i < this->chunks.size(); i++ ) {
        if ( this->chunks[i]->getX() == chunkX && this->chunks[i]->getZ() == chunkZ ) {
            c = this->chunks[i];
            break;
        }
    }

    if ( c == NULL ) {
        return NULL;
    }

    if ( c->getZ() >= this->size || c->getX() >= this->size || y >= 256 || y <= 0  ) {
        return NULL;
    }
    
    return c->getCube( x - c->getX()*16, y, z - c->getZ()*16 );
}


void World::setSeed( int seed ) {
    this->seed = seed;
} 

int World::getSeed(  ) {
    return this->seed;
}

int World::getChunkCount(  ) {
    return this->chunks.size();
}

std::vector<Chunk*> World::getChunks() {
    return this->chunks;
}