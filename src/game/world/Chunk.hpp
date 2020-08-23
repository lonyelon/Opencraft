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
    std::weak_ptr<World> world;
    std::unique_ptr<Model> chunkModel = nullptr;

    std::mutex mutex;

    std::array<std::shared_ptr<Cube>, 16 * 16 * 16> cubes;
    std::vector<std::shared_ptr<Cube>> renderedCubes;
    bool generated = false;
    bool updated = false;

public:
    static const int W = 16, H = 16, Z = 16;

    Chunk(std::weak_ptr<World> w, Position<int> pos) : WorldObject(pos), world(w) {}

    Chunk(std::weak_ptr<World> w, int posX, int posY, int posZ) : WorldObject(Position(posX, posY, posZ)),
                                                                  world(w) {} // TODO delete this

    void genTerrain();

    std::shared_ptr<Cube> getCube(Position<int> pos);

    void setCube(std::shared_ptr<Cube> c, Position<int> pos);

    std::array<std::shared_ptr<Cube>, Chunk::W * Chunk::H * Chunk::Z> getCubes() const;

    int isIllated(int x, int y, int z);

    bool isGenerated() const { return this->generated; };

    void genVao();

    void getVisibleCubes();

    void updateModel();

    int getCubeCount() { return this->renderedCubes.size(); };

    int getX() const { return this->position.getX(); };

    int getY() const { return this->position.getY(); };

    int getZ() const { return this->position.getZ(); };

    void save() const;

    void load();

    void draw();

    std::weak_ptr<World> getWorld();

    void setUpdated(bool update) { this->updated = update; };

    bool isDrawn() const {
        return this->chunkModel != nullptr && this->chunkModel->getVao() != 0;
    }

    void update() {};

    ~Chunk();
};

#endif
