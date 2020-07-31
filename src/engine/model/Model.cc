#include "Model.hpp"

Model::Model() {

}

void Model::setVertex( std::vector<double> vertex ) {
    this->vertex = vertex;
}


void Model::setTextureCoords( std::vector<int> textureCoords ) {
    this->textureCoords = textureCoords;
}


std::vector<double> Model::getVertex() {
    return this->vertex;
}


std::vector<int> Model::getTextureCoords() {
    return this->textureCoords;
}