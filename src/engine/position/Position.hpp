/*
 * Stores a position in the world as a 3-dimensional vector.
 *
 */

#ifndef OPENCRAFT_POSITION_HPP
#define OPENCRAFT_POSITION_HPP

template<typename T>
class Position {
private:
    T x, y, z;
public:
    // Constructor that sets the coordinates to 0.
    Position();

    // Constructor with coordinates.
    Position(T x, T y, T z);

    // Moves the coordinates of the current position and returns a new one with the new coordinates.
    Position<T> move(Position<T> desp);

    T getX();

    void setX(T x);

    T getY();

    void setY(T y);

    T getZ();

    void setZ(T z);

};

template<typename T>
Position<T>::Position(T x, T y, T z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

template<typename T>
Position<T>::Position() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

template<typename T>
T Position<T>::getX() {
    return this->x;
}

template<typename T>
void Position<T>::setX(T x) {
    Position::x = x;
}

template<typename T>
T Position<T>::getY() {
    return this->y;
}

template<typename T>
void Position<T>::setY(T y) {
    Position::y = y;
}

template<typename T>
T Position<T>::getZ() {
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


#endif //OPENCRAFT_POSITION_HPP
