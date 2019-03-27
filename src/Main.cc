#include "engine/glfw.hpp"

#include "engine/keyboard/KeyboardInput.hpp"
#include "game/Chunk.hpp"

#include <string>
#include <thread>

Camera cam = Camera();

KeyHandler k = KeyHandler();

void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;

bool pressingW = false;
bool pressingA = false;
bool pressingS = false;
bool pressingD = false;

int seed = 123456789;

int chunkCount = 0;

std::vector<Chunk*> c;

extern GLuint setShaders(const char *nVertx, const char *nFrag);
GLuint shaderProgram;

unsigned int VAO;
unsigned int VAOTriangulo;

void openGlInit() {
	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 1.0f, 1.0f); 

	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
}

void genChunk(std::vector<Chunk*> *c, int jump, int size) {
	for ( int x = 0; x < size; x++ ) {
		for ( int y = jump-1; y < size; y += jump ) {
			c->push_back(new Chunk(x, 0, y));
			(*c)[x*size+y]->genTerrain();
			printf("Generating chunk %i/%i\n", y+size*x+1, size*size);
			chunkCount++;
		}
	}
}

void genChunks(std::vector<Chunk*> *c) {
	for (int i = 0; i < c->size(); i++) {
		delete((*c)[i]);
	}
	c->clear();
	const int size = 16;
	const int threadCount = 1;
	//std::thread t(genChunk, c, 1, size);
	//t.join();
	genChunk(c, 1, size);
}

void windowResize(GLFWwindow *window, int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float temp = 0;
														
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
	
	openGlInit();

	genChunks(&c);
	printf("World generation completed\n");
	glUseProgram(shaderProgram);

	glBindVertexArray(VAOTriangulo);

	cam.setPos(0, 90, 0);
	cam.setRotation( glm::half_pi<float>() , glm::half_pi<float>()/3 );

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

		for (int k = 0; k < chunkCount; k++) {
			c[k]->draw();
		}
		 		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}