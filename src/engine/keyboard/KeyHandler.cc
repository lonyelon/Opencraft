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
        k.pressed = true;
        k.name = "";

        this->keys.push_back(k);
    } else {
        for ( int i = 0; i < this->keys.size(); i++ ) {
            std::string keyname = this->kc->getKeyName( this->keys[i].key );
            if ( this->keys[i].key == key ) {
                this->keys[i].pressed = true;
            }
        }
    }
}

void KeyHandler::removeKey( int key ) {
    for ( int i = 0; i < this->keys.size(); i++ ) {
        std::string keyname = this->kc->getKeyName( this->keys[i].key );
        if ( this->keys[i].key == key ) {
            //this->keys.erase(this->keys.begin() + i);
            this->keys[i].pressed = false;
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

        if ( keyname.compare( "move.jump" ) == 0 && this->keys[i].pressed ) {
            p->jump();
            continue;
        }

        if ( keyname.compare( "move.sprint" ) == 0 && this->keys[i].pressed ) {
            p->setSprint(true);
            continue;
        } else if ( keyname.compare( "move.sprint" ) == 0 && !this->keys[i].pressed ) { 
            p->setSprint(false);
            continue;
        }

        if ( keyname.compare( "move.forward" ) == 0 && this->keys[i].pressed ) {
            p->move( 
                -cos(p->getCam()->getRotX()), 
                0, 
                sin(p->getCam()->getRotX())
            );
            continue;
        }

        if ( keyname.compare( "move.left" )  == 0 && this->keys[i].pressed ) {
            p->move( 
                -cos(glm::half_pi<float>()+p->getCam()->getRotX()), 
                0, 
                sin(glm::half_pi<float>()+p->getCam()->getRotX())
            );
            continue;
        }
        
        if ( keyname.compare( "move.backward" ) == 0 && this->keys[i].pressed ) {
            p->move( 
                cos(p->getCam()->getRotX()), 
                0, 
                -sin(p->getCam()->getRotX())
            );
            continue;
        }
        if ( keyname.compare( "move.right" ) == 0 && this->keys[i].pressed ) {
            p->move( 
                cos(glm::half_pi<float>()+p->getCam()->getRotX()), 
                0, 
                -sin(glm::half_pi<float>()+p->getCam()->getRotX()) 
            );
            continue;
        }
        if ( keyname.compare( "special.worldgen" ) == 0 && this->keys[i].pressed ) {
            world->setSeed( rand() % 2000000 );
            world->genChunks();
            this->keys.erase(this->keys.begin() + i);
            continue;
        }
    }
}