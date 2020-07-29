#ifndef GAME_WORLD_CUBE_GRASSYDIRT_HPP
#define GAME_WORLD_CUBE_GRASSYDIRT_HPP

#include "Cube.hpp"

class GrassyDirt : public Cube {
public:
	GrassyDirt() : Cube() {
		this->transparent = false;
		this->type = CubeType::grassyDirt;
	}
};

#endif
