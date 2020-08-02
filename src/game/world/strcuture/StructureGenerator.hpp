//
// Created by lonyelon on 31/7/20.
//

#ifndef OPENCRAFT_STRUCTUREGENERATOR_HPP
#define OPENCRAFT_STRUCTUREGENERATOR_HPP

#include <engine/position/Position.hpp>

#include <memory>

class World;

class StructureGenerator {
protected:
    std::weak_ptr<World> world;
public:
    StructureGenerator(std::weak_ptr<World> w);

    virtual void constructAt(Position<int> at) = 0;
};


#endif //OPENCRAFT_STRUCTUREGENERATOR_HPP
