#include "KeyHandler.hpp"

#include "../glfw.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include "../../game/world/World.hpp"

extern Camera cam;

extern std::vector<Chunk*> chunks;
extern World *world;


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
        switch (this->keys[i].key) {
            case GLFW_KEY_W:
                cam.move( 
                    -cos(cam.getRotX())*cos(cam.getRotY()), 
                    sin(cam.getRotY()), 
                    sin(cam.getRotX())*cos(cam.getRotY()) 
                );
                break;
            case GLFW_KEY_A:
                cam.move( 
                    -cos(glm::half_pi<float>()+cam.getRotX())*cos(cam.getRotY()), 
                0, 
                    sin(glm::half_pi<float>()+cam.getRotX())*cos(cam.getRotY()) 
                );
                break;
            case GLFW_KEY_S:
                cam.move( 
                    cos(cam.getRotX())*cos(cam.getRotY()), 
                    -sin(cam.getRotY()), 
                    -sin(cam.getRotX())*cos(cam.getRotY()) 
                );
                break;
            case GLFW_KEY_D:
                cam.move( 
                    cos(glm::half_pi<float>()+cam.getRotX())*cos(cam.getRotY()), 
                    0, 
                    -sin(glm::half_pi<float>()+cam.getRotX())*cos(cam.getRotY()) 
                );
                break;
            case GLFW_KEY_SPACE:
                world->setSeed( rand() % 2000000 );
                world->genChunks();
                this->keys.erase(this->keys.begin() + i);
                break;
        }
    }
}