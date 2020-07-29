#ifndef GAME_WORLD_CUBE_DIRT_HPP
#define GAME_WORLD_CUBE_DIRT_HPP

#include "Cube.hpp"

class Dirt : public Cube {
public:
	Dirt() : Cube() {
		this->transparent = false;
		this->type = CubeType::dirt;
	}
};

#endif
