#ifndef ENGINE_MOUSE_CAMERA_HPP
#define ENGINE_MOUSE_CAMERA_HPP

#include "../glfw.hpp"

class Camera {
private:
    const float correction = 0.01f;

    float speed;
    float camRotX;
    float camRotY;

    float x, y, z;
public:
    Camera();

    void moveCoords( int xpos, int ypos );
    void move(float x, float y, float z);

    glm::mat4 getViewMatrix();
    float getRotX();
    float getRotY();
    void setRotation(int x, int y);
    void setPos(int x, int y, int z);
};

#endif