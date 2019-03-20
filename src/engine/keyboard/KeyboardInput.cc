#include "KeyboardInput.hpp"

#include "../glfw.hpp"
#include <stdio.h>
#include <iostream>

extern KeyHandler k;
 
void getKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if ( action == GLFW_PRESS ) {
        k.addKey(key);
    } else if ( action == GLFW_RELEASE ) {
        k.removeKey(key);
    }
}

