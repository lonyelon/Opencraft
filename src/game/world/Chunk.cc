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

void Chunk::genTerrain() {
    const float xCoordRed = 150;
    const float yCoordRed = 150;
    const float zCoordRed = 150;
    const float noiseFreq = 1.5;
    const float ystrech = 60;
    const int heightIncrease = -60;

    const int waterHeight = 40 + heightIncrease;

    this->load();
    if (this->generated) {
        return;
    }
    this->generated = true;

    noise::module::Perlin p;

    int heights[this->W * this->Z];
    for (int i = 0; i < this->W * this->Z; i++) {
        heights[i] = -1;
    }

    p.SetSeed(this->world->getSeed());
    p.SetFrequency(noiseFreq);

    // Generate stone world and water

    for (int y = 0; y < Chunk::H; y++) {
        for (int x = 0; x < Chunk::W; x++) {
            for (int z = 0; z < Chunk::Z; z++) {
                float cX = static_cast<float>(this->position.x * Chunk::W + x) / xCoordRed;
                float cY = static_cast<float>(this->position.y * Chunk::H + y) / yCoordRed;
                float cZ = static_cast<float>(this->position.z * Chunk::Z + z) / zCoordRed;

                int noiseValue = (int) ((p.GetValue(cX, cY, cZ) + 1.1) * ystrech) + heightIncrease;

                if (y + this->position.y * Chunk::H < noiseValue) {
                    this->setCube(std::make_shared<Stone>(), Position(x, y, z));
                } else {
                    this->setCube(std::make_shared<Air>(), Position(x, y, z));
                }
            }
        }
    }

    p.SetSeed(this->world->getSeed() * 2);
    p.SetLacunarity(1);

    for (int x = 0; x < this->W; x++) {
        for (int z = 0; z < this->Z; z++) {
            int dirtCount = 0;
            for (int y = Chunk::H - 1; y >= 0; y--) {
                auto cubePos = Position(x, y, z);
                std::shared_ptr<Cube> c = this->getCube(cubePos);

                float noiseX = (float) (this->position.x * this->W + x) / xCoordRed * 10;
                float noiseY = (float) (this->position.y * this->H + z) / yCoordRed * 10;
                float noiseZ = (float) (this->position.z * this->Z + z) / zCoordRed * 10;

                if (c->getType() == CubeType::air) {
                    if (y + this->position.y * Chunk::H < waterHeight) {
                        this->setCube(std::make_shared<Water>(), cubePos);
                    } else {
                        dirtCount = 0;
                    }
                } else {
                    if (dirtCount < 4) {
                        if (y + this->position.y * Chunk::H < heights[x + z * this->W] ||
                            heights[x + z * this->W] == -1) {
                            heights[x + z * this->W] = y + this->position.y * Chunk::H;
                        }
                        if ((p.GetValue(noiseX, noiseY, noiseZ) * 5) /
                            (waterHeight - y - this->position.y * Chunk::H) >
                            1) {
                            this->setCube(std::make_shared<Sand>(), cubePos);
                        } else if (dirtCount == 0 && y + this->position.y * Chunk::H >= waterHeight - 1) {
                            this->setCube(std::make_shared<GrassyDirt>(), cubePos);
                        } else {
                            this->setCube(std::make_shared<Dirt>(), cubePos);
                        }
                        dirtCount++;
                    }
                }
            }
        }
    }

    noise::module::Perlin caveNoise;

    float caveFreq = 1;

    caveNoise.SetSeed(this->world->getSeed() * 3);
    caveNoise.SetFrequency(caveFreq);
    caveNoise.SetLacunarity(1);

    for (int x = 0; x < this->W; x++) {
        for (int z = 0; z < this->Z; z++) {
            for (int y = this->H - 1; y >= 0; y--) {
                auto cubePos = Position(x, y, z);
                std::shared_ptr<Cube> c = this->getCube(cubePos);

                float caveProb = -(c->getY()+50)/500-0.75;
                if (caveProb > 0.0)
                    caveProb = 0.0;
                if (caveProb < -1.0)
                    caveProb = -1.0;

                float noiseX = (float) (this->position.x * this->W + x) / xCoordRed * 10;
                float noiseY = (float) (this->position.y * this->H + y) / yCoordRed * 10;
                float noiseZ = (float) (this->position.z * this->Z + z) / zCoordRed * 10;

                float caveHeightRedux = y + this->position.y * Chunk::H;
                if (caveHeightRedux < 1) {
                    caveHeightRedux = 1;
                }

                if ((caveNoise.GetValue(noiseX, noiseY, noiseZ) < caveProb) && c->getType() != CubeType::water) {
                    this->setCube(std::make_shared<Air>(), cubePos);
                }
            }
        }
    }
    /*
    for (int x = 0; x < this->W; x++) {
        for (int z = 0; z < this->position.z; z++) {
            int dirtCount = 0;
            for (int y = this->H-1; y > 0; y--) {
                std::shared_ptr<Cube> c = this->getCube( x, y, z );

                if ( y < 12 && c->getType() == CubeType::air ) {
                    this->setCube(new Lava(), x, y, z);
                }
            }
        }
    }*/

    this->save();
}

std::shared_ptr<Cube> Chunk::getCube(Position<int> pos) {
    /*if (pos.x >= Chunk::W || pos.y >= Chunk::H || pos.z >= Chunk::Z || pos.x < 0 ||
        pos.y < 0 || pos.z < 0) {
        return nullptr;
    }*/
    return this->cubes[pos.x + pos.y * Chunk::W + pos.z * Chunk::W * Chunk::H];
}

void Chunk::setCube(std::shared_ptr<Cube> c, Position<int> pos) {
    if (pos.x >= this->W || pos.y >= this->H || pos.z >= this->Z) {
        return;
    }

    if (pos.x < 0 && pos.y < 0 && pos.z < 0) {
        return;
    }

    auto p = Position(pos.x + this->position.x * Chunk::W, pos.y + this->position.y * Chunk::H,
                      pos.z + this->position.z * Chunk::Z);
    this->mutex.lock();
    c->setPos(p);
    c->setChunkPos(pos);
    c->setChunk(this);
    this->cubes[pos.x + pos.y * Chunk::W + pos.z * Chunk::H * Chunk::W] = c;

    if (this->generated) {
        Chunk *c = this->world->getChunk(this->position.x + 1, this->position.y,
                                         this->position.z);
        if (pos.x == Chunk::W - 1 && c != nullptr) {
            c->setUpdated(false);
        }

        c = this->world->getChunk(this->position.x - 1, this->position.y, this->position.z);
        if (pos.x == 0 && c != nullptr) {
            c->setUpdated(false);
        }

        c = this->world->getChunk(this->position.x, this->position.y + 1, this->position.z);
        if (pos.y == Chunk::H - 1 && c != nullptr) {
            c->setUpdated(false);
        }

        c = this->world->getChunk(this->position.x, this->position.y - 1, this->position.z);
        if (pos.y == 0 && c != nullptr) {
            c->setUpdated(false);
        }

        c = this->world->getChunk(this->position.x, this->position.y, this->position.z + 1);
        if (pos.z == Chunk::Z - 1 && c != nullptr) {
            c->setUpdated(false);
        }

        c = this->world->getChunk(this->position.x, this->position.y, this->position.z - 1);
        if (pos.z == 0 && c != nullptr) {
            c->setUpdated(false);
        }
    }
    this->updated = false;
    this->mutex.unlock();
}


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

    std::ofstream file(name.str());

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
    name << "saves/" << this->world->getName() << "/world/" << this->position.x << "_"
         << this->position.y << "_"
         << this->position.z
         << ".chunk";

    std::ifstream file(name.str());

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
    }
}

World *Chunk::getWorld() {
    return this->world;
}

Chunk::~Chunk() {
    printf("Delete chunk %d %d %d\n", this->position.x, this->position.y, this->position.z);
}
