#include "KeyHandler.hpp"

#include "../glfw.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include "../../game/Chunk.hpp"

extern float camX;
extern float camY;
extern float camZ;

extern float camRotX;
extern float camRotY;

extern void genChunks(std::vector<Chunk> *c);
extern std::vector<Chunk> c;
extern int seed;


KeyHandler::KeyHandler() {

}

bool KeyHandler::isKeyPresent( int key ) {
    for (int i = 0; i < this->keys.size(); i++) {
        if (this->keys[i].key == key) {
            return true;
        }
    }
    return false;
}

void KeyHandler::addKey( int key ) {
    if (!this->isKeyPresent(key)) {
        KeyPair k;
        k.key = key;
        k.pressed = false;

        this->keys.push_back(k);
    }
}

void KeyHandler::removeKey( int key ) {
    for ( int i = 0; i < this->keys.size(); i++ ) {
        if ( this->keys[i].key == key ) {
            this->keys.erase(this->keys.begin() + i);
        }
    }
}

void KeyHandler::keyHandler () {
    for (int i = 0; i < this->keys.size(); i++) {
        //if (this->keys[i].pressed) {
            switch (this->keys[i].key) {
                case GLFW_KEY_W:
                    camX -= cos(camRotX)*cos(camRotY);
                    camY += sin(camRotY);
                    camZ += sin(camRotX)*cos(camRotY);
                    break;
                case GLFW_KEY_A:
                    camX -= cos(camRotX+glm::half_pi<float>())*cos(camRotY);
                    //camY += sin(camRotY);
                    camZ += sin(camRotX+glm::half_pi<float>())*cos(camRotY);
                    break;
                case GLFW_KEY_S:
                    camX += cos(camRotX)*cos(camRotY);
                    camY -= sin(camRotY);
                    camZ -= sin(camRotX)*cos(camRotY);
                    break;
                case GLFW_KEY_D:
                    camX += cos(camRotX+glm::half_pi<float>())*cos(camRotY);
                    //camY += sin(camRotY);
                    camZ -= sin(camRotX+glm::half_pi<float>())*cos(camRotY);
                    break;
                case GLFW_KEY_Q:
                    camY += 0.1;
                    break;
                case GLFW_KEY_E:
                    camY -= 0.1;
                    break;
                case GLFW_KEY_SPACE:
                    seed = rand() % 200000000;
                    genChunks(&c);
                    this->keys.erase(this->keys.begin() + i);
                    break;
            }
        //}
    }
}