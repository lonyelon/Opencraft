#ifndef OPENCRAFT_WORLDOBJECT_HPP
#define OPENCRAFT_WORLDOBJECT_HPP

#include <engine/position/Position.hpp>

class WorldObject {
public:
    Position<int> position;

    WorldObject() = default;

    explicit WorldObject(Position<int> position) : position(position) {};

    virtual void update() = 0;
};

#endif
