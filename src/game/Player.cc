#include "Player.hpp"
#include "world/World.hpp"

#include <cstdio>
#include <memory>

extern int pint;

Player::Player(std::weak_ptr<World> world) {
    this->cam = std::make_shared<Camera>(this);
    this->cam->setPos(0, 100, 0);
    this->world = world;
    this->vspd = 0;
    this->selectedCube = CubeType::dirt;

    this->setSprint(false);
}

void Player::move(float mx, float my, float mz) {
    const float top = 0.05;
    const float bot = 1.70;

    const float right = 0.25;
    const float left = 0.25;

    const float front = 0.25;
    const float back = 0.25;

    std::shared_ptr<Cube> c;

    mx *= this->movementSpeed;
    my *= this->movementSpeed;
    mz *= this->movementSpeed;

    //? Can fall
    c = this->world.lock()->getCube(this->cam->getX() + right, this->cam->getY() - bot + my, this->cam->getZ() + front);
    if (c != nullptr && !c->isTransparent() && my < 0) {
        my = 0;
        this->vspd = 0;
        //this->cam->setPos(this->cam->getX(), c->getY() + 0.5 + bot, this->cam->getZ());
    }
    c = this->world.lock()->getCube(this->cam->getX() + right, this->cam->getY() - bot + my, this->cam->getZ() - back);
    if (c != nullptr && !c->isTransparent() && my < 0) {
        my = 0;
        this->vspd = 0;
        //this->cam->setPos(this->cam->getX(), c->getY() + 0.5 + bot, this->cam->getZ());
    }
    c = this->world.lock()->getCube(this->cam->getX() - left, this->cam->getY() - bot + my, this->cam->getZ() + front);
    if (c != nullptr && !c->isTransparent() && my < 0) {
        my = 0;
        this->vspd = 0;
        //this->cam->setPos(this->cam->getX(), c->getY() + 0.5 + bot, this->cam->getZ());
    }
    c = this->world.lock()->getCube(this->cam->getX() - left, this->cam->getY() - bot + my, this->cam->getZ() - back);
    if (c != nullptr && !c->isTransparent() && my < 0) {
        my = 0;
        this->vspd = 0;
        //this->cam->setPos(this->cam->getX(), c->getY() + 0.5 + bot, this->cam->getZ());
    }

    //? Positive Y
    c = this->world.lock()->getCube(this->cam->getX() + right, this->cam->getY() + top + my, this->cam->getZ() + front);
    if (c != nullptr && !c->isTransparent() && my > 0) {
        my = 0;
        if (this->vspd > 0) this->vspd = 0;
    }
    c = this->world.lock()->getCube(this->cam->getX() + right, this->cam->getY() + top + my, this->cam->getZ() + front);
    if (c != nullptr && !c->isTransparent() && my > 0) {
        my = 0;
        if (this->vspd > 0) this->vspd = 0;
    }
    c = this->world.lock()->getCube(this->cam->getX() - left, this->cam->getY() + top + my, this->cam->getZ() - back);
    if (c != nullptr && !c->isTransparent() && my > 0) {
        my = 0;
        if (this->vspd > 0) this->vspd = 0;
    }
    c = this->world.lock()->getCube(this->cam->getX() - left, this->cam->getY() + top + my, this->cam->getZ() - back);
    if (c != nullptr && !c->isTransparent() && my > 0) {
        my = 0;
        if (this->vspd > 0) this->vspd = 0;
    }

    //? Positive Z
    c = this->world.lock()->getCube(this->cam->getX() + right, this->cam->getY() - bot, this->cam->getZ() + front + mz);
    if (c != nullptr && !c->isTransparent() && mz > 0) {
        mz = 0;
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()-0.5-front);
    }
    c = this->world.lock()->getCube(this->cam->getX() - left, this->cam->getY() - bot, this->cam->getZ() + front + mz);
    if (c != nullptr && !c->isTransparent() && mz > 0) {
        mz = 0;
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()-0.5-front);
    }
    c = this->world.lock()->getCube(this->cam->getX() + right, this->cam->getY() + top, this->cam->getZ() + front + mz);
    if (c != nullptr && !c->isTransparent() && mz > 0) {
        mz = 0;
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()-0.5-front);
    }
    c = this->world.lock()->getCube(this->cam->getX() - left, this->cam->getY() + top, this->cam->getZ() + front + mz);
    if (c != nullptr && !c->isTransparent() && mz > 0) {
        mz = 0;
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()-0.5-front);
    }

    //? Negative Z
    c = this->world.lock()->getCube(this->cam->getX() + right, this->cam->getY() - bot, this->cam->getZ() - back + mz);
    if (c != nullptr && !c->isTransparent() && mz < 0) {
        mz = 0;
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()+0.5+back);
    }
    c = this->world.lock()->getCube(this->cam->getX() - left, this->cam->getY() - bot, this->cam->getZ() - back + mz);
    if (c != nullptr && !c->isTransparent() && mz < 0) {
        mz = 0;
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()+0.5+back);
    }
    c = this->world.lock()->getCube(this->cam->getX() + right, this->cam->getY() + top, this->cam->getZ() - back + mz);
    if (c != nullptr && !c->isTransparent() && mz < 0) {
        mz = 0;
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()+0.5+back);
    }
    c = this->world.lock()->getCube(this->cam->getX() - left, this->cam->getY() + top, this->cam->getZ() - back + mz);
    if (c != nullptr && !c->isTransparent() && mz < 0) {
        mz = 0;
        //this->cam->setPos(this->cam->getX(), this->cam->getY(), c->getZ()+0.5+back);
    }

    //? Positive X
    c = this->world.lock()->getCube(this->cam->getX() + right + mx, this->cam->getY() - bot, this->cam->getZ() + front);
    if (c != nullptr && !c->isTransparent() && mx > 0) {
        mx = 0;
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world.lock()->getCube(this->cam->getX() + right + mx, this->cam->getY() - bot, this->cam->getZ() - back);
    if (c != nullptr && !c->isTransparent() && mx > 0) {
        mx = 0;
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world.lock()->getCube(this->cam->getX() + right + mx, this->cam->getY() + top, this->cam->getZ() + front);
    if (c != nullptr && !c->isTransparent() && mx > 0) {
        mx = 0;
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world.lock()->getCube(this->cam->getX() + right + mx, this->cam->getY() + top, this->cam->getZ() - back);
    if (c != nullptr && !c->isTransparent() && mx > 0) {
        mx = 0;
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }

    //? Negative X
    c = this->world.lock()->getCube(this->cam->getX() - left + mx, this->cam->getY() - bot, this->cam->getZ() + front);
    if (c != nullptr && !c->isTransparent() && mx < 0) {
        mx = 0;
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world.lock()->getCube(this->cam->getX() - left + mx, this->cam->getY() - bot, this->cam->getZ() - back);
    if (c != nullptr && !c->isTransparent() && mx < 0) {
        mx = 0;
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world.lock()->getCube(this->cam->getX() - left + mx, this->cam->getY() + top, this->cam->getZ() + front);
    if (c != nullptr && !c->isTransparent() && mx < 0) {
        mx = 0;
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }
    c = this->world.lock()->getCube(this->cam->getX() - left + mx, this->cam->getY() + top, this->cam->getZ() - back);
    if (c != nullptr && !c->isTransparent() && mx < 0) {
        mx = 0;
        //this->cam->setPos(c->getX()-0.5-right, this->cam->getY(), this->cam->getZ());
    }

    this->cam->move(mx, my, mz);
}

std::shared_ptr<Cube> Player::getPointer(float *x, float *y, float *z) const {
    const int iter = 1000;
    const int maxdist = 8;

    float rx = this->cam->getRotX();
    float ry = this->cam->getRotY();

    for (int i = 0; i < iter * maxdist; i++) {
        int xx = round(this->cam->getX() - i * cos(rx) * cos(ry) / iter);
        int yy = round(this->cam->getY() + i * sin(ry) / iter);
        int zz = round(this->cam->getZ() + i * sin(rx) * cos(ry) / iter);

        std::shared_ptr<Cube> c = this->world.lock()->getCube(xx, yy, zz);
        if (c != nullptr && !c->isTransparent()) {
            *x = this->cam->getX() - (i - 1) * cos(rx) * cos(ry) / iter;
            *y = this->cam->getY() + (i - 1) * sin(ry) / iter;
            *z = this->cam->getZ() + (i - 1) * sin(rx) * cos(ry) / iter;

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

    std::shared_ptr<Cube> c0 = this->world.lock()->getCube(x, y, z);
    if (c0 == nullptr) {
        return;
    }

    Position<int> pos = c0->getChunkPos();
    c0->getChunk()->setCube(getCubeObj(this->selectedCube), pos);
    c0->getChunk()->getVisibleCubes();
    c0->getChunk()->genVao();
}

std::shared_ptr<Camera> Player::getCam() {
    return this->cam;
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
    if (this->vspd == 0)
        this->vspd = 3;
}
