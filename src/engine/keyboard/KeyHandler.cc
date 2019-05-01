#include "KeyHandler.hpp"
#include "KeyConverter.hpp"

#include "../glfw.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include "../../game/world/World.hpp"
#include "../../game/Player.hpp"

extern Player p;

extern std::vector<Chunk*> chunks;
extern World *world;


KeyHandler::KeyHandler() {
    this->kc = new KeyConverter(  );
}

bool KeyHandler::isKeyPresent( int key ) {
    for ( int i = 0; i < this->keys.size(); i++ ) {
        if (this->keys[i].key == key) {
            return true;
        }
    }
    return false;
}

void KeyHandler::addKey( int key ) {
    if ( !this->isKeyPresent(key) ) {
        KeyPair k;
        k.key = key;
        k.pressed = false;
        k.name = "";

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

KeyConverter *KeyHandler::getKeyConverter() {
    return this->kc;
}

void KeyHandler::keyHandler( ) {
    for (int i = 0; i < this->keys.size(); i++) {
        switch ( this->keys[i].key ) {
            case GLFW_KEY_W:
                p.getCam()->move( 
                    -cos(p.getCam()->getRotX())*cos(p.getCam()->getRotY()), 
                    sin(p.getCam()->getRotY()), 
                    sin(p.getCam()->getRotX())*cos(p.getCam()->getRotY()) 
                );
                break;
            case GLFW_KEY_A:
                p.getCam()->move( 
                    -cos(glm::half_pi<float>()+p.getCam()->getRotX())*cos(p.getCam()->getRotY()), 
                0, 
                    sin(glm::half_pi<float>()+p.getCam()->getRotX())*cos(p.getCam()->getRotY()) 
                );
                break;
            case GLFW_KEY_S:
                p.getCam()->move( 
                    cos(p.getCam()->getRotX())*cos(p.getCam()->getRotY()), 
                    -sin(p.getCam()->getRotY()), 
                    -sin(p.getCam()->getRotX())*cos(p.getCam()->getRotY()) 
                );
                break;
            case GLFW_KEY_D:
                p.getCam()->move( 
                    cos(glm::half_pi<float>()+p.getCam()->getRotX())*cos(p.getCam()->getRotY()), 
                    0, 
                    -sin(glm::half_pi<float>()+p.getCam()->getRotX())*cos(p.getCam()->getRotY()) 
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