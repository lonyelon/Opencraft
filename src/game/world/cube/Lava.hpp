#ifndef GAME_WORLD_CUBE_LAVA_HPP
#define GAME_WORLD_CUBE_LAVA_HPP

#include "Cube.hpp"

class Lava : public Cube {
public:
	Lava() : Cube() {
		this->transparent = true;
		this->type = CubeType::lava;
	}
};

#endif
