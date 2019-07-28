#include "World.hpp"

#include <boost/thread.hpp>

#include "../Player.hpp"

extern Player *p;

World::World(  ) {
    this->seed = 279;
    this->size = 15;
    this->chunkCount = 0;
    this->genThread = NULL;
    this->updateWorld = false;
}

void World::setSize( const int size ) {
    this->size = size;
}

void World::genChunkAt(int x, int y, int z) {
    Chunk *c = new Chunk(this, x, y, z);
    this->chunks.push_back(c);
    c->genTerrain();
    this->chunkCount++;
    c->getVisibleCubes();
    
    this->addChunkToQueue(c);
}

void World::addChunkToQueue(Chunk *c) {
    this->drawQueue.push_back(c);
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
        (*chunks)[i]->genVao();
    }
}

extern float renderDistance;

void worldUpdate( World *world, Player *player ) {
    const int maxDist = round(renderDistance/16);

    while (world->isWorldUpdating()) {
        Cube *c = world->getCube(player->getCam()->getX(), player->getCam()->getY(), player->getCam()->getZ());
        if (c == NULL) continue;
        Chunk *ck = c->getChunk();

        for (int radius = 0; radius < maxDist; radius++) {
            for (int x = -radius; x < radius; x++ ) {
                for (int z = -radius; z < radius; z++ ) {
                    if (world->getChunk(ck->getX()+x, ck->getY(), ck->getZ()+z) == NULL) {
                        world->genChunkAt(ck->getX()+x, ck->getY(), ck->getZ()+z);
                    }
                }
            }
        }

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

void World::deleteChunk(Chunk *c) {
    for (int i = 0; i < this->chunks.size(); i++) {
        if (this->chunks[i]->getX() == c->getX()
            && this->chunks[i]->getY() == c->getY()
            && this->chunks[i]->getZ() == c->getZ() ) {
            delete(c);
            this->chunks.erase(this->chunks.begin()+i);
            this->chunkCount--;
            break;
        }
    }
}

void World::genChunks(  ) {
    printf("Reserving memory for the world...\n");
    this->updateWorld = false;
    if (this->genThread != NULL) this->genThread->join();
    for (int i = 0; i < this->chunks.size(); i++) {
		delete(this->chunks[i]);
	}
	this->chunks.clear();
    this->chunkCount = 0;

	const int threadCount = 8;

	this->chunks = std::vector<Chunk*>(size*size, NULL);
	
    printf("Generating world...\n");

	boost::thread t[threadCount];
	for (int i = 0; i < threadCount; i++) {
		t[i] = boost::thread( genChunk, &(this->chunks), &(this->chunkCount), this->size, this, i, threadCount );
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

    for (int i = 0; i < this->chunkCount; i++) {
        printf("%d/%d\n", i, this->chunkCount);
        this->chunks[i]->getVisibleCubes();
        this->chunks[i]->genVao();
    }

    this->updateWorld = true;
    this->genThread = new boost::thread( worldUpdate, this, p );

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

    if ( y >= 256 || y <= 0  ) {
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
    
    if ( k != NULL && k->getX() == chunkX && k->getZ() == chunkZ ) {
        Cube *cube = k->getCube( x - k->getX()*16, y, z - k->getZ()*16 );
        return cube;
    }

    for ( int i = 0; i < this->chunks.size(); i++ ) {
        if ( k==NULL )
            printf("getcube %d\n", i);
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

Chunk *World::getChunk(int x, int y, int z) {
    for (int i = 0; i < this->chunkCount; i++) {
        if (this->chunks[i]->getX() == x && this->chunks[i]->getZ() == z) {
            return this->chunks[i];
        }
    }
    return NULL;
}