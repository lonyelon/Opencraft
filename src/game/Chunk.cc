#include "Chunk.hpp"

#include <noise/noise.h>

#include <stdio.h>
#include <random>
#include <cmath>

extern int seed;
extern GLuint shaderProgram;

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
                if (y < (int)((p.GetValue((float)(this->x*this->W + x)/200, (float)(this->y*this->H + y)/200, (float)(this->z*this->Z + z)/200)+1)*50)) {
                    this->getCube(x, y, z)->setType(CubeType::dirt);
                } else if (this->H*this->y + y < 50) {
                    this->getCube(x, y, z)->setType(CubeType::water);
                }
            }
        }
    }

    this->getVisibleCubes();
    this->genVao();
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
    /*
    if (x != this->W*this->x + this->W - 1) {
        if (this->getCube(x - this->W*this->x+1,y - this->H*this->y,z - this->Z*this->z)->getType() == CubeType::water) {
            return false;
        }
    }
    if (x != this->W*this->x) if (this->getCube(x - this->W*this->x-1,y - this->H*this->y,z - this->Z*this->z)->getType() == CubeType::water) {
        return false;
    }

    if (y != this->H*this->y + this->H - 1) if (this->getCube(x - this->W*this->x,y - this->H*this->y+1,z - this->Z*this->z)->getType() == CubeType::water) {
        return false;
    }

    if (y != this->H*this->y) if (this->getCube(x - this->W*this->x,y - this->H*this->y-1,z - this->Z*this->z)->getType() == CubeType::water) {
        return false;
    }

    if (z != this->Z*this->z + this->Z - 1) if (this->getCube(x - this->W*this->x,y - this->H*this->y,z - this->Z*this->z+1)->getType() == CubeType::water) {
        return false;
    }

    if (z != this->Z*this->z) if (this->getCube(x - this->W*this->x,y - this->H*this->y,z - this->Z*this->z-1)->getType() == CubeType::water) {
        return false;
    }*/
    
    return true;
}

void Chunk::getVisibleCubes() {
    if (this->renderedCubes.size() == 0) {
        for (int i = 0; i < this->W*this->H*this->Z; i++) {
            if ( this->isIllated(this->cubes[i].getX(), this->cubes[i].getY(), this->cubes[i].getZ()) ) {
                continue;
            }

            if ( this->cubes[i].getType() != CubeType::air ) {
                this->renderedCubes.push_back( &(this->cubes[i]) );
            }
        }
        return;
    }
}

void Chunk::genVao() {
    unsigned int VBO, EBO;

    std::vector<float> v;
    std::vector<int> i;

    for ( int k = 0; k < this->renderedCubes.size(); k++ ) {
        this->renderedCubes[k]->getVertex( &v, &i, k );
    }

    printf( "Vertices: %d\t%d\t%d\n\n", v.size(), v.size()/8/6, this->renderedCubes.size() );

    float *vertices = &v[0];
    int *indices = &i[0];

	glGenVertexArrays( 1, &(this->VAO) );
	glGenBuffers( 1, &VBO );
	glGenBuffers( 1, &EBO );

	glBindVertexArray( this->VAO );

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof( float ) * v.size(), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof( float ) * i.size(), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Chunk::draw() {
    glBindVertexArray( this->VAO );
    
    glDrawElements(GL_TRIANGLES, 36*this->renderedCubes.size(), GL_UNSIGNED_INT, 0);
}