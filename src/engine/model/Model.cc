#include "Model.hpp"

#include <engine/Engine.hpp>

void Model::setVertex(std::vector<float> vertex) {
    this->vertex = vertex;
}


void Model::setTextureCoords(std::vector<int> textureCoords) {
    this->textureCoords = textureCoords;
}


std::vector<float> Model::getVertex() {
    return this->vertex;
}


std::vector<int> Model::getTextureCoords() {
    return this->textureCoords;
}

int Model::genVao() {
    if (this->vao != 0)
        glDeleteVertexArrays(1, &(this->vao));

    unsigned int VBO, EBO;

    float *vertices = &this->vertex[0];
    int *indices = &this->textureCoords[0];

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertex.size(), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * this->textureCoords.size(), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return this->vao;
}

int Model::getVao() {
    return this->vao;
}

void Model::draw() {

    if (this->vao == 0) { // TODO I dont know why some chunks get drawn without previously generating VAO
        this->genVao();
        //return;
    }

    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->vertex.size() / 3, GL_UNSIGNED_INT, 0);
}

Model::~Model() {
    glDeleteVertexArrays(1, &(this->vao));
}
