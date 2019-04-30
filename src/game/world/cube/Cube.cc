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
}

Cube::Cube(  ) {
    Cube::x = 0;
    Cube::y = 0;
    Cube::z = 0;
    Cube::type = CubeType::air;
    Cube::chunk = NULL;
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
		-0.5f, -0.5f, 0.5f, // 2
		0.5f, -0.5f, 0.5f, // 3
		0.5f, 0.5f, 0.5f, // 1
		-0.5f, 0.5f, 0.5f, // 0
		-0.5f, 0.5f, -0.5f, // 5
		0.5f, 0.5f, -0.5f, // 6
		0.5f, -0.5f, -0.5f, // 7
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
		0, 0,
        0, 1,
        1, 0,
        1, 1,
        0, 1,
        0, 0,
        1, 1,
        1, 0
	};

    for ( int k = 0; k < 8*3; k += 3 ) {
        v->push_back( vertices[k + 0] + this->x );
        v->push_back( vertices[k + 1] + this->y );
        v->push_back( vertices[k + 2] + this->z );

        float texCoordX = texCoords[ 2*(k/3) ];
        float texCoordY = texCoords[ 2*(k/3) + 1 ];

        switch ( this->type ) {
            case CubeType::grassyDirt:
                v->push_back( texCoordX/(float)texFileSize + (float)1/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize + (float)1/(float)texFileSize );
                break;
            case CubeType::dirt:
                v->push_back( texCoordX/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize );
                break;
            case CubeType::water:
                v->push_back( texCoordX/(float)texFileSize + 3/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize + 1/(float)texFileSize );
                break;
            case CubeType::stone:
                v->push_back( texCoordX/(float)texFileSize + 1/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize );
                break;
            case CubeType::sand:
                v->push_back( texCoordX/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize + 1/(float)texFileSize );
                break;
            case CubeType::lava:
                v->push_back( texCoordX/(float)texFileSize + 3/(float)texFileSize );
                v->push_back( texCoordY/(float)texFileSize );
                break;
        }
    }

    for ( int k = 0; k < 6*6; k += 3 ) {
        i->push_back( indices[k + 0] + 8*n );
        i->push_back( indices[k + 1] + 8*n );
        i->push_back( indices[k + 2] + 8*n );
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
