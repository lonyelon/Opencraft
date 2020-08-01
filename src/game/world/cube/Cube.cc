#include <game/world/cube/Cube.hpp>

#include <cstdio>

#include <game/Game.hpp>
#include <engine/model/Model.hpp>
#include <memory>

extern std::unique_ptr<Game> game;

Cube::Cube(Chunk *c, int xpos, int ypos, int zpos) {
    this->x = xpos;
    this->y = ypos;
    this->z = zpos;
    this->type = CubeType::air;
    this->chunk = c;
    this->sides = 1;
}

Cube::Cube() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->type = CubeType::air;
    this->chunk = nullptr;
    this->sides = 1;
}

void Cube::setPosition(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Cube::setType(CubeType t) {
    this->type = t;
}

CubeType Cube::getType() {
    return Cube::type;
}

void Cube::getVertex(std::vector<float> *v, std::vector<int> *i, int n) {
    const int texFileSize = 16;
    const int texFileSizeX = 6;

    std::vector<float> vertex;
    std::vector<int> textureCoords;

    switch (this->type) {
        case CubeType::grass:
            vertex = game->grassModel->getVertex();
            textureCoords = game->grassModel->getTextureCoords();
            break;
        case CubeType::water:
            vertex = game->fluidModel->getVertex();
            textureCoords = game->fluidModel->getTextureCoords();
            break;
        default:
            vertex = game->cubeModel->getVertex();
            textureCoords = game->cubeModel->getTextureCoords();
    }

    for (std::size_t k = 0; k < vertex.size(); k += 3) {
        if (this->type != CubeType::grass) {
            if (k >= 0 * 3 && k < 6 * 3 && this->sides % 11 != 0) continue;
            if (k >= 6 * 3 && k < 12 * 3 && this->sides % 2 != 0) continue;
            if (k >= 12 * 3 && k < 18 * 3 && this->sides % 5 != 0) continue;
            if (k >= 18 * 3 && k < 24 * 3 && this->sides % 7 != 0) continue;
            if (k >= 24 * 3 && k < 30 * 3 && this->sides % 3 != 0) continue;
            if (k >= 30 * 3 && k < 36 * 3 && this->sides % 13 != 0) continue;
        }

        v->push_back(vertex[k + 0] + this->x);
        v->push_back(vertex[k + 1] + this->y);
        v->push_back(vertex[k + 2] + this->z);

        float texCoordX = textureCoords[2 * (k / 3)];
        float texCoordY = textureCoords[2 * (k / 3) + 1];

        v->push_back(texCoordX / (float) texFileSizeX);
        v->push_back(texCoordY / (float) texFileSize + (this->getType() - 1) / (float) texFileSize);
    }

    for (std::size_t k = 0; k < vertex.size() / 3; k++) {
        if (this->type != CubeType::grass) {
            if (k >= 0 && k < 6 && this->sides % 11 != 0) continue;
            if (k >= 6 && k < 12 && this->sides % 2 != 0) continue;
            if (k >= 12 && k < 18 && this->sides % 5 != 0) continue;
            if (k >= 18 && k < 24 && this->sides % 7 != 0) continue;
            if (k >= 24 && k < 30 && this->sides % 3 != 0) continue;
            if (k >= 30 && k < 36 && this->sides % 13 != 0) continue;
        }

        if (i->size() != 0) {
            i->push_back((*i)[i->size() - 1] + 1);
        } else {
            i->push_back(0);
        }
    }
}

FixedPosition Cube::getPos() {
    int x = this->getX();
    int y = this->getY();
    int z = this->getZ();
    return FixedPosition(x, y, z);
}

Cube::~Cube() {

}
