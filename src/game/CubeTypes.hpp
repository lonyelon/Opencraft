#ifndef _CUBE_TYPES_HPP_
#define _CUBE_TYPES_HPP_

enum CubeType {
    dirt,
    stone,
    water,
    air,
};

bool isTransparent(CubeType t);

#endif