#include "Chunk.hpp"

#include "World.hpp"
#include "cube/Cubes.hpp"

#include <game/world/cube/CubeTypes.hpp>
#include <engine/position/Position.hpp>

#include <lz4.h>
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
    if (this->updated)
        return;

    if (this->renderedCubes.size() != 0)
        this->renderedCubes.clear();

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
    for (std::size_t k = 0; k < this->renderedCubes.size(); k++)
        this->renderedCubes[k]->getVertex(&v, &i);
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

std::string _getChunkFilePath(Position<int>& position) {
    std::stringstream name;

    name << "saves/"
         << game->getWorld()->getName()
         << "/world/"
         << position.x << "_"
         << position.y << "_"
         << position.z << ".chunk";

    return name.str();
}

void Chunk::save() {
    this->mutex.lock();

    if (!this->generated)
        return;

    // TODO: This should not happen. My guess is that there is a race condition
    //       and chunks that are being generated get saved before having any
    //       cubes. This should do for now.
    for (std::size_t i = 0; i < this->cubes.size(); i++) {
        if (this->cubes[i] == nullptr) {
            std::cerr << "Found NULL blocks in chunk ("
                      << this->position.x << ", "
                      << this->position.y << ", "
                      << this->position.z << ")." << std::endl;
            return;
        }
    }

    std::ofstream file(_getChunkFilePath(this->position), std::ios::binary);

    auto bytes = this->toBytes();

    this->mutex.unlock();

    file.write(bytes.data(), bytes.size() * sizeof(decltype(bytes)::value_type));

    file.close();
}

void Chunk::load() {
    std::ifstream file(_getChunkFilePath(this->position), std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> compressedBytes(fileSize);

        file.read(compressedBytes.data(), compressedBytes.size());

        std::vector<char> outBytes(Chunk::W * Chunk::H * Chunk::Z);
        auto rv = LZ4_decompress_safe(compressedBytes.data(),
                                      outBytes.data(),
                                      compressedBytes.size(),
                                      outBytes.size());

        if (rv != outBytes.size()) {
            std::cerr << "Chunk data from disk is not complete!" << std::endl;
            return;
        }

        for (std::size_t i = 0; i < outBytes.size(); i++) {
            Position<int> position(
                i % Chunk::W,
                (i / Chunk::W) % Chunk::H,
                i / Chunk::W / Chunk::H
            );
            switch ((int)outBytes[i]) {
                case CubeType::air:
                    this->setCube(std::make_shared<Air>(), position, false);
                    break;
                case CubeType::dirt:
                    this->setCube(std::make_shared<Dirt>(), position, false);
                    break;
                case CubeType::grassyDirt:
                    this->setCube(std::make_shared<GrassyDirt>(), position, false);
                    break;
                case CubeType::sand:
                    this->setCube(std::make_shared<Sand>(), position, false);
                    break;
                case CubeType::water:
                    this->setCube(std::make_shared<Water>(), position, false);
                    break;
                case CubeType::lava:
                    this->setCube(std::make_shared<Lava>(), position, false);
                    break;
                default:
                    this->setCube(std::make_shared<Stone>(), position, false);
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

std::vector<char> Chunk::toBytes() const {
    std::vector outBuffer(Chunk::W * Chunk::H * Chunk::Z, (char)0);
    std::vector chunkBuffer(Chunk::W * Chunk::H * Chunk::Z, (char)0);

    for (std::size_t i = 0; i < this->cubes.size(); i++)
        chunkBuffer[i] = (char)this->cubes[i]->getType();

    auto rv = LZ4_compress_default(chunkBuffer.data(),
                                   outBuffer.data(),
                                   chunkBuffer.size(),
                                   outBuffer.size());

    if (rv < 1)
        std::cerr << "Error compressing chunk." << std::endl;

    outBuffer.resize(rv);
    return outBuffer;
}

std::shared_ptr<Cube> Chunk::getCube(Position<int> pos) {
    /*if (pos.x >= Chunk::W || pos.y >= Chunk::H || pos.z >= Chunk::Z || pos.x < 0 ||
        pos.y < 0 || pos.z < 0) {
        return nullptr;
    }*/
    return this->cubes[pos.x + pos.y * Chunk::W + pos.z * Chunk::W * Chunk::H];
}

void Chunk::setCube(std::shared_ptr<Cube> c, Position<int> pos, bool lockMutex) {
    if (pos.x >= this->W || pos.y >= this->H || pos.z >= this->Z)
        return;

    if (pos.x < 0 && pos.y < 0 && pos.z < 0)
        return;

    auto p = Position(pos.x + this->position.x * Chunk::W,
                      pos.y + this->position.y * Chunk::H,
                      pos.z + this->position.z * Chunk::Z);

    if (lockMutex)
        this->mutex.lock();
    c->setPos(p);
    c->setChunkPos(pos);
    c->setChunk(this);
    this->cubes[pos.x + pos.y * Chunk::W + pos.z * Chunk::H * Chunk::W] = c;

    if (pos.x == 0) {
        Chunk* chunk = this->world->getChunk(this->position.x - 1, this->position.y, this->position.z);
        if (chunk != nullptr)
            chunk->setUpdated(false);
    } else if (pos.x == Chunk::W - 1) {
        Chunk* chunk = this->world->getChunk(this->position.x + 1, this->position.y, this->position.z);
        if (chunk != nullptr)
            chunk->setUpdated(false);
    }

    if (pos.y == 0) {
        Chunk* chunk = this->world->getChunk(this->position.x, this->position.y - 1, this->position.z);
        if (chunk != nullptr) 
            chunk->setUpdated(false);
    } else if (pos.y == Chunk::H - 1) {
        Chunk* chunk = this->world->getChunk(this->position.x, this->position.y + 1, this->position.z);
        if (chunk != nullptr)
            chunk->setUpdated(false);
    }

    if (pos.z == 0) {
        Chunk* chunk = this->world->getChunk(this->position.x, this->position.y, this->position.z - 1);
        if (chunk != nullptr)
            chunk->setUpdated(false);
    } else if (pos.z == Chunk::Z - 1) {
        Chunk* chunk = this->world->getChunk(this->position.x, this->position.y, this->position.z + 1);
        if (chunk != nullptr)
            chunk->setUpdated(false);
    }

    this->updated = false;

    if (lockMutex)
        this->mutex.unlock();
}

Chunk::~Chunk() {
    printf("Delete chunk %d %d %d\n", this->position.x, this->position.y, this->position.z);
}
