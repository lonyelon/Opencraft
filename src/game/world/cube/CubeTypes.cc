#include "CubeTypes.hpp"

#include "Cubes.hpp"

std::shared_ptr<Cube> getCubeObj(CubeType t) {
    switch (t) {
        case CubeType::air:
            return std::make_shared<Air>();
        case CubeType::stone:
            return std::make_shared<Stone>();
        case CubeType::dirt:
            return std::make_shared<Dirt>();
        case CubeType::grassyDirt:
            return std::make_shared<GrassyDirt>();
        case CubeType::sand:
            return std::make_shared<Sand>();
    }
}