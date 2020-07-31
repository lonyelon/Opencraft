//
// Created by lonyelon on 31/7/20.
//

#ifndef OPENCRAFT_POSITION_HPP
#define OPENCRAFT_POSITION_HPP


class Position {
private:
    int x, y, z;
public:
    // Constructor that sets the coordinates to 0.
    Position();

    // Constructor with coordinates.
    Position(int x, int y, int z);

    // Same as with ints but with floats.
    Position(float x, float y, float z);

    // Same as with ints but with doubles.
    Position(double x, double y, double z);

    // Moves the coordinates of the current position and returns a new one with the new coordinates.
    Position move(Position desp);

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);

    int getZ() const;

    void setZ(int z);

};
#endif //OPENCRAFT_POSITION_HPP
