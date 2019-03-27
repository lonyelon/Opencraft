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
    Camera() {
        this->speed = 1.0f/200.0f;
        this->camRotX = 0;
        this->camRotY = 0;
    }

    void moveCoords( int xpos, int ypos ) {
        camRotY -= ypos*this->speed;
        camRotX -= xpos*this->speed;

        if ( camRotY <= -glm::half_pi<float>() ) {
            camRotY = -glm::half_pi<float>() + this->correction;
        }

        if ( camRotY >= glm::half_pi<float>() ) {
            camRotY = glm::half_pi<float>() - this->correction;
        }
    }

    void move(float x, float y, float z) {
        this->x += x;
        this->y += y;
        this->z += z;
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(
            glm::vec3(this->x, this->y, this->z), 
            glm::vec3(this->x - cos(this->camRotX)*cos(this->camRotY), 
            this->y + sin(this->camRotY), 
            this->z + sin(this->camRotX)*cos(this->camRotY)), 
            glm::vec3(0,1,0)
        );
    }

    float getRotX() {
        return this->camRotX;
    }

    float getRotY() {
        return this->camRotY;
    }

    void setRotation(int x, int y) {
        this->camRotX = x;
        this->camRotY = y;
    }

    void setPos(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

#endif