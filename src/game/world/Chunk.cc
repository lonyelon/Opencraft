#include "Chunk.hpp"

#include "World.hpp"
#include "cube/Cubes.hpp"

#include <game/world/cube/CubeTypes.hpp>
#include <engine/position/Position.hpp>

#include <noise/noise.h>

#include <cstdio>
#include <vector>
#include <sstream>
#include <memory>

#include <game/Game.hpp>

extern std::unique_ptr<Game> game;

std::array<std::shared_ptr<Cube>, Chunk::W * Chunk::H * Chunk::Z> Chunk::getCubes() const {
    return this->cubes;
}

bool Chunk::isCubeCovered(int x, int y, int z) {
    std::shared_ptr<Cube> c = nullptr;
    std::shared_ptr<Cube> k = this->world->getCube(this, x, y, z);

    if (k == nullptr)
        return false;

    c = this->world->getCube(this, x, y + 1, z);
    if (c != nullptr && c->isTransparent() && k->getType() != c->getType())
        return false;

    c = this->world->getCube(this, x, y - 1, z);
    if (c != nullptr && c->isTransparent() && k->getType() != c->getType())
        return false;

    c = this->world->getCube(this, x + 1, y, z);
    if (c != nullptr && c->isTransparent() && k->getType() != c->getType())
        return false;

    c = this->world->getCube(this, x - 1, y, z);
    if (c != nullptr && c->isTransparent() && k->getType() != c->getType())
        return false;

    c = this->world->getCube(this, x, y, z + 1);
    if (c != nullptr && c->isTransparent() && k->getType() != c->getType())
        return false;

    c = this->world->getCube(this, x, y, z - 1);
    if (c != nullptr && c->isTransparent() && k->getType() != c->getType())
        return false;

    return true;
}

void Chunk::getCubeVisibleSides(int x, int y, int z) {
    std::shared_ptr<Cube> c = nullptr;
    std::shared_ptr<Cube> k = this->world->getCube(this, x, y, z);
    int n = 1;

    if (k != nullptr) {
        c = this->world->getCube(this, x, y + 1, z);
        if (c != nullptr && c->isTransparent() && k->getType() != c->getType()) {
            n *= 2;
        } else if (c == nullptr) {
            n *= 2;
        }

        c = this->world->getCube(this, x, y - 1, z);
        if (c != nullptr && c->isTransparent() && k->getType() != c->getType()) {
            n *= 3;
        } else if (c == nullptr) {
            n *= 3;
        }

        c = this->world->getCube(this, x + 1, y, z);
        if (c != nullptr && c->isTransparent() && k->getType() != c->getType()) {
            n *= 5;
        } else if (c == nullptr) {
            n *= 5;
        }

        c = this->world->getCube(this, x - 1, y, z);
        if (c != nullptr && c->isTransparent() && k->getType() != c->getType()) {
            n *= 7;
        } else if (c == nullptr) {
            n *= 7;
        }

        c = this->world->getCube(this, x, y, z + 1);
        if (c != nullptr && c->isTransparent() && k->getType() != c->getType()) {
            n *= 11;
        } else if (c == nullptr) {
            n *= 11;
        }

        c = this->world->getCube(this, x, y, z - 1);
        if (c != nullptr && c->isTransparent() && k->getType() != c->getType()) {
            n *= 13;
        } else if (c == nullptr) {
            n *= 13;
        }

        k->setSides(n);
    }
}

void Chunk::getVisibleCubes() {
    if (this->updated) {
        return;
    }

    if (this->renderedCubes.size() != 0) {
        this->renderedCubes.clear();
    }

    this->world->genChunkAt(false, this->position.x + 1, this->position.y, this->position.z);
    this->world->genChunkAt(false, this->position.x - 1, this->position.y, this->position.z);
    this->world->genChunkAt(false, this->position.x, this->position.y, this->position.z + 1);
    this->world->genChunkAt(false, this->position.x, this->position.y, this->position.z - 1);
    this->world->genChunkAt(false, this->position.x, this->position.y - 1, this->position.z);
    this->world->genChunkAt(false, this->position.x, this->position.y + 1, this->position.z);

    this->mutex.lock();
    for (int i = 0; i < this->W * this->H * this->Z; i++) {
        if (this->cubes[i]->getType() != CubeType::air && !this->isCubeCovered(this->cubes[i]->getX(), this->cubes[i]->getY(), this->cubes[i]->getZ()) == 1) {
            this->getCubeVisibleSides(this->cubes[i]->getX(), this->cubes[i]->getY(), this->cubes[i]->getZ());
            this->renderedCubes.push_back(this->cubes[i]);
        }
    }
    this->mutex.unlock();

    this->updateModel();

    this->mutex.lock();
    this->updated = true;
    this->mutex.unlock();

}

void Chunk::updateModel() {
    this->mutex.lock();
    this->chunkModel = std::make_unique<Model>();
    std::vector<float> v;
    std::vector<int> i;
    for (std::size_t k = 0; k < this->renderedCubes.size(); k++) {
        this->renderedCubes[k]->getVertex(&v, &i);
    }
    this->chunkModel->setVertex(v);
    this->chunkModel->setTextureCoords(i);
    this->mutex.unlock();
}


void Chunk::genVao() {
    this->chunkModel->genVao();
}

void Chunk::draw() {
    if (this->chunkModel != nullptr) {
        this->mutex.lock();
        this->chunkModel->draw();
        this->mutex.unlock();
    }
}

void Chunk::save() const {
    if (!this->generated) {
        return;
    }

    std::stringstream name;
    name << "saves/" << this->world->getName() << "/world/" << this->position.x << "_"
         << this->position.y << "_"
         << this->position.z
         << ".chunk";

    std::ofstream file(name.str(), std::ios::out | std::ios::binary);

    for (int i = 0; i < Chunk::W * Chunk::H * Chunk::Z; i++) {
        file << this->cubes[i]->getType() << "\t";
        Position<int> p = this->cubes[i]->getChunkPos();
        file << p.x << "\t";
        file << p.y << "\t";
        file << p.z << "\n";
    }

    file.close();
}

void Chunk::load() {
    std::stringstream name;
    name << "saves/" << this->world->getName()
         << "/world/"
         << this->position.x
         << "_"
         << this->position.y
         << "_"
         << this->position.z
         << ".chunk";

    std::ifstream file(name.str(), std::ios::in | std::ios::binary);

    if (file.is_open()) {
        while (!file.eof()) {
            int type, x, y, z;
            file >> type >> x >> y >> z;
            auto cubePos = Position(x, y, z);
            switch (type) {
                case CubeType::air:
                    this->setCube(std::make_shared<Air>(), cubePos);
                    break;
                case CubeType::dirt:
                    this->setCube(std::make_shared<Dirt>(), cubePos);
                    break;
                case CubeType::grassyDirt:
                    this->setCube(std::make_shared<GrassyDirt>(), cubePos);
                    break;
                case CubeType::sand:
                    this->setCube(std::make_shared<Sand>(), cubePos);
                    break;
                case CubeType::water:
                    this->setCube(std::make_shared<Water>(), cubePos);
                    break;
                case CubeType::lava:
                    this->setCube(std::make_shared<Lava>(), cubePos);
                    break;
                default:
                    this->setCube(std::make_shared<Stone>(), cubePos);
                    break;
            }
        }

        this->generated = true;
        file.close();
    }
}

World *Chunk::getWorld() {
    return this->world;
}

Chunk::~Chunk() {
    printf("Delete chunk %d %d %d\n", this->position.x, this->position.y, this->position.z);
}
