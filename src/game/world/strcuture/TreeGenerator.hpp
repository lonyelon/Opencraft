//
// Created by lonyelon on 31/7/20.
//

#ifndef OPENCRAFT_TREEGENERATOR_HPP
#define OPENCRAFT_TREEGENERATOR_HPP

#include <game/world/strcuture/StructureGenerator.hpp>
#include <engine/position/Position.hpp>

class World;

class TreeGenerator : public StructureGenerator {
public:
    TreeGenerator(std::weak_ptr<World> w);

    void constructAt(Position<int> at);
};


#endif //OPENCRAFT_TREEGENERATOR_HPP
