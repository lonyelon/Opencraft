#include "MouseInput.hpp"
#include "../../game/Player.hpp"

extern Player * p;

void getMouseInput(GLFWwindow* window, double xpos, double ypos) {
	p->getCam()->moveCoords(xpos, ypos);
	 
	glfwSetCursorPos(window, 0, 0);
}

void getMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		p->breakCube();
	}
}