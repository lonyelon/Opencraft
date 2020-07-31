#ifndef GAME_WORLD_CUBE_GRASSYDIRT_HPP
#define GAME_WORLD_CUBE_GRASSYDIRT_HPP

#include "Cube.hpp"
#include <game/world/World.hpp>

class GrassyDirt : public Cube {
public:
	GrassyDirt();
    void update();
};

#endif
