#ifndef GAME_WORLD_CUBE_GRASS_HPP
#define GAME_WORLD_CUBE_GRASS_HPP

#include "Cube.hpp"

class Grass : public Cube {
public:
	Grass() : Cube() {
		this->transparent = false;
		this->type = CubeType::grass;
	}
};

#endif
