//
// Created by lonyelon on 31/7/20.
//

#include "GrassyDirt.hpp"

#include <game/world/World.hpp>

GrassyDirt::GrassyDirt() : Cube() {
    this->transparent = false;
    this->type = CubeType::grassyDirt;
}

void GrassyDirt::update() {
    if (!this->chunk->getWorld()->get_cube(this->position + Position<int>(0, 1, 0))->isTransparent()) {
        this->chunk->setCube(std::make_shared<Dirt>(), this->position);
    }
};
