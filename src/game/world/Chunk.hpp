#ifndef _CHUNK_HPP_
#define _CHUNK_HPP_

#include <vector>
#include <memory>

#include "Chunk.hpp"
#include "cube/Cube.hpp"
#include "cube/Dirt.hpp"
#include <engine/model/Model.hpp>

class World;

class Chunk {
private:
    Position<int> pos;
    std::vector<std::shared_ptr<Cube>> cubes;
    std::vector<std::shared_ptr<Cube>> renderedCubes;
    std::weak_ptr<World> world;
    bool generated;
    bool updated = false;
    std::unique_ptr<Model> chunkModel;

public:
    static const int W = 16, H = 16, Z = 16;

    Chunk(std::weak_ptr<World> w, int posX, int posY, int posZ);

    void genTerrain();

    std::shared_ptr<Cube> getCube(unsigned int x, int y, int z); // TODO remove this
    std::shared_ptr<Cube> getCube(Position<int> pos);

    void setCube(std::shared_ptr<Cube> c, int x, int y, int z); // TODO remove this
    void setCube(std::shared_ptr<Cube> c, Position<int> pos);

    std::vector<std::shared_ptr<Cube> > getCubes();

    int isIllated(int x, int y, int z);

    void genVao();

    void getVisibleCubes();

    void updateModel();

    int getCubeCount() { return this->renderedCubes.size(); };

    int getX() { return this->pos.getX(); };

    int getY() { return this->pos.getY(); };

    int getZ() { return this->pos.getZ(); };

    void Save();

    void Load();

    void draw();

    std::weak_ptr<World> getWorld();

    void setUpdated(bool update) { this->updated = update; };

    bool isDrawn() {
        return this->chunkModel->getVao() != 0;
    }

    ~Chunk();
};

#endif
