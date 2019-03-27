#include "CubeTypes.hpp"

bool isTransparent(CubeType t) {
    switch (t) {
        case CubeType::air:
            return true;
        case CubeType::water:
            return true;
    }
    return false;
}