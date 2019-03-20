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

int Cube::draw() {
    if ( this->type == CubeType::air ) return -1;
	//if ( c[k].isIllated(cube[i].getX(),cube[i].getY(),cube[i].getZ()) ) return;

    if (this->type == CubeType::water)
        glUniform1i(glGetUniformLocation(shaderProgram, "isWater"), 1);
    else 
        glUniform1i(glGetUniformLocation(shaderProgram, "isWater"), 0);

    glm::mat4 model = *((glm::mat4*)this->m);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUseProgram(shaderProgram);

	glBindVertexArray(VAOTriangulo);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    return 0;
}