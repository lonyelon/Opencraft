/*
 * Stores a position in the world as a 3-dimensional vector.
 *
 */

#ifndef OPENCRAFT_POSITION_HPP
#define OPENCRAFT_POSITION_HPP

#include <glm/glm.hpp>

template<typename T>
struct Position : glm::vec<3, T, glm::packed_highp> {
    Position() = default;

    Position(T x, T y, T z);

    bool operator==(Position<T> other) const;

    bool operator!=(Position<T> other) const;

    bool operator<(Position<T> other) const;

    Position<T> operator+(Position<T> other) const;

    void operator+=(Position<T> other);
};

template<typename T>
Position<T>::Position(T x, T y, T z) {
    this->x = x;
    this->y = y;
    this->z = z;
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

template<typename T>
Position<T> Position<T>::operator+(Position<T> other) const {
    return Position<T>(
        this->x + other.x,
        this->y + other.y,
        this->z + other.z
    );
}

template<typename T>
void Position<T>::operator+=(Position<T> other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
}

#endif //OPENCRAFT_POSITION_HPP
