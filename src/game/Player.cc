#include "Player.hpp"
#include "world/World.hpp"

#include <cstdio>

Player::Player( World *world ) {
    this->cam = new Camera(this);
    this->cam->setPos( 0, 100, 0 );
    this->world = world;
    this->vspd = 0;

    this->setSprint( false );
}

void Player::move() {
}

Cube *Player::getPointedCube() {
    const int iter = 1000;
    const int maxdist = 8;

    float rx = this->cam->getRotX();
    float ry = this->cam->getRotY();

    for (int i = 0; i < iter*maxdist; i++) {
        int x = round(this->cam->getX()-i*cos(rx)*cos(ry)/iter); 
        int y = round(this->cam->getY()+i*sin(ry)/iter); 
        int z = round(this->cam->getZ()+i*sin(rx)*cos(ry)/iter);
        
        Cube *c = this->world->getCube(x, y, z);
        if (c != NULL && c->getType() != CubeType::air) {
            return c;
        }
    }
    return NULL;
}

void Player::breakCube() {
    Cube *c = this->getPointedCube();
    if (c == NULL) {
        return;
    }

    c->setType(CubeType::air);
    c->getChunk()->getVisibleCubes();
    c->getChunk()->genVao();
}

Camera *Player::getCam() {
    return this->cam;
}

void Player::setSprint(bool sprint) {
    float sprintSpeed = 0.8f;
    float normalSpeed = 0.1f;
    printf("Set!\n");

    if (sprint == true) {
        this->movementSpeed = sprintSpeed;
    } else {
        this->movementSpeed = normalSpeed;
    }
}