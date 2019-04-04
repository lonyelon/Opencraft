#include "Chunk.hpp"
#include "World.hpp"

#include <noise/noise.h>

#include <stdio.h>
#include <random>
#include <cmath>
#include <vector>

extern int seed;
extern GLuint shaderProgram;

Chunk::Chunk ( World *w, int xpos, int ypos, int zpos ) {
    this->world = w;
    this->x = xpos;
    this->y = ypos;
    this->z = zpos;

    this->cubes = std::vector<Cube*>(this->W*this->H*this->Z);
    
    this->VAO = 0;
}

void Chunk::genTerrain() {
    const float xCoordRed = 200;
    const float yCoordRed = 200;
    const float zCoordRed = 200;
    const float ystrech = 50;

    const int waterHeight = 63;

    noise::module::Perlin p;


    p.SetSeed( this->world->getSeed(  ) );
    p.SetFrequency(1);

    for (int y = 0; y < this->H; y++) {
        for (int x = 0; x < this->W; x++) {
            for (int z = 0; z < this->Z; z++) {
                float noiseX = (float)(this->x*this->W + x)/xCoordRed;
                float noiseY = (float)(this->y*this->H + y)/yCoordRed;
                float noiseZ = (float)(this->z*this->Z + z)/zCoordRed;

                int noiseValue = (int)((p.GetValue(noiseX, noiseY, noiseZ) + 1.1)*ystrech);
                
                this->cubes[x + y*this->W + z*this->W*this->H] = new Cube( this, this->x*this->W + x, this->H*this->y + y, this->z*this->Z + z );

                if ( y < noiseValue ) {
                    this->getCube( x, y, z )->setType( CubeType::stone );
                }
            }
        }
    }
    
    for (int x = 0; x < this->W; x++) {
        for (int z = 0; z < this->Z; z++) {
            int dirtCount = 0;
            for (int y = this->H-1; y > 0; y--) {    
                Cube *c = this->getCube( x, y, z );

                if ( c->getType() == CubeType::air ) {
                    if ( y < waterHeight ) {
                        c->setType(CubeType::water);
                    } else {
                        dirtCount = 0;
                    }
                } else {
                    if (dirtCount < 3) {
                        if (dirtCount == 0 && y >= waterHeight-1) {
                            c->setType(CubeType::grassyDirt);
                        } else {
                            c->setType(CubeType::dirt);
                        }
                        dirtCount++;
                    }
                }
            }
        }
    }
}

Cube *Chunk::getCube(int x, int y, int z) {
    return this->cubes[x + y*this->W + z*this->W*this->H];
    //return NULL;
}

std::vector<Cube*> Chunk::getCubes() {
    return this->cubes;
    //return NULL;
}

bool Chunk::isIllated(int x, int y, int z) {
    Cube *c = NULL;
    
    c = this->world->getCube(x + 1,y ,z );
    if ( c != NULL && c->getType() == CubeType::air) {
        return false;
    }

    c = this->world->getCube(x - 1,y ,z );
    if ( c != NULL && c->getType() == CubeType::air) {
        return false;
    }

    c = this->world->getCube(x ,y + 1 ,z );
    if ( c != NULL && c->getType() == CubeType::air) {
        return false;
    }

    c = this->world->getCube(x, y - 1 ,z );
    if ( c != NULL && c->getType() == CubeType::air) {
        return false;
    }

    c = this->world->getCube(x,y ,z + 1 );
    if ( c != NULL && c->getType() == CubeType::air) {
        return false;
    }

    c = this->world->getCube(x ,y ,z - 1 );
    if ( c != NULL && c->getType() == CubeType::air) {
        return false;
    }

    c = this->world->getCube( x, y ,z );

    if ( c != NULL && c->getType() != CubeType::water ) {
        c = this->world->getCube(x + 1,y ,z );
        if ( c != NULL && c->getType() == CubeType::water) {
            return false;
        }

        c = this->world->getCube(x - 1,y ,z );
        if ( c != NULL && c->getType() == CubeType::water) {
            return false;
        }

        c = this->world->getCube(x ,y + 1 ,z );
        if ( c != NULL && c->getType() == CubeType::water) {
            return false;
        }

        c = this->world->getCube(x, y - 1 ,z );
        if ( c != NULL && c->getType() == CubeType::water) {
            return false;
        }

        c = this->world->getCube(x,y ,z + 1 );
        if ( c != NULL && c->getType() == CubeType::water) {
            return false;
        }

        c = this->world->getCube(x ,y ,z - 1 );
        if ( c != NULL && c->getType() == CubeType::water) {
            return false;
        }
    }

    return true;
}

void Chunk::getVisibleCubes() {
    if (this->renderedCubes.size() == 0) {
        for (int i = 0; i < this->W*this->H*this->Z; i++) {
            if ( this->isIllated(this->cubes[i]->getX(), this->cubes[i]->getY(), this->cubes[i]->getZ()) ) {
                //delete(&(this->cubes[i]));
                continue;
            }

            if ( this->cubes[i]->getType() != CubeType::air ) {
                this->renderedCubes.push_back( this->cubes[i] );
            }
        }
        return;
    }
}

void Chunk::genVao() {
    if ( this->VAO != 0 ) return;

    unsigned int VBO, EBO;

    std::vector<float> v;
    std::vector<int> i;

    for ( int k = 0; k < this->renderedCubes.size(); k++ ) {
        this->renderedCubes[k]->getVertex( &v, &i, k );
    }

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

Chunk::~Chunk() {
    glDeleteVertexArrays(1, &(this->VAO));
    for (int i = 0; i < this->W*this->H*this->Z; i++) {
        delete(this->cubes[i]);
    }
    this->cubes.clear();
    this->renderedCubes.clear();
}