/*
 * Stores a position in the world as a 3-dimensional vector.
 *
 */

#ifndef OPENCRAFT_POSITION_HPP
#define OPENCRAFT_POSITION_HPP

#include <glm/glm.hpp>

template<typename T>
struct Position : glm::vec<3, int, glm::packed_highp>{
    Position() = default;

    Position(T x, T y, T z);

    [[deprecated]]
    Position<T> move(Position<T> desp);

    bool operator==(Position<T> other) const;

    bool operator!=(Position<T> other) const;

    bool operator<(Position<T> other) const;
};

template<typename T>
Position<T>::Position(T x, T y, T z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

template<typename T>
Position<T> Position<T>::move(Position<T> desp) {
    T nx = this->x + desp.x;
    T ny = this->y + desp.y;
    T nz = this->z + desp.z;
    return Position(nx, ny, nz);
}

template<typename T>
bool Position<T>::operator==(Position<T> other) const {
    return this->x == other.x && this->y == other.y && this->z == other.z;
}

template<typename T>
bool Position<T>::operator!=(Position<T> other) const {
    return !(*this == other);
}

template<typename T>
bool Position<T>::operator<(Position<T> other) const {
    return this->x < other.x || (this->x == other.x && this->y < other.y) ||
           (this->x == other.x && this->y == other.y && this->z < other.z);
}

#endif //OPENCRAFT_POSITION_HPP
