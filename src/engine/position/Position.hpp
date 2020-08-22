/*
 * Stores a position in the world as a 3-dimensional vector.
 *
 */

#ifndef OPENCRAFT_POSITION_HPP
#define OPENCRAFT_POSITION_HPP

template<typename T>
class Position {
private:
    T x = 0, y = 0, z = 0;
public:
    // Constructor that sets the coordinates to 0.
    Position() = default;

    // Constructor with coordinates.
    Position(T x, T y, T z) : x(x), y(y), z(z) {};

    // Moves the coordinates of the current position and returns a new one with the new coordinates.
    Position<T> move(Position<T> desp);

    T getX() const;

    void setX(T x);

    T getY() const;

    void setY(T y);

    T getZ() const;

    void setZ(T z);

    bool operator==(Position<T> other) const;

    bool operator<(Position<T> other) const;
};

template<typename T>
T Position<T>::getX() const {
    return this->x;
}

template<typename T>
void Position<T>::setX(T x) {
    Position::x = x;
}

template<typename T>
T Position<T>::getY() const {
    return this->y;
}

template<typename T>
void Position<T>::setY(T y) {
    Position::y = y;
}

template<typename T>
T Position<T>::getZ() const {
    return this->z;
}

template<typename T>
void Position<T>::setZ(T z) {
    Position::z = z;
}

template<typename T>
Position<T> Position<T>::move(Position<T> desp) {
    T nx = this->getX() + desp.getX();
    T ny = this->getY() + desp.getY();
    T nz = this->getZ() + desp.getZ();
    return Position(nx, ny, nz);
}

template<typename T>
bool Position<T>::operator==(Position<T> other) const {
    return this->getX() == other.getX() && this->getY() == other.getY() && this->getZ() == other.getZ();
}

template<typename T>
bool Position<T>::operator<(Position<T> other) const {
    return this->getX() < other.getX() || (this->getX() == other.getX() && this->getY() < other.getY()) ||
           (this->getX() == other.getX() && this->getY() == other.getY() && this->getZ() < other.getZ());
}

#endif //OPENCRAFT_POSITION_HPP
