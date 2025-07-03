#include "Camera.hpp"

#include "../../game/Player.hpp"

Camera::Camera( Player * p ) {
    this->speed = 1.0f/200.0f;
    this->camRotX = 0;
    this->camRotY = 0;
    this->player = p;
}

void Camera::moveCoords( int xpos, int ypos ) {
    this->camRotY -= ypos*this->speed;
    this->camRotX -= xpos*this->speed;

    if (this->camRotY <= -glm::half_pi<float>())
        this->camRotY = -glm::half_pi<float>() + this->correction;

    if (this->camRotY >= glm::half_pi<float>())
        this->camRotY = glm::half_pi<float>() - this->correction;
}

void Camera::move(float x, float y, float z) {
    this->x += x;
    this->y += y;
    this->z += z;
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(
        glm::vec3(this->x, this->y, this->z), 
        glm::vec3(this->x - cos(this->camRotX)*cos(this->camRotY), 
        this->y + sin(this->camRotY), 
        this->z + sin(this->camRotX)*cos(this->camRotY)), 
        glm::vec3(0,1,0)
    );
}

float Camera::getRotX() {
    return this->camRotX;
}

float Camera::getRotY() {
    return this->camRotY;
}

void Camera::setRotation(int x, int y) {
    this->camRotX = x;
    this->camRotY = y;
}

void Camera::setPos(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
