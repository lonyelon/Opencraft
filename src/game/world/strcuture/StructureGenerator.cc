//
// Created by lonyelon on 31/7/20.
//

#include "StructureGenerator.hpp"

#include <memory>

StructureGenerator::StructureGenerator(std::weak_ptr<World> w) {
    this->world = w;
}