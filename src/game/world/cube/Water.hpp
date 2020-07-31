#ifndef GAME_WORLD_CUBE_WATER_HPP
#define GAME_WORLD_CUBE_WATER_HPP

#include "Cube.hpp"

class Water : public Cube {
public:
	Water() : Cube() {
		this->transparent = true;
		this->type = CubeType::water;
	}

    void update() {

    };
};

#endif
