#ifndef GAME_WORLD_HPP
#define GAME_WORLD_HPP

#include <vector>
#include <thread>
#include <memory>
#include <map>
#include <mutex>

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

    std::map<Position<int>, Chunk*> chunks;
    std::vector<Chunk *> drawQueue;
    std::unique_ptr<std::thread> genThread;

public:
    std::mutex chunkMutex, drawMutex;

    World(std::string name, int seed);

    void genChunks();

    void genChunkAt(bool draw, int x, int y, int z);

    void setSize(const int size);

    void setCube(std::shared_ptr<Cube> c, Position<int> pos);

    std::shared_ptr<Cube> getCube(Position<int> pos);

    std::shared_ptr<Cube> getCube(Chunk *c, Position<int> pos);

    std::shared_ptr<Cube> getCube(int x, int y, int z); // TODO delete this
    std::shared_ptr<Cube> getCube(Chunk *c, int x, int y, int z); // TODO delete this
    std::shared_ptr<Cube> getCube(float x, float y, float z) {
        return this->getCube((int) round(x), (int) round(y), (int) round(z));
    }; // TODO delete this
    std::shared_ptr<Cube> getCube(double x, double y, double z) {
        return this->getCube((int) round(x), (int) round(y), (int) round(z));
    }; // TODO delete this
    void setSeed(int seed);

    int getSeed() const;

    int getChunkCount() const;

    std::map<Position<int>, Chunk*> getChunks() const;

    Chunk *getChunk(int x, int y, int z);

    void deleteChunk(Chunk *c);

    bool isWorldUpdating() const;

    void addChunkToQueue(Chunk *c);

    void draw();

    std::string getName();

    void saveWorld();

    ~World(); // Free memory
};

#endif
