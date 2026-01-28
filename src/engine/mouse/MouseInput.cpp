#include "MouseInput.hpp"

#include <game/Game.hpp>
#include <memory>

extern std::unique_ptr<Game> game;

void getMouseInput(GLFWwindow* window, double xpos, double ypos) {
	game->getPlayer()->get_camera()->moveCoords(xpos, ypos);
	 
	glfwSetCursorPos(window, 0, 0);
}

void getMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		game->getPlayer()->breakCube();
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		game->getPlayer()->placeCube();
	}

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        game->getPlayer()->selectCube();
    }
}
