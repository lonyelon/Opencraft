#include "KeyHandler.hpp"
#include "KeyConverter.hpp"

#include "../glfw.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include "../../game/world/World.hpp"
#include "../../game/Player.hpp"

extern Player * p;

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
        std::string keyname = this->kc->getKeyName( this->keys[i].key );
        if ( this->keys[i].key == key ) {

            if (keyname.compare( "move.sprint" ) == 0) {
                p->setSprint(false);
            }

            this->keys.erase(this->keys.begin() + i);
        }
    }
}

KeyConverter *KeyHandler::getKeyConverter() {
    return this->kc;
}

void KeyHandler::keyHandler( ) {
    for (int i = 0; i < this->keys.size(); i++) {
        std::string keyname = this->kc->getKeyName( this->keys[i].key );
        if ( keyname.compare("") == 0 || kc == NULL ) {
            continue;
        }

        if ( keyname.compare( "move.sprint" ) == 0 ) {
            p->setSprint(true);
            break;
        }

        if ( keyname.compare( "move.forward" ) == 0 ) {
            p->getCam()->move( 
                -cos(p->getCam()->getRotX())*cos(p->getCam()->getRotY()), 
                sin(p->getCam()->getRotY()), 
                sin(p->getCam()->getRotX())*cos(p->getCam()->getRotY()) 
            );
            break;
        }

        if ( keyname.compare( "move.left" )  == 0) {
            p->getCam()->move( 
                -cos(glm::half_pi<float>()+p->getCam()->getRotX())*cos(p->getCam()->getRotY()), 
                0, 
                sin(glm::half_pi<float>()+p->getCam()->getRotX())*cos(p->getCam()->getRotY()) 
            );
        }
        
        if ( keyname.compare( "move.backward" ) == 0 ) {
            p->getCam()->move( 
                cos(p->getCam()->getRotX())*cos(p->getCam()->getRotY()), 
                -sin(p->getCam()->getRotY()), 
                -sin(p->getCam()->getRotX())*cos(p->getCam()->getRotY()) 
            );
        }
        if ( keyname.compare( "move.right" ) == 0 ) {
            p->getCam()->move( 
                cos(glm::half_pi<float>()+p->getCam()->getRotX())*cos(p->getCam()->getRotY()), 
                0, 
                -sin(glm::half_pi<float>()+p->getCam()->getRotX())*cos(p->getCam()->getRotY()) 
            );
        }
        if ( keyname.compare( "special.worldgen" ) == 0 ) {
            world->setSeed( rand() % 2000000 );
            world->genChunks();
            this->keys.erase(this->keys.begin() + i);
        }
    }
}