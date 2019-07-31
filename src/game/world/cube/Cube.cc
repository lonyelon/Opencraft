#include "Cube.hpp"

#include <cstdio>
#include <cstdlib>

#include "../../../engine/model/Model.hpp"

extern GLuint shaderProgram;
extern Model *cubeModel;

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

    std::vector<float> vertex = cubeModel->getVertex();
    std::vector<int> textureCoords = cubeModel->getTextureCoords();

    for ( int k = 0; k < 6*2*3*3; k += 3 ) {
        if ( k >= 0*3 && k < 6*3 && this->sides % 11 != 0 ) continue;
        if ( k >= 6*3 && k < 12*3 && this->sides % 2 != 0 ) continue;
        if ( k >= 12*3 && k < 18*3 && this->sides % 5 != 0 ) continue;
        if ( k >= 18*3 && k < 24*3 && this->sides % 7 != 0 ) continue;
        if ( k >= 24*3 && k < 30*3 && this->sides % 3 != 0 ) continue;
        if ( k >= 30*3 && k < 36*3 && this->sides % 13 != 0 ) continue;

        v->push_back( vertex[k + 0] + this->x );
        v->push_back( vertex[k + 1] + this->y );
        v->push_back( vertex[k + 2] + this->z );

        float texCoordX = textureCoords[ 2*(k/3) ];
        float texCoordY = textureCoords[ 2*(k/3) + 1 ];

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