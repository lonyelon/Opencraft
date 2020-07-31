//
// Created by lonyelon on 31/7/20.
//

#include "Tree.hpp"

#include <game/world/World.hpp>

Tree::Tree(World *w) {
    this->world = w;
}

void Tree::construct(FixedPosition at) {
    // trunk
    int height = 5 + rand() % 5;
    for (int i = 0; i < height; i++) {
        this->world->setCube(new Wood(), at.move(FixedPosition(0, i, 0)));
    }

    for (int i = height - 4; i < height; i++) {
        int wide = 1*(float)height/i;
        for (int j = -wide; j <= wide; j++) {
            for (int k = -wide; k <= wide; k++) {
                this->world->setCube(new Wood(), at.move(FixedPosition(j, i, k)));
            }
        }
    }
}