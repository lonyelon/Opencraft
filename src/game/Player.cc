#include "Player.hpp"
#include "world/World.hpp"

#include <cstdio>

Player::Player(  ) {
    this->cam = NULL;
    this->world = NULL;
    this->vspd = 0;
}

Player::Player( World *world ) {
    this->cam = new Camera();
    this->cam->setPos( 0, 100, 0 );
    this->world = world;
    this->vspd = 0;
}

void Player::move(  ) {
    /*float hboxSize = 0.6;
    Cube *c0 = this->world->getCube( NULL, (int)(this->cam->getX() + hboxSize), (int)this->cam->getY()-1, (int)(this->cam->getZ() + hboxSize) );
    Cube *c1 = this->world->getCube( NULL, (int)(this->cam->getX() - hboxSize), (int)this->cam->getY()-1, (int)(this->cam->getZ() + hboxSize) );
    Cube *c2 = this->world->getCube( NULL, (int)(this->cam->getX() + hboxSize), (int)this->cam->getY()-1, (int)(this->cam->getZ() - hboxSize) );
    Cube *c3 = this->world->getCube( NULL, (int)(this->cam->getX() - hboxSize), (int)this->cam->getY()-1, (int)(this->cam->getZ() - hboxSize) );

    Cube *c4 = this->world->getCube( NULL, (int)(this->cam->getX() + hboxSize), (int)this->cam->getY()+1, (int)(this->cam->getZ() + hboxSize) );
    Cube *c5 = this->world->getCube( NULL, (int)(this->cam->getX() - hboxSize), (int)this->cam->getY()+1, (int)(this->cam->getZ() + hboxSize) );
    Cube *c6 = this->world->getCube( NULL, (int)(this->cam->getX() + hboxSize), (int)this->cam->getY()+1, (int)(this->cam->getZ() - hboxSize) );
    Cube *c7 = this->world->getCube( NULL, (int)(this->cam->getX() - hboxSize), (int)this->cam->getY()+1, (int)(this->cam->getZ() - hboxSize) );

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

    if ( c4 != NULL && (float)c4->getY() - this->cam->getY() <= 0.1 && !isTransparent(c4) ) {
        this->vspd = 0;
        this->cam->move( 0, -0.1, 0 );
    }

    if ( c5 != NULL && (float)c5->getY() - this->cam->getY() <= 0.1 && !isTransparent(c5) ) {
        this->vspd = 0;
        this->cam->move( 0, -0.1, 0 );
    }

    if ( c6 != NULL && (float)c6->getY() - this->cam->getY() <= 0.1 && !isTransparent(c6) ) {
        this->vspd = 0;
        this->cam->move( 0, -0.1, 0 );
    }

    if ( c7 != NULL && (float)c7->getY() - this->cam->getY() <= 0.1 && !isTransparent(c7) ) {
        this->vspd = 0;
        this->cam->move( 0, -0.1, 0 );
    }

    this->cam->move( 0, vspd, 0 );*/
}

Camera *Player::getCam() {
    return this->cam;
}