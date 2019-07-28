#include "Hitbox.hpp"

#include "Player.hpp"

HitBox::HitBox( Player * p, float x, float y, float z, float w, float h, float d ) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->width = w;
    this->height = h;
    this->depth = d;
    this->player = p;
}

void HitBox::setPos( float x, float y, float z ) {
    this->x = x;
    this->x = y;
    this->x = z;
}

int HitBox::isColliding() {
    int where = 1;
    Cube *c;
        
    // Block collition
    //c = this->p->getWorld()->getCube( this->x + );

    return where;
};