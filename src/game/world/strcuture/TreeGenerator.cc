//
// Created by lonyelon on 31/7/20.
//

#include "TreeGenerator.hpp"

#include <game/world/World.hpp>

TreeGenerator::TreeGenerator(World *w) : StructureGenerator(w) {
    this->world = w;
}

void TreeGenerator::constructAt(FixedPosition at) {
    // trunk
    int height = 5 + rand() % 5;
    for (int i = 0; i < height; i++) {
        this->world->setCube(new Wood(), at.move(FixedPosition(0, i, 0)));
    }

    for (int i = height - 4; i < height; i++) {
        int wide = 4;
        for (int j = -wide; j <= wide; j++) {
            for (int k = -wide; k <= wide; k++) {
                if (j == 0 && k == 0) {
                    continue;
                }
                this->world->setCube(new TreeLeaves(), at.move(FixedPosition(j, i, k)));
            }
        }
    }
}