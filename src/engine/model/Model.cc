#include "Model.hpp"

Model::Model() {

}

void Model::setVertex( std::vector<float> vertex ) {
    this->vertex = vertex;
}


void Model::setTextureCoords( std::vector<int> textureCoords ) {
    this->textureCoords = textureCoords;
}


std::vector<float> Model::getVertex() {
    return this->vertex;
}


std::vector<int> Model::getTextureCoords() {
    return this->textureCoords;
}