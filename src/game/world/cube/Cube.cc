#include "Cube.hpp"

#include <cstdio>
#include <cstdlib>

extern GLuint shaderProgram;

Cube::Cube( Chunk *c, int xpos, int ypos, int zpos ) {
    Cube::x = xpos;
    Cube::y = ypos;
    Cube::z = zpos;
    Cube::type = CubeType::air;
    Cube::chunk = c;
    this->sides = 1;
}

Cube::Cube(  ) {
    Cube::x = 0;
    Cube::y = 0;
    Cube::z = 0;
    Cube::type = CubeType::air;
    Cube::chunk = NULL;
    this->sides = 1;
}

void Cube::setPosition( int x, int y, int z ) {
    Cube::x = x;
    Cube::y = y;
    Cube::z = z;
}

void Cube::setType(CubeType t) {
    Cube::type=t;
}

CubeType Cube::getType() {
    return Cube::type;
}

void Cube::getVertex(std::vector<float> *v, std::vector<int> *i, int n) {
    const int texFileSize = 256/32;

/*
      5--6
    0--1 -
    - 4--7
    2--3

*/

    float vertices[  ] = {
        // Front
        -0.5f, -0.5f, 0.5f, // 2
		0.5f, -0.5f, 0.5f, // 3
		0.5f, 0.5f, 0.5f, // 1
        -0.5f, -0.5f, 0.5f, // 2
        0.5f, 0.5f, 0.5f, // 1
		-0.5f, 0.5f, 0.5f, // 0

        // Top
        -0.5f, 0.5f, 0.5f, // 0
        0.5f, 0.5f, 0.5f, // 1
        -0.5f, 0.5f, -0.5f, // 5
        -0.5f, 0.5f, -0.5f, // 5
        0.5f, 0.5f, 0.5f, // 1
        0.5f, 0.5f, -0.5f, // 6

        // Left
        0.5f, -0.5f, 0.5f, // 3
        0.5f, 0.5f, -0.5f, // 6
        0.5f, 0.5f, 0.5f, // 1
        0.5f, -0.5f, 0.5f, // 3
        0.5f, -0.5f, -0.5f, // 7
        0.5f, 0.5f, -0.5f, // 6

        // Right
        -0.5f, -0.5f, 0.5f, // 2
        -0.5f, 0.5f, 0.5f, // 0
		-0.5f, 0.5f, -0.5f, // 5
        -0.5f, -0.5f, 0.5f, // 2
        -0.5f, 0.5f, -0.5f, // 5
        -0.5f, -0.5f, -0.5f, // 4

        // Bot
        0.5f, -0.5f, 0.5f, // 3
        -0.5f, -0.5f, 0.5f, // 2
        -0.5f, -0.5f, -0.5f, // 4
        0.5f, -0.5f, 0.5f, // 3
        -0.5f, -0.5f, -0.5f, // 4
        0.5f, -0.5f, -0.5f, // 7

        // Back
        0.5f, 0.5f, -0.5f, // 6
		0.5f, -0.5f, -0.5f, // 7
		-0.5f, -0.5f, -0.5f, // 4
        -0.5f, 0.5f, -0.5f, // 5
        0.5f, 0.5f, -0.5f, // 6
        -0.5f, -0.5f, -0.5f, // 4
	};

    unsigned int indices[] = {
		0, 1, 2, 0, 2, 3, 
		3, 2, 4, 4, 2, 5,
		1, 5, 2, 1, 6, 5,
		0, 3, 4, 0, 4, 7,
		1, 0, 7, 1, 7, 6,
		5, 6, 7, 4, 5, 7,
	};

    float texCoords[] = {
		1, 1,
        2, 1,
        2, 0,
        1, 1,
        2, 0,
        1, 0,

        0, 1,
        1, 1,
        0, 0,
        0, 0,
        1, 1,
        1, 0,

        3, 1,
        2, 0,
        3, 0,
        3, 1,
        2, 1,
        2, 0,

        4, 1,
        4, 0,
        3, 0,
        4, 1,
        3, 0,
        3, 1,

        5, 1,
        6, 1,
        6, 0,
        5, 1,
        6, 0,
        5, 0,

        5, 0,
        5, 1,
        4, 1,
        4, 0,
        5, 0,
        4, 1,
	};

    for ( int k = 0; k < 6*2*3*3; k += 3 ) {
        if ( k >= 0*3 && k < 6*3 && this->sides % 11 != 0 ) continue;
        if ( k >= 6*3 && k < 12*3 && this->sides % 2 != 0 ) continue;
        if ( k >= 12*3 && k < 18*3 && this->sides % 5 != 0 ) continue;
        if ( k >= 18*3 && k < 24*3 && this->sides % 7 != 0 ) continue;
        if ( k >= 24*3 && k < 30*3 && this->sides % 3 != 0 ) continue;
        if ( k >= 30*3 && k < 36*3 && this->sides % 13 != 0 ) continue;

        v->push_back( vertices[k + 0] + this->x );
        v->push_back( vertices[k + 1] + this->y );
        v->push_back( vertices[k + 2] + this->z );

        float texCoordX = texCoords[ 2*(k/3) ];
        float texCoordY = texCoords[ 2*(k/3) + 1 ];

        switch ( this->type ) {
            case CubeType::grassyDirt:
                v->push_back( texCoordX/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize + 0/(float)texFileSize );
                break;
            case CubeType::dirt:
                v->push_back( texCoordX/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize + 1/(float)texFileSize );
                break;
            case CubeType::water:
                v->push_back( texCoordX/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize + 4/(float)texFileSize );
                break;
            case CubeType::stone:
                v->push_back( texCoordX/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize + 3/(float)texFileSize );
                break;
            case CubeType::sand:
                v->push_back( texCoordX/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize + 2/(float)texFileSize );
                break;
            case CubeType::lava:
                v->push_back( texCoordX/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize + 5/(float)texFileSize );
                break;
        }
    }

    for ( int k = 0; k < 6*6; k++ ) {
        if ( k >= 0 && k < 6 && this->sides % 11 != 0 ) continue;
        if ( k >= 6 && k < 12 && this->sides % 2 != 0 ) continue;
        if ( k >= 12 && k < 18 && this->sides % 5 != 0 ) continue;
        if ( k >= 18 && k < 24 && this->sides % 7 != 0 ) continue;
        if ( k >= 24 && k < 30 && this->sides % 3 != 0 ) continue;
        if ( k >= 30 && k < 36 && this->sides % 13 != 0 ) continue;

        if (i->size() != 0) {
            i->push_back( (*i)[i->size() - 1] + 1 );
        } else {
            i->push_back( 0 );
        }
    }
}

Cube::~Cube() {
    
}

/*
    0   0 
    3   2 
    6   4 
    9   6 
    12  8 
    15  10
    18  12
    21  14
    24  12
*/
