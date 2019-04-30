#include "MouseInput.hpp"
#include "../../game/Player.hpp"

extern Player p;

void getMouseInput(GLFWwindow* window, double xpos, double ypos) {
	p.getCam()->moveCoords(xpos, ypos);
	 
	glfwSetCursorPos(window, 0, 0);
}