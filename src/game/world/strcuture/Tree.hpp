//
// Created by lonyelon on 31/7/20.
//

#ifndef OPENCRAFT_TREE_HPP
#define OPENCRAFT_TREE_HPP

#include <engine/position/FixedPosition.hpp>

class World;

class Tree {
private:
    World *world;
public:
    Tree(World *w);

    void construct(FixedPosition at);
};


#endif //OPENCRAFT_TREE_HPP
