#include "Player.hpp"
#include "world/World.hpp"

#include <cstdio>

Player::Player(  ) {
    this->cam = NULL;
    this->world = NULL;
}

Player::Player( World *world ) {
    this->cam = new Camera();
    this->cam->setPos( 0, 100, 0 );
    this->world = world;
}

void Player::move(  ) {
    float hboxSize = 0.3;
    Cube *c0 = this->world->getCube( NULL, (int)(this->cam->getX() + hboxSize), (int)this->cam->getY()-1, (int)(this->cam->getZ() + hboxSize) );
    Cube *c1 = this->world->getCube( NULL, (int)(this->cam->getX() - hboxSize), (int)this->cam->getY()-1, (int)(this->cam->getZ() + hboxSize) );
    Cube *c2 = this->world->getCube( NULL, (int)(this->cam->getX() + hboxSize), (int)this->cam->getY()-1, (int)(this->cam->getZ() - hboxSize) );
    Cube *c3 = this->world->getCube( NULL, (int)(this->cam->getX() - hboxSize), (int)this->cam->getY()-1, (int)(this->cam->getZ() - hboxSize) );

    this->vspd -= 0.01;

    if ( c0 != NULL && (float)c0->getY() - this->cam->getY() <= 0.1 && !isTransparent(c0) ) {
        this->vspd = 0;
    }

    if ( c1 != NULL && (float)c1->getY() - this->cam->getY() <= 0.1 && !isTransparent(c1) ) {
        this->vspd = 0;
    }

    if ( c2 != NULL && (float)c2->getY() - this->cam->getY() <= 0.1 && !isTransparent(c2) ) {
        this->vspd = 0;
    }

    if ( c3 != NULL && (float)c3->getY() - this->cam->getY() <= 0.1 && !isTransparent(c3) ) {
        this->vspd = 0;
    }

    this->cam->move( 0, vspd, 0 );
}

Camera *Player::getCam() {
    return this->cam;
}