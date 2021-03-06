//
// Created by lonyelon on 31/7/20.
//

#include "TreeGenerator.hpp"

#include <game/world/World.hpp>

TreeGenerator::TreeGenerator(std::weak_ptr<World> w) : StructureGenerator(w) {
    this->world = w;
}

void TreeGenerator::constructAt(Position<int> at) {
    // trunk
    int height = 5 + rand() % 5;
    for (int i = 0; i < height; i++) {
        this->world.lock()->setCube(std::make_shared<Wood>(), at.move(Position(0, i, 0)));
    }

    for (int i = height - 4; i < height; i++) {
        int wide = 4;
        for (int j = -wide; j <= wide; j++) {
            for (int k = -wide; k <= wide; k++) {
                if (j == 0 && k == 0) {
                    continue;
                }
                this->world.lock()->setCube(std::make_shared<TreeLeaves>(), at.move(Position(j, i, k)));
            }
        }
    }
}