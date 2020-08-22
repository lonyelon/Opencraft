#ifndef OPENCRAFT_WORLDOBJECT_HPP
#define OPENCRAFT_WORLDOBJECT_HPP

#include <engine/position/Position.hpp>

class WorldObject {
protected:
    Position<int> position;
public:
    WorldObject() = default;

    explicit WorldObject(Position<int> pos) : position(pos) {};

    auto getPos() const { return this->position; };

    void setPos(Position<int> newPos) { this->position = newPos; };

    virtual void update() = 0;
};

#endif
