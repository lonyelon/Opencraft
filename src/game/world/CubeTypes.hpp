#ifndef _CUBE_TYPES_HPP_
#define _CUBE_TYPES_HPP_

enum CubeType {
    grassyDirt,
    dirt,
    stone,
    water,
    air,
    sand,
    lava
};

bool isTransparent(CubeType t);

#endif