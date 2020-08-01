#ifndef GAME_WORLD_HPP
#define GAME_WORLD_HPP

#include <vector>
#include <thread>
#include <memory>

#include <game/world/Chunk.hpp>
#include <game/world/cube/Cubes.hpp>


#include <game/Player.hpp>

class World {
private:
    std::string name;

    int seed;
    int size;
    int chunkCount;
    bool updateWorld;

    std::vector<Chunk *> chunks;
    std::vector<Chunk *> drawQueue;
    std::unique_ptr<std::thread> genThread;

public:
    World(std::string name, int seed);

    void genChunks();

    void genChunkAt(bool draw, int x, int y, int z);

    void setSize(const int size);

    void setCube(std::shared_ptr<Cube> c, FixedPosition pos);

    std::shared_ptr<Cube> getCube(FixedPosition pos);

    std::shared_ptr<Cube> getCube(Chunk *c, FixedPosition pos);

    std::shared_ptr<Cube> getCube(int x, int y, int z); // TODO delete this
    std::shared_ptr<Cube> getCube(Chunk *c, int x, int y, int z); // TODO delete this
    std::shared_ptr<Cube> getCube(float x, float y, float z) {
        return this->getCube((int) round(x), (int) round(y), (int) round(z));
    }; // TODO delete this
    std::shared_ptr<Cube> getCube(double x, double y, double z) {
        return this->getCube((int) round(x), (int) round(y), (int) round(z));
    }; // TODO delete this
    void setSeed(int seed);

    int getSeed();

    int getChunkCount();

    std::vector<Chunk *> getChunks();

    Chunk *getChunk(int x, int y, int z);

    void deleteChunk(Chunk *c);

    bool isWorldUpdating();

    void addChunkToQueue(Chunk *c);

    void draw();

    std::string getName();

    void saveWorld();

    int getCubesDrawn();

    ~World(); // Free memory
};

#endif
