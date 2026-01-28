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
    static constexpr float size_reduction = 4.0f;

    Cube() = default;

    Cube(Chunk *c, int x, int y, int z) : WorldObject(Position(x, y, z)), chunk(c) {};

    Cube(Chunk *c, Position<int> p) : WorldObject(p), chunk(c) {};

    void setPos(Position<int> p);

    void setType(CubeType t);

    void setSides(int s) { this->sides = s; };

    void setChunkPos(Position<int> pos) { this->chunkPos = pos; };

    void setChunk(Chunk *c) { this->chunk = c; };

    CubeType getType() const;

    int getSides() const { return this->sides; };

    void getVertex(std::vector<float> *v, std::vector<int> *i) const;

    int getX() const { return this->position.x; };

    int getY() const { return this->position.y; };

    int getZ() const { return this->position.z; };

    Chunk *getChunk() const { return this->chunk; };

    bool isTransparent() const { return this->transparent; }

    Position<int> getChunkPos() const { return this->chunkPos; };

    Position<int> getPos() const;

    virtual void update() = 0;

    ~Cube();
};


#endif
