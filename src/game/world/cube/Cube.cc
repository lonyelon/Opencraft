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
    float vertices[] = {
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3, 
		3, 2, 4, 4, 2, 5,
		1, 5, 2, 1, 6, 5,
		0, 3, 4, 0, 4, 7,
		1, 0, 7, 1, 7, 6,
		5, 6, 7, 4, 5, 7,
	};

    float grassColor[] = {
		0.66, 0.4, 0.125,
		0.66*0.9, 0.4*0.9, 0.125*0.9,
		0, 1, 0,
		0, 0.9, 0,
		0, 0.8, 0,
		0, 0.7, 0,
		0.66*0.8, 0.4*0.8, 0.125*0.8,
		0.66*0.7, 0.4*0.7, 0.125*0.7,
	};

    float dirtColor[] = {
		0.66*1.0, 0.4*1.0, 0.125*1.0,
		0.66*0.95, 0.4*0.95, 0.125*0.95,
		0.66*0.9, 0.4*0.9, 0.125*0.9,
		0.66*0.85, 0.4*0.85, 0.125*0.85,
		0.66*0.8, 0.4*0.8, 0.125*0.8,
		0.66*0.75, 0.4*0.75, 0.125*0.75,
		0.66*0.65, 0.4*0.65, 0.125*0.65,
		0.66*0.7, 0.4*0.7, 0.125*0.7,
	};

    float sandColor[] = {
		0.9*1.0, 0.8*1.0, 0.2*1.0,
		0.9*0.95, 0.8*0.95, 0.2*0.95,
		0.9*0.9, 0.8*0.9, 0.2*0.9,
		0.9*0.85, 0.8*0.85, 0.2*0.85,
		0.9*0.8, 0.8*0.8, 0.2*0.8,
		0.9*0.75, 0.8*0.75, 0.2*0.75,
		0.9*0.65, 0.8*0.65, 0.2*0.65,
		0.9*0.7, 0.8*0.7, 0.2*0.7,
	};

    float lavaColor[] = {
		0.9*1.0, 0.4*1.0, 0.1*1.0,
		0.9*0.95, 0.4*0.95, 0.1*0.95,
		0.9*0.9, 0.4*0.9, 0.1*0.9,
		0.9*0.85, 0.4*0.85, 0.1*0.85,
		0.9*0.8, 0.4*0.8, 0.1*0.8,
		0.9*0.75, 0.4*0.75, 0.1*0.75,
		0.9*0.65, 0.4*0.65, 0.1*0.65,
		0.9*0.7, 0.4*0.7, 0.1*0.7,
	};

    float waterColor[] = {
		0, 0, 1,
        0, 0, 0.95,
        0, 0, 0.9,
        0, 0, 0.85,
        0, 0, 0.8,
        0, 0, 0.75,
        0, 0, 0.7,
        0, 0, 0.65,
	};

    float stoneColor[] = {
		0.4, 0.4, 0.4,
        0.375, 0.375, 0.375,
        0.35, 0.35, 0.35,
        0.325, 0.325, 0.325,
        0.3, 0.3, 0.3,
        0.275, 0.275, 0.275,
        0.25, 0.25, 0.25,
        0.225, 0.225, 0.225,
	};

    for ( int k = 0; k < 8*3; k += 3 ) {
        v->push_back( vertices[k + 0] + this->x );
        v->push_back( vertices[k + 1] + this->y );
        v->push_back( vertices[k + 2] + this->z );

        switch (this->type) {
            case CubeType::grassyDirt:
                v->push_back( grassColor[k + 0] );
                v->push_back( grassColor[k + 1] );
                v->push_back( grassColor[k + 2] );
                break;
            case CubeType::dirt:
                v->push_back( dirtColor[k + 0] );
                v->push_back( dirtColor[k + 1] );
                v->push_back( dirtColor[k + 2] );
                break;
            case CubeType::water:
                v->push_back( waterColor[k + 0] );
                v->push_back( waterColor[k + 1] );
                v->push_back( waterColor[k + 2] );
                break;
            case CubeType::stone:
                v->push_back( stoneColor[k + 0] );
                v->push_back( stoneColor[k + 1] );
                v->push_back( stoneColor[k + 2] );
                break;
            case CubeType::sand:
                v->push_back( sandColor[k + 0] );
                v->push_back( sandColor[k + 1] );
                v->push_back( sandColor[k + 2] );
                break;
            case CubeType::lava:
                v->push_back( lavaColor[k + 0] );
                v->push_back( lavaColor[k + 1] );
                v->push_back( lavaColor[k + 2] );
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

