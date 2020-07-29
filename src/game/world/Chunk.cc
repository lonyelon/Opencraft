#include "Chunk.hpp"
#include "World.hpp"
#include "cube/Cubes.hpp"
#include "../../engine/FixedPosition.hpp"

#include <noise/noise.h>

#include <stdio.h>
#include <random>
#include <cmath>
#include <vector>

extern GLuint shaderProgram;
extern int pint;

Chunk::Chunk ( World *w, int xpos, int ypos, int zpos ) {
    this->world = w;
    this->x = xpos;
    this->y = ypos;
    this->z = zpos;

    this->generated = false;

    this->cubes = std::vector<Cube*>(this->W*this->H*this->Z);
    this->VAO = 0;
}

void Chunk::genTerrain() {
    const float xCoordRed = 300;
    const float yCoordRed = 300;
    const float zCoordRed = 300;
    const float noiseFreq = 1;
     float ystrech = 60;
    const int heightIncrease = 60;

    const int waterHeight = 64+heightIncrease;

    if ( this->generated == true ) {
        return;
    }
    this->generated = true;

    noise::module::Perlin p;

    int heights[this->W*this->Z];
    for (int i = 0; i < this->W*this->Z; i++) {
        heights[i] = -1;
    }

    p.SetSeed( this->world->getSeed(  ) );
    p.SetFrequency( noiseFreq );

    // Generate stone world and water

    for (int y = 0; y < this->H; y++) {
        for (int x = 0; x < this->W; x++) {
            for (int z = 0; z < this->Z; z++) {
                float noiseX = (float)(this->x*this->W + x)/xCoordRed;
                float noiseY = (float)(this->y*this->H + y)/yCoordRed;
                float noiseZ = (float)(this->z*this->Z + z)/zCoordRed;

                int noiseValue = (int)((p.GetValue(noiseX, noiseY, noiseZ) + 1.1)*ystrech)+heightIncrease;

                if ( y < noiseValue ) {
                    this->setCube(new Stone(), x, y, z);
                } else {
					this->setCube(new Air(), x, y, z);
				}
            }
        }
    }

    p.SetSeed( this->world->getSeed(  )*2 );

    for (int x = 0; x < this->W; x++) {
        for (int z = 0; z < this->Z; z++) {
            int dirtCount = 0;
            for (int y = this->H-1; y > 0; y--) {
                Cube *c = this->getCube( x, y, z );

                float noiseX = (float)(this->x*this->W + x)/xCoordRed*10;
                float noiseZ = (float)(this->z*this->Z + z)/zCoordRed*10;

                if ( c->getType() == CubeType::air ) {
                    if ( y < waterHeight ) {
                        this->setCube(new Water(), x, y, z);
                    } else {
                        dirtCount = 0;
                    }
                } else {
                    if (dirtCount < 4) {
                        if ( y < heights[x + z*this->W] || heights[x + z*this->W] == -1 ) {
                            heights[x + z*this->W] = y;
                        }
                        if ( (p.GetValue(noiseX, 1, noiseZ)*5 + waterHeight)/y > 1) {
                            this->setCube(new Sand(), x, y, z);
                        } else if (dirtCount == 0 && y >= waterHeight-1) {
                            this->setCube(new GrassyDirt(), x, y, z);
                        } else {
                            this->setCube(new Dirt(), x, y, z);
                        }
                        dirtCount++;
                    }
                }
            }
        }
    }

    noise::module::Perlin caveNoise;

    float caveFreq = 1;
    int caveDistance = 20;
    float caveProb = -0.8;

    caveNoise.SetSeed( this->world->getSeed(  )*3 );
    caveNoise.SetFrequency(caveFreq);
    caveNoise.SetLacunarity(3.5);

    for (int x = 0; x < this->W; x++) {
        for (int z = 0; z < this->Z; z++) {
            int dirtCount = 0;
            for (int y = this->H-1; y > 0; y--) {
                Cube *c = this->getCube( x, y, z );

                float noiseX = (float)(this->x*this->W + x)/xCoordRed*10;
                float noiseY = (float)(this->y*this->H + y)/yCoordRed*10;
                float noiseZ = (float)(this->z*this->Z + z)/zCoordRed*10;

                if ( (caveNoise.GetValue( noiseX, noiseY, noiseZ )*( heights[x + z*this->W] - caveDistance ) )/y < caveProb && c->getType() != CubeType::water ) {
                    this->setCube(new Air(), z, y, z);
                }
            }
        }
    }

    for (int x = 0; x < this->W; x++) {
        for (int z = 0; z < this->Z; z++) {
            int dirtCount = 0;
            for (int y = this->H-1; y > 0; y--) {
                Cube *c = this->getCube( x, y, z );

                if ( y < 12 && c->getType() == CubeType::air ) {
                    this->setCube(new Lava(), x, y, z);
                }
            }
        }
    }
}

Cube *Chunk::getCube(unsigned int x, int y, int z) {
    if ( x + y*this->W + z*this->W*this->H > this->W*this->H*this->Z ) {
        return NULL;
    }
    return this->cubes[x + y*this->W + z*this->W*this->H];
    //return NULL;
}

void Chunk::setCube(Cube *c, int x, int y, int z) {

	/*if (x < this->W && y < this->H && z < this->Z) {
		return;
	}*/

	c->setX(x + this->x*this->W);
	c->setY(y + this->y*this->H);
	c->setZ(z + this->z*this->Z);

	FixedPosition chunkPos;
	chunkPos.x = x;
	chunkPos.y = y;
	chunkPos.z = z;
	c->setChunkPos(chunkPos);

	c->setChunk(this);

    this->cubes[x + y*this->W + z*this->W*this->H] = c;
}


std::vector<Cube*> Chunk::getCubes() {
    return this->cubes;
    //return NULL;
}

int Chunk::isIllated(int x, int y, int z) {
    Cube *c = NULL;
    Cube *k = this->world->getCube( this, x, y, z );
    int n = 1;

    if ( k == NULL ) {
        return 1;
    }

    c = this->world->getCube(this, x ,y + 1 ,z );
    if ( c != NULL && isTransparent( c ) && k->getType() != c->getType() ) {
        n *= 2;
    }

    c = this->world->getCube(this, x, y - 1 ,z );
    if ( c != NULL && isTransparent( c ) && k->getType() != c->getType() ) {
        n *= 3;
    }

    c = this->world->getCube(this, x + 1,y ,z );
    if ( c != NULL && isTransparent( c ) && k->getType() != c->getType() ) {
        n *= 5;
    }

    c = this->world->getCube(this, x - 1,y ,z );
    if ( c != NULL && isTransparent( c ) && k->getType() != c->getType() ) {
        n *= 7;
    }

    c = this->world->getCube(this, x,y ,z + 1 );
    if ( c != NULL && isTransparent( c ) && k->getType() != c->getType() ) {
        n *= 11;
    }

    c = this->world->getCube(this, x ,y ,z - 1 );
    if ( c != NULL && isTransparent( c ) && k->getType() != c->getType() ) {
        n *= 13;
    }

    k->setSides(n);
    return n;
}

void Chunk::getVisibleCubes() {
    if (this->renderedCubes.size() != 0) {
        this->renderedCubes.clear();
    }

    this->world->genChunkAt(false, this->x + 1, this->y, this->z);
    this->world->genChunkAt(false, this->x - 1, this->y, this->z);
    this->world->genChunkAt(false, this->x, this->y, this->z + 1);
    this->world->genChunkAt(false, this->x, this->y, this->z - 1);

    for ( int i = 0; i < this->W*this->H*this->Z; i++ ) {
        if ( this->cubes[i]->getType(  ) != CubeType::air ) {
            if ( this->isIllated(this->cubes[i]->getX(), this->cubes[i]->getY(), this->cubes[i]->getZ()) == 1  ) {
                //delete(this->cubes[i]);
                continue;
            }

            this->renderedCubes.push_back( this->cubes[i] );
        }
    }
    return;
}

void Chunk::genVao() {
    if ( this->VAO != 0 ) {
        glDeleteVertexArrays(1, &(this->VAO));
    }

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Chunk::draw() {
    if ( this->VAO == 0 ) return;


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
