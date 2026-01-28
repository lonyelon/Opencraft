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

    float correction = 0.01f;
public:
    float x, y, z;

    Camera(Player *p);

    void moveCoords( int xpos, int ypos );
    void move(float x, float y, float z);

    glm::mat4 getViewMatrix();

    float getRotX();

    float getRotY();

    void setRotation(int x, int y);

    void setPos(float x, float y, float z);

    Position<float> getPosition() { return Position<float>(x, y, z); } 
};

#endif
