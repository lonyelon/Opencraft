#ifndef GAME_WORLD_HPP
#define GAME_WORLD_HPP

#include <vector>

#include <boost/thread.hpp>

#include "Chunk.hpp"

class World {
private:
    int seed;
    int size;
    int chunkCount;

    bool updateWorld;

    std::vector<Chunk*> chunks;
    std::vector<Cube*> cubesToRender;

    std::vector<Chunk*> drawQueue;

    boost::thread *genThread;
public:
    World(  );

    void genChunks(  );
    void genChunkAt(int x, int y, int z);
    void setSize(const int size );
    Cube *getCube( int x, int y, int z );
    Cube *getCube( float x, float y, float z ) { return this->getCube((int)round(x), (int)round(y), (int)round(z)); };
    Cube *getCube( double x, double y, double z ) { return this->getCube((int)round(x), (int)round(y), (int)round(z)); };
    Cube *getCube( Chunk *c, int x, int y, int z );
    void setSeed( int seed );
    int getSeed(  );
    int getChunkCount(  );
    std::vector<Chunk*> getChunks();
    Chunk *getChunk(int x, int y, int z);
    void deleteChunk(Chunk *c);

    bool isWorldUpdating() { return this->updateWorld; };

    void addChunkToQueue(Chunk *c);

    void draw(  );

    ~World() {
        if (this->genThread != NULL) {
            this->updateWorld = false;
            this->genThread->join();
        }
    }

    int getCubesDrawn() {
        int c = 0;
        for (int i = 0; i < this->chunks.size(); i++) {
            c += this->chunks[i]->getCubeCount();
        }
        return c;
    }
};

#endif
