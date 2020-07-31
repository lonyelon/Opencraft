#ifndef GAME_WORLD_CUBE_SAND_HPP
#define GAME_WORLD_CUBE_SAND_HPP

#include "Cube.hpp"

class Sand : public Cube {
public:
	Sand() : Cube() {
		this->transparent = false;
		this->type = CubeType::sand;
	}

    void update() {

    };
};

#endif
