#include "Chunk.hpp"
#include "World.hpp"
#include "cube/Cubes.hpp"
#include <game/world/cube/CubeTypes.hpp>
#include <game/world/strcuture/Tree.hpp>

#include <noise/noise.h>

#include <cstdio>
#include <vector>
#include <sstream>


extern GLuint shaderProgram;
extern int pint;

Chunk::Chunk(World *w, int xpos, int ypos, int zpos) {
    this->world = w;
    this->x = xpos;
    this->y = ypos;
    this->z = zpos;

    this->generated = false;

    this->cubes = std::vector<Cube *>(this->W * this->H * this->Z);
    this->VAO = 0;
}

void Chunk::genTerrain() {
    const float xCoordRed = 150;
    const float yCoordRed = 150;
    const float zCoordRed = 150;
    const float noiseFreq = 1;
    const float ystrech = 60;
    const int heightIncrease = -60;

    const int waterHeight = 40 + heightIncrease;

    this->Load();
    if (this->generated == true) {
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
                float noiseX = (float) (this->x * Chunk::W + x) / xCoordRed;
                float noiseY = (float) (this->y * Chunk::H + y) / yCoordRed;
                float noiseZ = (float) (this->z * Chunk::Z + z) / zCoordRed;

                int noiseValue = (int) ((p.GetValue(noiseX, noiseY, noiseZ) + 1.1) * ystrech) + heightIncrease;

                if (y + this->y * Chunk::H < noiseValue) {
                    this->setCube(new Stone(), x, y, z);
                } else {
                    this->setCube(new Air(), x, y, z);
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
                Cube *c = this->getCube(x, y, z);

                float noiseX = (float) (this->x * this->W + x) / xCoordRed * 10;
                float noiseY = (float) (this->y * this->H + z) / yCoordRed * 10;
                float noiseZ = (float) (this->z * this->Z + z) / zCoordRed * 10;

                if (c->getType() == CubeType::air) {
                    if (y + this->y * Chunk::H < waterHeight) {
                        this->setCube(new Water(), x, y, z);
                    } else {
                        dirtCount = 0;
                    }
                } else {
                    if (dirtCount < 4) {
                        if (y + this->y * Chunk::H < heights[x + z * this->W] || heights[x + z * this->W] == -1) {
                            heights[x + z * this->W] = y + this->y * Chunk::H;
                        }
                        if ((p.GetValue(noiseX, noiseY, noiseZ) * 5) / (waterHeight - y - this->y * Chunk::H) > 1) {
                            this->setCube(new Sand(), x, y, z);
                        } else if (dirtCount == 0 && y + this->y * Chunk::H >= waterHeight - 1) {
                            this->setCube(new GrassyDirt(), x, y, z);

                            if (rand() % 100 == 1) {
                                Tree t = Tree(this->world);
                                t.construct(FixedPosition(x + this->x*Chunk::W,y + this->y*Chunk::H + 1,z + this->z*Chunk::Z));
                            }
                        } else {
                            this->setCube(new Dirt(), x, y, z);
                        }
                        dirtCount++;
                    }
                }
            }
        }
    }

    noise::module::Perlin caveNoise;

    float caveFreq = 1;
    float caveProb = -0.5;

    caveNoise.SetSeed(this->world->getSeed() * 3);
    caveNoise.SetFrequency(caveFreq);
    caveNoise.SetLacunarity(1);

    for (int x = 0; x < this->W; x++) {
        for (int z = 0; z < this->Z; z++) {
            for (int y = this->H - 1; y >= 0; y--) {
                Cube *c = this->getCube(x, y, z);

                float noiseX = (float) (this->x * this->W + x) / xCoordRed * 10;
                float noiseY = (float) (this->y * this->H + y) / yCoordRed * 10;
                float noiseZ = (float) (this->z * this->Z + z) / zCoordRed * 10;

                float caveHeightRedux = y + this->y * Chunk::H;
                if (caveHeightRedux < 1) {
                    caveHeightRedux = 1;
                }

                if ((caveNoise.GetValue(noiseX, noiseY, noiseZ) < caveProb) && c->getType() != CubeType::water) {
                    this->setCube(new Air(), x, y, z);
                }
            }
        }
    }
/*
    for (int x = 0; x < this->W; x++) {
        for (int z = 0; z < this->Z; z++) {
            int dirtCount = 0;
            for (int y = this->H-1; y > 0; y--) {
                Cube *c = this->getCube( x, y, z );

                if ( y < 12 && c->getType() == CubeType::air ) {
                    this->setCube(new Lava(), x, y, z);
                }
            }
        }
    }*/

    this->Save();
}

Cube *Chunk::getCube(unsigned int x, int y, int z) {
    if (x + y * this->W + z * this->W * this->H > this->W * this->H * this->Z) {
        return NULL;
    }
    return this->cubes[x + y * this->W + z * this->W * this->H];
}

Cube *Chunk::getCube(FixedPosition pos) {
    if (pos.getX() + pos.getY() * this->W + pos.getZ() * this->W * this->H > this->W * this->H * this->Z) {
        return NULL;
    }
    return this->cubes[pos.getX() + pos.getY() * this->W + pos.getZ() * this->W * this->H];
}

void Chunk::setCube(Cube *c, int x, int y, int z) {

    this->setCube(c, FixedPosition(x, y, z));
}

void Chunk::setCube(Cube *c, FixedPosition pos) {
    if (pos.getX() >= this->W || pos.getY() >= this->H || pos.getZ() >= this->Z) {
        return;
    }

    if (pos.getX() < 0 && pos.getY() < 0 && pos.getZ() < 0) {
        return;
    }

    c->setX(pos.getX() + this->x * Chunk::W);
    c->setY(pos.getY() + this->y * Chunk::H);
    c->setZ(pos.getZ() + this->z * Chunk::Z);

    c->setChunkPos(pos);

    c->setChunk(this);

    this->cubes[pos.getX() + pos.getY() * Chunk::W + pos.getZ() * Chunk::H * Chunk::W] = c;

    this->updated = false;
}


std::vector<Cube *> Chunk::getCubes() {
    return this->cubes;
    //return NULL;
}

int Chunk::isIllated(int x, int y, int z) {
    Cube *c = NULL;
    Cube *k = this->world->getCube(this, x, y, z);
    int n = 1;

    if (k == NULL) {
        return 1;
    }

    c = this->world->getCube(this, x, y + 1, z);
    if (c != NULL && c->isTransparent() && k->getType() != c->getType()) {
        n *= 2;
    } else if (c == NULL) {
        n *= 2;
    }

    c = this->world->getCube(this, x, y - 1, z);
    if (c != NULL && c->isTransparent() && k->getType() != c->getType()) {
        n *= 3;
    } else if (c == NULL) {
        n *= 3;
    }

    c = this->world->getCube(this, x + 1, y, z);
    if (c != NULL && c->isTransparent() && k->getType() != c->getType()) {
        n *= 5;
    } else if (c == NULL) {
        n *= 5;
    }

    c = this->world->getCube(this, x - 1, y, z);
    if (c != NULL && c->isTransparent() && k->getType() != c->getType()) {
        n *= 7;
    } else if (c == NULL) {
        n *= 7;
    }

    c = this->world->getCube(this, x, y, z + 1);
    if (c != NULL && c->isTransparent() && k->getType() != c->getType()) {
        n *= 11;
    } else if (c == NULL) {
        n *= 11;
    }

    c = this->world->getCube(this, x, y, z - 1);
    if (c != NULL && c->isTransparent() && k->getType() != c->getType()) {
        n *= 13;
    } else if (c == NULL) {
        n *= 13;
    }

    k->setSides(n);
    return n;
}

void Chunk::getVisibleCubes() {
    if (this->updated == true) {
        return;
    }

    if (this->renderedCubes.size() != 0) {
        this->renderedCubes.clear();
    }

    this->world->genChunkAt(false, this->x + 1, this->y, this->z);
    this->world->genChunkAt(false, this->x - 1, this->y, this->z);
    this->world->genChunkAt(false, this->x, this->y, this->z + 1);
    this->world->genChunkAt(false, this->x, this->y, this->z - 1);
    this->world->genChunkAt(false, this->x, this->y - 1, this->z);
    this->world->genChunkAt(false, this->x, this->y + 1, this->z);

    for (int i = 0; i < this->W * this->H * this->Z; i++) {
        if (this->cubes[i]->getType() != CubeType::air) {
            if (this->isIllated(this->cubes[i]->getX(), this->cubes[i]->getY(), this->cubes[i]->getZ()) == 1) {
                //delete(this->cubes[i]);
                continue;
            }

            this->renderedCubes.push_back(this->cubes[i]);
        }
    }

    this->updateModel();

    this->updated = true;
}

void Chunk::updateModel() {
    this->v.clear();
    this->i.clear();
    for (std::size_t k = 0; k < this->renderedCubes.size(); k++) {
        this->renderedCubes[k]->getVertex(&this->v, &this->i, k);
    }
}


void Chunk::genVao() {
    if (this->VAO != 0) {
        glDeleteVertexArrays(1, &(this->VAO));
    }

    unsigned int VBO, EBO;

    this->getVisibleCubes();

    float *vertices = &this->v[0];
    int *indices = &this->i[0];

    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * i.size(), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Chunk::draw() {
    if (this->VAO == 0) return;


    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, 36 * this->renderedCubes.size(), GL_UNSIGNED_INT, 0);
}

Chunk::~Chunk() {
    this->Save();
    glDeleteVertexArrays(1, &(this->VAO));
    for (int i = 0; i < this->W * this->H * this->Z; i++) {
        delete (this->cubes[i]);
    }
    this->cubes.clear();
    this->renderedCubes.clear();
}


void Chunk::Save() {
    std::stringstream name;
    name << "saves/" << this->world->getName() << "/world/" << this->x << "_" << this->y << "_" << this->z << ".chunk";

    std::ofstream file(name.str());

    for (int i = 0; i < Chunk::W * Chunk::H * Chunk::Z; i++) {
        file << this->cubes[i]->getType() << "\t";
        FixedPosition p = this->cubes[i]->getChunkPos();
        file << p.getX() << "\t";
        file << p.getY() << "\t";
        file << p.getZ() << "\n";
    }

    file.close();
}

void Chunk::Load() {
    std::stringstream name;
    name << "saves/" << this->world->getName() << "/world/" << this->x << "_" << this->y << "_" << this->z << ".chunk";

    std::ifstream file(name.str(), std::ios::binary);

    if (file.is_open()) {
        while (!file.eof()) {
            int type, x, y, z;
            file >> type >> x >> y >> z;
            switch (type) {
                case CubeType::air:
                    this->setCube(new Air(), x, y, z);
                    break;
                case CubeType::dirt:
                    this->setCube(new Dirt(), x, y, z);
                    break;
                case CubeType::grassyDirt:
                    this->setCube(new GrassyDirt(), x, y, z);
                    break;
                case CubeType::sand:
                    this->setCube(new Sand(), x, y, z);
                    break;
                case CubeType::water:
                    this->setCube(new Water(), x, y, z);
                    break;
                case CubeType::lava:
                    this->setCube(new Lava(), x, y, z);
                    break;
                default:
                    this->setCube(new Stone(), x, y, z);
                    break;
            }
        }

        this->generated = true;
    }
}

World *Chunk::getWorld() {
    return this->world;
}
