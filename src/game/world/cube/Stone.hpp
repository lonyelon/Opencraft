#ifndef GAME_WORLD_CUBE_STONE_HPP
#define GAME_WORLD_CUBE_STONE_HPP

#include "Cube.hpp"

class Stone : public Cube {
public:
	Stone() : Cube() {
		this->transparent = false;
		this->type = CubeType::stone;
	}

    void update() {

    };
};

#endif
