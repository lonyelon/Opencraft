#ifndef _KEY_HANDLER_HPP_
#define _KEY_HANDLER_HPP_

#include <vector>

#include "KeyType.hpp"

struct KeyPair {
    int key;
    bool pressed;
};

class KeyHandler {
private:
    std::vector<KeyPair> keys;
    bool isKeyPresent(int key);

public:
    KeyHandler();
    void addKey (int key);
    void removeKey (int key);
    void keyHandler ();
};

#endif