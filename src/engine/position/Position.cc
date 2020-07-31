//
// Created by lonyelon on 31/7/20.
//

#include "Position.hpp"

#include <cmath>

Position::Position(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Position::Position(float x, float y, float z) {
    this->x = (int)round(x);
    this->y = (int)round(y);
    this->z = (int)round(z);
}

Position::Position(double x, double y, double z) {
    this->x = (int)round(x);
    this->y = (int)round(y);
    this->z = (int)round(z);
}

Position::Position() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

int Position::getX() const {
    return x;
}

void Position::setX(int x) {
    Position::x = x;
}

int Position::getY() const {
    return y;
}

void Position::setY(int y) {
    Position::y = y;
}

int Position::getZ() const {
    return z;
}

void Position::setZ(int z) {
    Position::z = z;
}

Position Position::move(Position desp) {
    int x = this->getX() + desp.getX();
    int y = this->getY() + desp.getY();
    int z = this->getZ() + desp.getZ();
    return Position(x, y, z);
}
