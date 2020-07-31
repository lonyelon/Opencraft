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
    if (!this->chunk->getWorld()->getCube(this->getPos().move(FixedPosition(0,1,0)))->isTransparent()) {
        this->chunk->setCube(new Dirt(), this->getPos());
    }
};