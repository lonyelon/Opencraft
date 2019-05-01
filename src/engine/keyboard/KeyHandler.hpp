#ifndef _KEY_HANDLER_HPP_
#define _KEY_HANDLER_HPP_

#include <vector>
#include <string>

#include "KeyConverter.hpp"

struct KeyPair {
    std::string name;
    int key;
    bool pressed;
};

class KeyHandler {
private:
    KeyConverter *kc;
    std::vector<KeyPair> keys;
    bool isKeyPresent(int key);

public:
    KeyHandler();
    void addKey (int key);
    void removeKey (int key);
    void keyHandler ();

    KeyConverter *getKeyConverter();
};

#endif