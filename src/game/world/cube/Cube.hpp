#ifndef _CUBE_HPP_
#define _CUBE_HPP_

#include <vector>

#include <game/world/WorldObject.hpp>

#include "CubeTypes.hpp"
#include "../../../engine/Engine.hpp"
#include "engine/position/Position.hpp"

class Chunk;

class Cube : public WorldObject {
protected:
    Chunk *chunk = nullptr;
    CubeType type = CubeType::air;

    int sides = 1;
    Position<int> chunkPos = Position(0, 0, 0);

    bool transparent = true;

public:
    Cube() = default;

    Cube(Chunk *c, int x, int y, int z) : chunk(c), WorldObject(Position(x, y, z)) {};

    Cube(Chunk *c, Position<int> p) : chunk(c), WorldObject(p) {};

    void setPos(Position<int> p);

    void setType(CubeType t);

    void setSides(int s) { this->sides = s; };

    void setChunkPos(Position<int> pos) { this->chunkPos = pos; };

    void setChunk(Chunk *c) { this->chunk = c; };

    CubeType getType() const;

    int getSides() const { return this->sides; };

    void getVertex(std::vector<float> *v, std::vector<int> *i) const;

    int getX() const { return this->position.getX(); };

    int getY() const { return this->position.getY(); };

    int getZ() const { return this->position.getZ(); };

    Chunk *getChunk() const { return this->chunk; };

    bool isTransparent() const { return this->transparent; }

    Position<int> getChunkPos() const { return this->chunkPos; };

    Position<int> getPos() const;

    virtual void update() = 0;

    ~Cube();
};


#endif
