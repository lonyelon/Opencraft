#include "MouseInput.hpp"

extern Camera cam;

void getMouseInput(GLFWwindow* window, double xpos, double ypos) {
	cam.moveCoords(xpos, ypos);
	 
	glfwSetCursorPos(window, 0, 0);
}