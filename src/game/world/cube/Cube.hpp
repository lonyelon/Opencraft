#ifndef _CUBE_HPP_
#define _CUBE_HPP_

#include <vector>

//#include "Cube.hpp"
#include "../CubeTypes.hpp"
#include "../../../engine/glfw.hpp"
#include "../../../engine/FixedPosition.hpp"

class Chunk;

class Cube {
protected:
    Chunk *chunk;
    CubeType type;
    int x, y, z;
	FixedPosition chunkPos;
    int sides;
	bool transparent;

public:
    Cube ();
    Cube ( Chunk *c, int xpos, int ypos, int zpos );

    void setPosition( int x, int y, int z );

    void setType( CubeType t );
    CubeType getType();

    void setSides(int s) { this->sides = s; };
    int getSides() { return this->sides; };

    void getVertex( std::vector<float> *v, std::vector<int> *i, int n );

    int getX() { return this->x; };
    int getY() { return this->y; };
    int getZ() { return this->z; };
	void setX(int x) { this->x = x; };
    void setY(int y) { this->y = y; };
    void setZ(int z) { this->z = z; };

	void setChunkPos(FixedPosition pos) { this->chunkPos = pos; };
	FixedPosition getChunkPos() {return this->chunkPos; };

    Chunk * getChunk() { return this->chunk; };
	void setChunk(Chunk *c) { this->chunk = c; };

	bool isTransparent() { return this->transparent; }

    ~Cube();
};


#endif
