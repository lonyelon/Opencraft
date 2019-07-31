#include "Player.hpp"
#include "world/World.hpp"

#include <cstdio>

extern int pint;

Player::Player( World *world ) {
    this->cam = new Camera(this);
    this->cam->setPos( 0, 100, 0 );
    this->world = world;
    this->vspd = 0;

    this->setSprint( false );
}

void Player::move(float mx, float my, float mz) {
    const float top = 0.05;
    const float bot = 1.70;
    
    const float right = 0.25;
    const float left = 0.25;

    const float front = 0.25;
    const float back = 0.25;

    Cube *c;

    mx *= this->movementSpeed;
    my *= this->movementSpeed;
    mz *= this->movementSpeed;

    //? Can fall
    c = this->world->getCube( this->cam->getX() + right, this->cam->getY() - bot + my, this->cam->getZ() + front );
    if ( c != NULL && !isTransparent(c) && my < 0 ) { 
        my = 0; 
        this->vspd = 0;
        //this->cam->setPos(this->cam->getX(), c->getY() + 0.5 + bot, this->cam->getZ());
    }
    c = this->world->getCube( this->cam->getX() + right, this->cam->getY() - bot + my, this->cam->getZ() - back );
    if ( c != NULL && !isTransparent(c) && my < 0 ) { 
        my = 0; 
        this->vspd = 0;
        //this->cam->setPos(this->cam->getX(), c->getY() + 0.5 + bot, this->cam->getZ());
    }
    c = this->world->getCube( this->cam->getX() - left, this->cam->getY() - bot + my, this->cam->getZ() + front );
    if ( c != NULL && !isTransparent(c) && my < 0 ) { 
        my = 0; 
        this->vspd = 0;
        //this->cam->setPos(this->cam->getX(), c->getY() + 0.5 + bot, this->cam->getZ());
    }
    c = this->world->getCube( this->cam->getX() - left, this->cam->getY() - bot + my, this->cam->getZ() - back );
    if ( c != NULL && !isTransparent(c) && my < 0 ) { 
        my = 0; 
        this->vspd = 0;
        //this->cam->setPos(this->cam->getX(), c->getY() + 0.5 + bot, this->cam->getZ());
    }

    //? Positive Y
    c = this->world->getCube( this->cam->getX() + right, this->cam->getY() + top + my, this->cam->getZ() + front );
    if ( c != NULL && !isTransparent(c) && my > 0 ) { 
        my = 0; 
        if (this->vspd > 0) this->vspd = 0;
    }
    c = this->world->getCube( this->cam->getX() + right, this->cam->getY() + top + my, this->cam->getZ() + front );
    if ( c != NULL && !isTransparent(c) && my > 0 ) { 
        my = 0; 
        if (this->vspd > 0) this->vspd = 0;
    }
    c = this->world->getCube( this->cam->getX() - left, this->cam->getY() +top + my, this->cam->getZ() - back );
    if ( c != NULL && !isTransparent(c) && my > 0 ) { 
        my = 0; 
        if (this->vspd > 0) this->vspd = 0;
    }
    c = this->world->getCube( this->cam->getX() - left, this->cam->getY() + top + my, this->cam->getZ() - back );
    if ( c != NULL && !isTransparent(c) && my > 0 ) { 
        my = 0; 
        if (this->vspd > 0) this->vspd = 0;
    }

    //? Positive Z
    c = this->world->getCube( this->cam->getX() + right, this->cam->getY() - bot, this->cam->getZ() + front + mz );
    if ( c != NULL && !isTransparent(c) && mz > 0 ) { 
        mz = 0; 
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()-0.5-front);
    }
    c = this->world->getCube( this->cam->getX() - left, this->cam->getY() - bot, this->cam->getZ() + front + mz );
    if ( c != NULL && !isTransparent(c) && mz > 0 ) { 
        mz = 0; 
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()-0.5-front);
    }
    c = this->world->getCube( this->cam->getX() + right, this->cam->getY() + top, this->cam->getZ() + front + mz );
    if ( c != NULL && !isTransparent(c) && mz > 0 ) { 
        mz = 0; 
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()-0.5-front);
    }
    c = this->world->getCube( this->cam->getX() - left, this->cam->getY() + top, this->cam->getZ() + front + mz );
    if ( c != NULL && !isTransparent(c) && mz > 0 ) { 
        mz = 0; 
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()-0.5-front);
    }

    //? Negative Z
    c = this->world->getCube( this->cam->getX() + right, this->cam->getY() - bot, this->cam->getZ() - back + mz );
    if ( c != NULL && !isTransparent(c) && mz < 0 ) { 
        mz = 0; 
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()+0.5+back);
    }
    c = this->world->getCube( this->cam->getX() - left, this->cam->getY() - bot, this->cam->getZ() - back + mz );
    if ( c != NULL && !isTransparent(c) && mz < 0 ) { 
        mz = 0; 
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()+0.5+back);
    }
    c = this->world->getCube( this->cam->getX() + right, this->cam->getY() + top, this->cam->getZ() - back + mz );
    if ( c != NULL && !isTransparent(c) && mz < 0 ) { 
        mz = 0; 
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()+0.5+back);
    }
    c = this->world->getCube( this->cam->getX() - left, this->cam->getY() + top, this->cam->getZ() - back + mz );
    if ( c != NULL && !isTransparent(c) && mz < 0 ) { 
        mz = 0; 
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()+0.5+back);
    }

    //? Positive X
    c = this->world->getCube( this->cam->getX() + right + mx, this->cam->getY() - bot, this->cam->getZ() + front );
    if ( c != NULL && !isTransparent(c) && mx > 0 ) { 
        mx = 0; 
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world->getCube( this->cam->getX() + right + mx, this->cam->getY() - bot, this->cam->getZ() - back );
    if ( c != NULL && !isTransparent(c) && mx > 0 ) { 
        mx = 0; 
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world->getCube( this->cam->getX() + right + mx, this->cam->getY() + top, this->cam->getZ() + front );
    if ( c != NULL && !isTransparent(c) && mx > 0 ) { 
        mx = 0; 
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world->getCube( this->cam->getX() + right + mx, this->cam->getY() + top, this->cam->getZ() - back );
    if ( c != NULL && !isTransparent(c) && mx > 0 ) { 
        mx = 0; 
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }

    //? Negative X
    c = this->world->getCube( this->cam->getX() - left + mx, this->cam->getY() - bot, this->cam->getZ() + front );
    if ( c != NULL && !isTransparent(c) && mx < 0 ) { 
        mx = 0; 
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world->getCube( this->cam->getX() - left + mx, this->cam->getY() - bot, this->cam->getZ() - back );
    if ( c != NULL && !isTransparent(c) && mx < 0 ) { 
        mx = 0; 
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world->getCube( this->cam->getX() - left + mx, this->cam->getY() + top, this->cam->getZ() + front );
    if ( c != NULL && !isTransparent(c) && mx < 0 ) { 
        mx = 0; 
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world->getCube( this->cam->getX() - left + mx, this->cam->getY() + top, this->cam->getZ() - back );
    if ( c != NULL && !isTransparent(c) && mx < 0 ) { 
        mx = 0; 
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    
    this->cam->move(mx, my, mz);
}

Cube *Player::getPointer( float *x, float *y, float *z ) {
    const int iter = 1000;
    const int maxdist = 8;

    float rx = this->cam->getRotX();
    float ry = this->cam->getRotY();

    for (int i = 0; i < iter*maxdist; i++) {
        int xx = round(this->cam->getX()-i*cos(rx)*cos(ry)/iter); 
        int yy = round(this->cam->getY()+i*sin(ry)/iter); 
        int zz = round(this->cam->getZ()+i*sin(rx)*cos(ry)/iter);
        
        Cube *c = this->world->getCube(xx, yy, zz);
        if (c != NULL && c->getType() != CubeType::air) {
            *x = this->cam->getX()-(i-1)*cos(rx)*cos(ry)/iter;
            *y = this->cam->getY()+(i-1)*sin(ry)/iter;
            *z = this->cam->getZ()+(i-1)*sin(rx)*cos(ry)/iter;

            return c;
        }
    }
    return NULL;
}

Cube *Player::getPointedCube() {
    float x, y, z;
    return this->getPointer( &x, &y, &z );
}

bool Player::getPointedPosition( float *x, float *y, float *z ) {
    if (this->getPointer( x, y, z ) == NULL) {
        return false;
    }
    return true;
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

void Player::placeCube() {
    Cube *c = this->getPointedCube();
    if (c == NULL) {
        return;
    }

    float x, y, z;
    if (this->getPointedPosition(&x, &y, &z) == 0) {
        return;
    }

    Cube *c0 = this->world->getCube(x, y, z);
    if (c0 == NULL) {
        return;
    }

    pint = 1;
    c0->setType(CubeType::stone);
    c0->getChunk()->getVisibleCubes();
    printf("Chunk[%d, %d, %d]\tBlock[%d, %d, %d]\n", 
        c0->getChunk()->getX(), c0->getChunk()->getY(), c0->getChunk()->getZ(), c0->getX(), c0->getY(), c0->getZ());
    c0->getChunk()->genVao();
    pint = 0;
}

Camera *Player::getCam() {
    return this->cam;
}

void Player::setSprint(bool sprint) {
    float sprintSpeed = 0.125f;
    float normalSpeed = 0.07f;

    if (sprint) {
        if (this->vspd == 0 && this->movementSpeed < sprintSpeed) {
            this->movementSpeed = sprintSpeed + 0.00001f;
        }
    } else {
        this->movementSpeed = normalSpeed;
    }
}

void Player::gravity(float ammount) {
    if (this->vspd > -8) 
        this->vspd -= ammount;

    this->move(0, this->vspd, 0);
}

void Player::jump() {
    if (this->vspd == 0)
        this->vspd = 2;
    Cube *c = this->world->getCube(-80, 127, 54);
    if (c != NULL) {
        printf("%d\n", isTransparent(c));
    }
}