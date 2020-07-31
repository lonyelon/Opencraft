//
// Created by lonyelon on 31/7/20.
//

#include "FixedPosition.hpp"

#include <cmath>

FixedPosition::FixedPosition(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

FixedPosition::FixedPosition(float x, float y, float z) {
    this->x = (int)round(x);
    this->y = (int)round(y);
    this->z = (int)round(z);
}

FixedPosition::FixedPosition(double x, double y, double z) {
    this->x = (int)round(x);
    this->y = (int)round(y);
    this->z = (int)round(z);
}

FixedPosition::FixedPosition() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

int FixedPosition::getX() const {
    return x;
}

void FixedPosition::setX(int x) {
    FixedPosition::x = x;
}

int FixedPosition::getY() const {
    return y;
}

void FixedPosition::setY(int y) {
    FixedPosition::y = y;
}

int FixedPosition::getZ() const {
    return z;
}

void FixedPosition::setZ(int z) {
    FixedPosition::z = z;
}

FixedPosition FixedPosition::move(FixedPosition desp) {
    int nx = this->getX() + desp.getX();
    int ny = this->getY() + desp.getY();
    int nz = this->getZ() + desp.getZ();
    return FixedPosition(nx, ny, nz);
}
