#ifndef _CUBE_TYPES_HPP_
#define _CUBE_TYPES_HPP_

#include <memory>

class Cube;

enum CubeType {
	air = 0,
	stone = 1,
	dirt = 2,
    grassyDirt = 3,
	sand = 4,
    water = 5,
    lava = 6,
    grass = 7,
    wood = 8,
    treeleaves = 9
};

std::shared_ptr<Cube> getCubeObj(CubeType t);

#endif
