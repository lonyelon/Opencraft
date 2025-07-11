#ifndef _CHUNK_HPP_
#define _CHUNK_HPP_

#include <vector>
#include <memory>
#include <array>
#include <mutex>

#include "Chunk.hpp"
#include <game/world/WorldObject.hpp>
#include "cube/Cube.hpp"
#include "cube/Dirt.hpp"
#include <engine/model/Model.hpp>

class World;

class Chunk : public WorldObject {
private:
    World *world;
    std::unique_ptr<Model> chunkModel = nullptr;

    std::mutex mutex;

    std::array<std::shared_ptr<Cube>, 32 * 32 * 32> cubes;
    std::vector<std::shared_ptr<Cube>> renderedCubes;
    bool generated = false;
    bool updated = false;

public:
    static const int W = 32, H = 32, Z = 32;

    Chunk(World *w, Position<int> pos) : WorldObject(pos), world(w) {}

    Chunk(World *w, int posX, int posY, int posZ) : WorldObject(Position(posX, posY, posZ)),
                                                                  world(w) {} // TODO delete this

    void genTerrain();

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

    void save() const;

    void load();

    void draw();

    World *getWorld();

    void setUpdated(bool update) { this->updated = update; };

    bool isDrawn() const {
        return this->chunkModel != nullptr && this->chunkModel->getVao() != 0;
    }

    int isUpdated() const { return this->updated; };

    void update() {};

    ~Chunk();
};

#endif
