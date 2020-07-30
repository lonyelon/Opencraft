#include "CubeTypes.hpp"
#include "cube/Cube.hpp"

bool isTransparent(Cube *c) {
    CubeType t = c->getType();
    if (
        t == CubeType::air ||
        t == CubeType::water ||
        t == CubeType::lava ||
        t == CubeType::grass
    ) {
        return true;
    }
    return false;
}
