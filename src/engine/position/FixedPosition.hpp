//
// Created by lonyelon on 31/7/20.
//

#ifndef OPENCRAFT_FIXEDPOSITION_HPP
#define OPENCRAFT_FIXEDPOSITION_HPP

#include <engine/position/Position.hpp>

class FixedPosition {
private:
    int x, y, z;
public:
    // Constructor that sets the coordinates to 0.
    FixedPosition();

    // Constructor with coordinates.
    FixedPosition(int x, int y, int z);

    // Same as with ints but with floats.
    FixedPosition(float x, float y, float z);

    // Same as with ints but with doubles.
    FixedPosition(double x, double y, double z);

    // Moves the coordinates of the current position and returns a new one with the new coordinates.
    FixedPosition move(FixedPosition desp);

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);

    int getZ() const;

    void setZ(int z);

};


#endif //OPENCRAFT_FIXEDPOSITION_HPP
