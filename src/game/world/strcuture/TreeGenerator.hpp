//
// Created by lonyelon on 31/7/20.
//

#ifndef OPENCRAFT_TREEGENERATOR_HPP
#define OPENCRAFT_TREEGENERATOR_HPP

#include <game/world/strcuture/StructureGenerator.hpp>
#include <engine/position/FixedPosition.hpp>

class World;

class TreeGenerator : public StructureGenerator {
private:
    World *world;
public:
    TreeGenerator(World *w);

    void constructAt(FixedPosition at);
};


#endif //OPENCRAFT_TREEGENERATOR_HPP
