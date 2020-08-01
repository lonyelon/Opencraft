//
// Created by lonyelon on 31/7/20.
//

#ifndef OPENCRAFT_STRUCTUREGENERATOR_HPP
#define OPENCRAFT_STRUCTUREGENERATOR_HPP

#include <engine/position/FixedPosition.hpp>

class World;

class StructureGenerator {
private:
    World *world;
public:
    StructureGenerator(World *w);

    virtual void constructAt(FixedPosition at) = 0;
};


#endif //OPENCRAFT_STRUCTUREGENERATOR_HPP
