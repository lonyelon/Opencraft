#include "Cube.hpp"

#include <cstdio>
#include <cstdlib>

#include "../engine/glfw.hpp"

extern GLuint shaderProgram;
extern unsigned int VAOTriangulo;

void Cube::recalcModelMatrix() {
    glm::mat4 model;

    if (this->m == NULL) {
        this->m = malloc(sizeof(glm::mat4));
    }

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(this->x, this->y, this->z));

    *((glm::mat4*)this->m) = model;
}

Cube::Cube(int xpos, int ypos, int zpos) {
    Cube::x = xpos;
    Cube::y = ypos;
    Cube::z = zpos;
    Cube::type = CubeType::air;

    Cube::recalcModelMatrix();
}

Cube::Cube() {
    Cube::x = 0;
    Cube::y = 0;
    Cube::z = 0;
    Cube::type = CubeType::air;

    Cube::recalcModelMatrix();
}

void Cube::setPosition(int x, int y, int z) {
    Cube::x = x;
    Cube::y = y;
    Cube::z = z;

    Cube::recalcModelMatrix();
}

void Cube::setType(CubeType t) {
    Cube::type=t;
}

CubeType Cube::getType() {
    return Cube::type;
}

void Cube::getVertex(std::vector<float> *v, std::vector<int> *i, int n) {
    float vertices[] = {
		-0.5f, -0.5f, 0.5f, 	0.4,0.4,0.4, // back
		0.5f, -0.5f, 0.5f,		0.6,0.6,0.6,
		0.5f, 0.5f, 0.5f, 		0,1,0,
		-0.5f, 0.5f, 0.5f, 		0,0.25,0,
		-0.5f, 0.5f, -0.5f, 	0,0.5,0, // top
		0.5f, 0.5f, -0.5f, 		0,0.75,0,
		0.5f, -0.5f, -0.5f, 	1,1,1, // right
		-0.5f, -0.5f, -0.5f, 	0.8,0.8,0.8, // right
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3, 
		3, 2, 4, 4, 2, 5,
		1, 5, 2, 1, 6, 5,
		0, 3, 4, 0, 4, 7,
		1, 0, 7, 1, 7, 6,
		5, 6, 7, 4, 5, 7,
	};

    for ( int k = 0; k < 8*6; k += 3 ) {
        if ( k % 2 == 0 ) {
            v->push_back( vertices[k + 0] + this->x );
            v->push_back( vertices[k + 1] + this->y );
            v->push_back( vertices[k + 2] + this->z );
        } else {
            v->push_back( vertices[k + 0] );
            v->push_back( vertices[k + 1] );
            v->push_back( vertices[k + 2] );
        }
    }

    for ( int k = 0; k < 6*6; k += 3 ) {
        i->push_back( indices[k + 0] + 8*n );
        i->push_back( indices[k + 1] + 8*n );
        i->push_back( indices[k + 2] + 8*n );
    }
}

int Cube::draw() {
    if ( this->type == CubeType::air ) return -1;

    if (this->type == CubeType::water)
        glUniform1i(glGetUniformLocation(shaderProgram, "isWater"), 1);
    else 
        glUniform1i(glGetUniformLocation(shaderProgram, "isWater"), 0);

    glm::mat4 model = *((glm::mat4*)this->m);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    return 0;
}