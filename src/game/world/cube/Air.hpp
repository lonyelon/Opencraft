#ifndef GAME_WORLD_CUBE_AIR_HPP
#define GAME_WORLD_CUBE_AIR_HPP

#include "Cube.hpp"

class Air : public Cube {
public:
	Air() : Cube() {
		this->transparent = true;
		this->type = CubeType::air;
	}
};

#endif
