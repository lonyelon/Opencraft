#ifndef ENGINE_MOUSE_CAMERA_HPP
#define ENGINE_MOUSE_CAMERA_HPP

#include "../Engine.hpp"

class Player;

class Camera {
private:
    Player *player;

    float speed;
    float camRotX;
    float camRotY;

    float x, y, z;

    float correction = 0.01f;
public:
    Camera(Player *p);

    void moveCoords( int xpos, int ypos );
    void move(float x, float y, float z);

    glm::mat4 getViewMatrix();
    float getRotX();
    float getRotY();
    void setRotation(int x, int y);
    void setPos(float x, float y, float z);

    float getX(  ) { return this->x; }
    float getY(  ) { return this->y; }
    float getZ(  ) { return this->z; } 

    Position<float> getPosition() { return Position<float>(x, y, z); } 
};

#endif
