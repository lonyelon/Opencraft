#ifndef GAME_WORLD_HPP
#define GAME_WORLD_HPP

#include <vector>

#include <thread>

#include <game/world/Chunk.hpp>
#include <game/world/cube/Cubes.hpp>


#include <game/Player.hpp>

extern Player *p;

class World {
private:
    int seed;
    int size;
    int chunkCount;
    bool updateWorld;
	std::string name;
    std::vector<Chunk*> chunks;
    std::vector<Cube*> cubesToRender;
    std::vector<Chunk*> drawQueue;
    std::thread *genThread;
public:
    World( std::string name, int seed );

    void genChunks(  );
    void genChunkAt(bool draw, int x, int y, int z);
    void setSize(const int size );

    void setCube(Cube *c, FixedPosition pos);
    Cube *getCube(FixedPosition pos);
    Cube *getCube(Chunk *c, FixedPosition pos);
    Cube *getCube( int x, int y, int z ); // TODO delete this
    Cube *getCube( Chunk *c, int x, int y, int z ); // TODO delete this
    Cube *getCube( float x, float y, float z ) { return this->getCube((int)round(x), (int)round(y), (int)round(z)); }; // TODO delete this
    Cube *getCube( double x, double y, double z ) { return this->getCube((int)round(x), (int)round(y), (int)round(z)); }; // TODO delete this
    void setSeed( int seed );
    int getSeed();
    int getChunkCount();
    std::vector<Chunk*> getChunks();
    Chunk *getChunk(int x, int y, int z);
    void deleteChunk(Chunk *c);
    bool isWorldUpdating();
    void addChunkToQueue(Chunk *c);
    void draw(  );
	std::string getName();
	void saveWorld();
    int getCubesDrawn();
    ~World(); // Free memory
};

#endif
