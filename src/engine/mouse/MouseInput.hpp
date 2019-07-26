#ifndef ENGINE_MOUSE_MOUSE_INPUT_HPP
#define ENGINE_MOUSE_MOUSE_INPUT_HPP

#include "../glfw.hpp"

void getMouseInput( GLFWwindow* window, double xpos, double ypos );
void getMouseButton( GLFWwindow* window, int button, int action, int mods );
#endif