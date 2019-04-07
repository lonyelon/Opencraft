#include "engine/glfw.hpp"

#include "engine/keyboard/KeyboardInput.hpp"
#include "game/world/Chunk.hpp"
#include "game/world/World.hpp"

#include <string>
#include <thread>

Camera cam = Camera();
World *world;

KeyHandler k = KeyHandler();

void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;

int seed = 0;

extern GLuint setShaders(const char *nVertx, const char *nFrag);
GLuint shaderProgram;

void openGlInit() {
	glClearDepth(1.0f);
	glClearColor(0.2f, 0.2f, 1.0f, 1.0f); 

	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
}

void windowResize(GLFWwindow *window, int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

int main() {
	world = new World(  );
	srand(time(NULL));

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
														
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Clases", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, getKeyboardInput);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	glfwSetCursorPosCallback(window, getMouseInput);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowSizeCallback (window, windowResize);
	glfwSetFramebufferSizeCallback (window, windowResize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	shaderProgram = setShaders("./bin/shaders/shader.vert", "./bin/shaders/shader.frag");
	glUseProgram(shaderProgram);
	
	openGlInit();
	
	world->genChunks();
	printf("World generation completed\n");

	cam.setPos(0, 90, 0);
	cam.setRotation( glm::half_pi<float>() , glm::half_pi<float>()/3 );

	unsigned int windowSizeLoc = glGetUniformLocation(shaderProgram, "windowSize");
	glUniform2f(windowSizeLoc, SCR_WIDTH, SCR_HEIGHT);

	while (!glfwWindowShouldClose(window)) {
		double t = glfwGetTime();
		processInput(window);
		k.keyHandler();

		glm::mat4 view; // Se Calcula
		glm::mat4 projection; // Se calcula

		view = cam.getViewMatrix();
		projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 1000.0f);

		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		world->draw();
		 		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}