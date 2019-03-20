#include "Chunk.hpp"

#include <noise/noise.h>

#include <stdio.h>
#include <random>
#include <cmath>

extern int seed;

Chunk::Chunk ( int xpos, int ypos, int zpos ) {
    //this->world = w;
    this->x = xpos;
    this->y = ypos;
    this->z = zpos;
    this->cubes = new Cube[this->W * this->H * this->Z];
}

void Chunk::genTerrain() {
    noise::module::Perlin p;
    p.SetSeed(seed);
    p.SetFrequency(1);
    for (int y = 0; y < this->H; y++) {
        for (int x = 0; x < this->W; x++) {
            for (int z = 0; z < this->Z; z++) {
                this->getCube(x, y, z)->setPosition(this->x*this->W + x, this->H*this->y + y, this->z*this->Z + z);
                if (y < (int)((p.GetValue((float)(this->x*this->W + x)/200, 0.5, (float)(this->z*this->Z + z)/200)+1)*50)) {
                    this->getCube(x, y, z)->setType(CubeType::dirt);
                } else if (this->H*this->y + y < 50) {
                    this->getCube(x, y, z)->setType(CubeType::water);
                }
            }
        }
    }
}

Cube *Chunk::getCube(int x, int y, int z) {
    return &(this->cubes[x + y*this->W + z*this->W*this->H]);
    //return NULL;
}

Cube *Chunk::getCubes() {
    return this->cubes;
    //return NULL;
}

bool Chunk::isIllated(int x, int y, int z) {
    if (x != this->W*this->x + this->W - 1) {
        if (this->getCube(x - this->W*this->x+1,y - this->H*this->y,z - this->Z*this->z)->getType() == CubeType::air) {
            return false;
        }
    }
    if (x != this->W*this->x) if (this->getCube(x - this->W*this->x-1,y - this->H*this->y,z - this->Z*this->z)->getType() == CubeType::air) {
        return false;
    }

    if (y != this->H*this->y + this->H - 1) if (this->getCube(x - this->W*this->x,y - this->H*this->y+1,z - this->Z*this->z)->getType() == CubeType::air) {
        return false;
    }

    if (y != this->H*this->y) if (this->getCube(x - this->W*this->x,y - this->H*this->y-1,z - this->Z*this->z)->getType() == CubeType::air) {
        return false;
    }

    if (z != this->Z*this->z + this->Z - 1) if (this->getCube(x - this->W*this->x,y - this->H*this->y,z - this->Z*this->z+1)->getType() == CubeType::air) {
        return false;
    }

    if (z != this->Z*this->z) if (this->getCube(x - this->W*this->x,y - this->H*this->y,z - this->Z*this->z-1)->getType() == CubeType::air) {
        return false;
    }
    
    return true;
}

void Chunk::draw() {
    if (this->renderedCubes.size() == 0) {
        for (int i = 0; i < this->W*this->H*this->Z; i++) {
            if ( this->isIllated(this->cubes[i].getX(), this->cubes[i].getY(), this->cubes[i].getZ()) ) {
                continue;
            }

            if ( this->cubes[i].draw() == 0 ) {
                this->renderedCubes.push_back( &(this->cubes[i]) );
            }
        }
        return;
    }

    for ( int i = 0; i < this->renderedCubes.size(); i++ ) {
        this->renderedCubes[i]->draw();
    }
}