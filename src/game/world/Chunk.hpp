#ifndef _CHUNK_HPP_
#define _CHUNK_HPP_

#include <array>
#include <memory>
#include <mutex>
#include <vector>

#include "Chunk.hpp"
#include <game/world/WorldObject.hpp>
#include "cube/Cube.hpp"
#include "cube/Dirt.hpp"
#include <engine/model/Model.hpp>

#define CHUNK_SIZE 32

class World;

class Chunk : public WorldObject {
private:
    World *world;
    std::unique_ptr<Model> chunkModel = nullptr;

    std::array<std::shared_ptr<Cube>, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> cubes;
    std::vector<std::shared_ptr<Cube>> renderedCubes;
    bool updated = false;

public:
    bool generated = false;
    std::mutex mutex;
    
    static const int W = CHUNK_SIZE, H = CHUNK_SIZE, Z = CHUNK_SIZE;

    Chunk(World *w, Position<int> pos) : WorldObject(pos), world(w) {}

    Chunk(World *w, int posX, int posY, int posZ) : WorldObject(Position(posX, posY, posZ)),
                                                                  world(w) {} // TODO delete this

    std::shared_ptr<Cube> getCube(Position<int> pos);

    void setCube(std::shared_ptr<Cube> c, Position<int> pos);

    std::array<std::shared_ptr<Cube>, Chunk::W * Chunk::H * Chunk::Z> getCubes() const;

    bool isCubeCovered(int x, int y, int z);

    void getCubeVisibleSides(int x, int y, int z);

    bool isGenerated() const { return this->generated; };

    void genVao();

    void getVisibleCubes();

    void updateModel();

    int getCubeCount() { return this->renderedCubes.size(); };

    int getX() const { return this->position.x; };

    int getY() const { return this->position.y; };

    int getZ() const { return this->position.z; };

    void save();

    void load();

    void draw();

    World *getWorld();

    void setUpdated(bool update) { this->updated = update; };

    bool isDrawn() const {
        return this->chunkModel != nullptr && this->chunkModel->getVao() != 0;
    }

    int isUpdated() const { return this->updated; };

    void update() {};

    std::vector<char> toBytes() const;

    ~Chunk();
};

#endif
