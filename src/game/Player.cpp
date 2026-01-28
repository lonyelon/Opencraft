#include "Player.hpp"
#include "world/World.hpp"

#include <cstdio>
#include <memory>

extern int pint;

Player::Player(std::weak_ptr<World> world) {
    this->camera = std::make_shared<Camera>(this);
    this->camera->setPos(0, 100, 0);
    this->world = world;
    this->vspd = 0;
    this->selectedCube = CubeType::dirt;

    this->setSprint(false);
}

void Player::move(float movement_x, float movement_y, float movement_z) {
    const float T = 0.05;
    const float B = 1.70;
    const float W = 0.6;
    const float Z = 0.6;

    movement_x *= this->movementSpeed;
    movement_y *= this->movementSpeed;
    movement_z *= this->movementSpeed;

    auto camera_position = Position(this->camera->x,
                                    this->camera->y,
                                    this->camera->z);

    std::shared_ptr<Cube> c;

    //? Can fall
    for (auto x = -W/2.0f; x <= W/2.0f; x += W/4.0f) {
        for (auto z = -Z/2.0f; z <= Z/2.0f; z += Z/4.0f) {
            c = this->world.lock()->get_cube(camera_position + Position(x, -B + movement_y, z));
            if (c != nullptr && !c->isTransparent() && movement_y < 0) {
                movement_y = 0;
                this->vspd = 0;
                can_jump = true;
            }
        }
    }

    for (auto x = -W/2.0f; x <= W/2.0f; x += W/4.0f) {
        for (auto z = -Z/2.0f; z <= Z/2.0f; z += Z/4.0f) {
            c = this->world.lock()->get_cube(camera_position + Position(x, T + movement_y, z));
            if (c != nullptr && !c->isTransparent() && movement_y < 0) {
                movement_y = 0;
                this->vspd = 0;
            }
        }
    }

    // FIXME: Autojump is not smooth at all.
    // FIXME: Autojump has to check if space is available, not only the primitive
    //        check done here.
    bool can_autojump = true;
    if (movement_z != 0) {
        for (auto y = -B; y <= T; y += (B+T)/10.0f) {
            for (auto x = -W/2.0f; x <= W/2.0f; x += W/4.0f) {
                auto z = movement_z > 0 ? 1.0f : -1.0f;
                c = this->world.lock()->get_cube(camera_position + Position(x, y, z*Z/2.0f + movement_z));
                if (c != nullptr && !c->isTransparent()) {
                    c = this->world.lock()->get_cube(c->getPos() + Position(0, 1, 0));
                    if (c != nullptr && c->isTransparent()) {
                        movement_y = 1.0f/Cube::size_reduction;
                        this->can_jump = false;
                    } else {
                        movement_z = 0;
                        break;
                    }
                }
            }
            if (movement_z == 0)
                break;
        }
    }

    if (movement_x != 0) {
        for (auto y = -B; y <= T; y += (B+T)/10.0f) {
            for (auto z = -Z/2.0f; z <= Z/2.0f; z += Z/4.0f) {
                auto x = movement_x > 0 ? 1.0f : -1.0f;
                c = this->world.lock()->get_cube(camera_position + Position(x*W/2.0f + movement_x, y, z));
                if (c != nullptr && !c->isTransparent()) {
                    c = this->world.lock()->get_cube(c->getPos() + Position(0, 1, 0));
                    if (c != nullptr && c->isTransparent()) {
                        movement_y = 1.0f/Cube::size_reduction;
                        this->can_jump = false;
                    } else {
                        movement_x = 0;
                    }
                }
            }
            if (movement_x == 0)
                break;
        }
    }

    this->camera->move(movement_x, movement_y, movement_z);
}

std::shared_ptr<Cube> Player::getPointer(float *x, float *y, float *z) const {
    const int iter = 1000;
    const int maxdist = 8;

    float rx = this->camera->getRotX();
    float ry = this->camera->getRotY();

    for (int i = 0; i < iter * maxdist; i++) {
        float xx = this->camera->x - i * cos(rx) * cos(ry) / iter;
        float yy = this->camera->y + i * sin(ry) / iter;
        float zz = this->camera->z + i * sin(rx) * cos(ry) / iter;

        std::shared_ptr<Cube> c = this->world.lock()->get_cube(Position(xx, yy, zz));
        if (c != nullptr && !c->isTransparent()) {
            *x = this->camera->x - (i - 1) * cos(rx) * cos(ry) / iter;
            *y = this->camera->y + (i - 1) * sin(ry) / iter;
            *z = this->camera->z + (i - 1) * sin(rx) * cos(ry) / iter;

            return c;
        }
    }
    return nullptr;
}

void Player::selectCube() {
    std::shared_ptr<Cube> c = this->getPointedCube();
    if (c != nullptr) {
        this->selectedCube = c->getType();
    }
}

std::shared_ptr<Cube> Player::getPointedCube() const {
    float x, y, z;
    return this->getPointer(&x, &y, &z);
}

bool Player::getPointedPosition(float *x, float *y, float *z) const {
    if (this->getPointer(x, y, z) == nullptr) {
        return false;
    }
    return true;
}

void Player::breakCube() const {
    std::shared_ptr<Cube> c = this->getPointedCube();
    if (c == nullptr) {
        return;
    }

    Position<int> pos = c->getChunkPos();
    c->getChunk()->setCube(std::make_shared<Air>(), pos);
    c->getChunk()->getVisibleCubes();
    c->getChunk()->genVao();

    Chunk *c0 = c->getChunk();

    // TODO: Move this to the world or chunk class

    if (c->getX() % Chunk::W == 15 || c->getX() % Chunk::W == -1) {
        this->world.lock()->getChunk(c0->getX() + 1, c0->getY(), c0->getZ())->getVisibleCubes();
        this->world.lock()->getChunk(c0->getX() + 1, c0->getY(), c0->getZ())->genVao();
    }

    if (c->getZ() % Chunk::Z == 15 || c->getZ() % Chunk::Z == -1) {
        this->world.lock()->getChunk(c0->getX(), c0->getY(), c0->getZ() + 1)->getVisibleCubes();
        this->world.lock()->getChunk(c0->getX(), c0->getY(), c0->getZ() + 1)->genVao();
    }

    if (c->getY() % Chunk::H == 15 || c->getY() % Chunk::H == -1) {
        this->world.lock()->getChunk(c0->getX(), c0->getY() + 1, c0->getZ())->getVisibleCubes();
        this->world.lock()->getChunk(c0->getX(), c0->getY() + 1, c0->getZ())->genVao();
    }

    if (c->getX() % Chunk::W == 0) {
        this->world.lock()->getChunk(c0->getX() - 1, c0->getY(), c0->getZ())->getVisibleCubes();
        this->world.lock()->getChunk(c0->getX() - 1, c0->getY(), c0->getZ())->genVao();
    }

    if (c->getZ() % Chunk::Z == 0) {
        this->world.lock()->getChunk(c0->getX(), c0->getY(), c0->getZ() - 1)->getVisibleCubes();
        this->world.lock()->getChunk(c0->getX(), c0->getY(), c0->getZ() - 1)->genVao();
    }

    if (c->getY() % Chunk::H == 0) {
        this->world.lock()->getChunk(c0->getX(), c0->getY() - 1, c0->getZ())->getVisibleCubes();
        this->world.lock()->getChunk(c0->getX(), c0->getY() - 1, c0->getZ())->genVao();
    }
}

/*
	TODO: Optimize for cubes as objects
*/
void Player::placeCube() const {
    std::shared_ptr<Cube> c = this->getPointedCube();
    if (c == nullptr) {
        return;
    }

    float x, y, z;
    if (this->getPointedPosition(&x, &y, &z) == 0) {
        return;
    }

    std::shared_ptr<Cube> c0 = this->world.lock()->get_cube(Position(x, y, z));
    if (c0 == nullptr) {
        return;
    }

    Position<int> pos = c0->getChunkPos();
    c0->getChunk()->setCube(getCubeObj(this->selectedCube), pos);
    c0->getChunk()->getVisibleCubes();
    c0->getChunk()->genVao();
}

std::shared_ptr<Camera> Player::get_camera() {
    return this->camera;
}

void Player::setSprint(bool sprint) {
    float sprintSpeed = 0.07f;
    float normalSpeed = 0.03f;

    if (sprint) {
        if (this->vspd == 0 && this->movementSpeed < sprintSpeed) {
            this->movementSpeed = sprintSpeed + 0.00001f;
        }
    } else {
        this->movementSpeed = normalSpeed;
    }
}

void Player::gravity(float ammount) {
    if (this->vspd > -8)
        this->vspd -= ammount;

    this->move(0, this->vspd, 0);
}

void Player::jump() {
    if (this->can_jump) {
        this->vspd = 2.25;
        this->can_jump = false;
    }
}
