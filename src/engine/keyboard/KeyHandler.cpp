#include "KeyHandler.hpp"
#include "KeyConverter.hpp"

#include <cmath>
#include <vector>
#include <memory>

#include <engine/Engine.hpp>
#include <game/Game.hpp>

extern std::unique_ptr<Game> game;

KeyHandler::KeyHandler() {
    this->kc = new KeyConverter();
}

bool KeyHandler::isKeyPresent(int key) {
    for (auto k: this->keys) {
        if (k.key == key) {
            return true;
        }
    }

    return false;
}

void KeyHandler::addKey(int key) {
    if (!this->isKeyPresent(key)) {
        KeyPair k;
        k.key = key;
        k.pressed = true;
        k.name = "";

        this->keys.push_back(k);
    } else {
        for (std::size_t i = 0; i < this->keys.size(); i++) {
            if (this->keys[i].key == key) {
                this->keys[i].pressed = true;
            }
        }
    }
}

void KeyHandler::removeKey(int key) {
    for (std::size_t i = 0; i < this->keys.size(); i++) {
        std::string keyname = this->kc->getKeyName(this->keys[i].key);
        if (this->keys[i].key == key) {
            //this->keys.erase(this->keys.begin() + i);
            this->keys[i].pressed = false;
        }
    }
}

KeyConverter *KeyHandler::getKeyConverter() {
    return this->kc;
}

void KeyHandler::keyHandler() {
    for (size_t i = 0; i < this->keys.size(); i++) {
        auto k = this->keys[i];

        std::string keyname = this->kc->getKeyName(k.key);
        if (keyname.compare("") == 0 || kc == nullptr) {
            continue;
        }

        if (keyname.compare("move.jump") == 0 && k.pressed) {
            game->getPlayer()->jump();
            continue;
        }

        if (keyname.compare("move.sprint") == 0 && k.pressed) {
            game->getPlayer()->setSprint(true);
            continue;
        } else if (keyname.compare("move.sprint") == 0 && !k.pressed) {
            game->getPlayer()->setSprint(false);
            continue;
        }

        if (keyname.compare("move.forward") == 0 && k.pressed) {
            game->getPlayer()->move(-cos(game->getPlayer()->get_camera()->getRotX()), 0, sin(game->getPlayer()->get_camera()->getRotX()));
            continue;
        }

        if (keyname.compare("move.left") == 0 && k.pressed) {
            game->getPlayer()->move(
                    -cos(glm::half_pi<float>() + game->getPlayer()->get_camera()->getRotX()),
                    0,
                    sin(glm::half_pi<float>() + game->getPlayer()->get_camera()->getRotX())
            );
            continue;
        }

        if (keyname.compare("move.backward") == 0 && k.pressed) {
            game->getPlayer()->move(
                    cos(game->getPlayer()->get_camera()->getRotX()),
                    0,
                    -sin(game->getPlayer()->get_camera()->getRotX())
            );
            continue;
        }
        if (keyname.compare("move.right") == 0 && k.pressed) {
            game->getPlayer()->move(
                    cos(glm::half_pi<float>() + game->getPlayer()->get_camera()->getRotX()),
                    0,
                    -sin(glm::half_pi<float>() + game->getPlayer()->get_camera()->getRotX())
            );
            continue;
        }
        if (keyname.compare("special.worldgen") == 0 && k.pressed) {
            game->getWorld()->setSeed(rand() % 2000000);
            game->getWorld()->genChunks();
            this->keys.erase(this->keys.begin() + i);
            continue;
        }
    }
}
