#include "KeyboardInput.hpp"

#include <game/Game.hpp>

extern std::unique_ptr<Game> game;
 
void getKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if ( action == GLFW_PRESS ) {
        game->k.addKey(key);
    } else if ( action == GLFW_RELEASE ) {
        game->k.removeKey(key);
    }
}

