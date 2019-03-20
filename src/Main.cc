#include "engine/glfw.hpp"

#include "engine/keyboard/KeyboardInput.hpp"
#include "game/Chunk.hpp"

#include <string>


KeyHandler k = KeyHandler();

void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

bool pressingW = false;
bool pressingA = false;
bool pressingS = false;
bool pressingD = false;

float camX = 0, camY = 50, camZ = 0;
int mDifX = 0, mDifY = 0;
float camRotX = 0, camRotY = 0;

int seed = 123456789;

std::vector<Chunk> c;

extern GLuint setShaders(const char *nVertx, const char *nFrag);
GLuint shaderProgram;

unsigned int VAO;
unsigned int VAOTriangulo;

void drawCube() {
	unsigned int VBO, EBO;
	// set up vertex data and buffer 

	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 	0.4,0.4,0.4, // back
		0.5f, -0.5f, 0.5f,		0.6,0.6,0.6,
		0.5f, 0.5f, 0.5f, 		0,1,0,
		-0.5f, 0.5f, 0.5f, 		0,0.25,0,
		-0.5f, 0.5f, -0.5f, 	0,0.5,0, // top
		0.5f, 0.5f, -0.5f, 		0,0.75,0,
		0.5f, -0.5f, -0.5f, 	1,1,1, // right
		-0.5f, -0.5f, -0.5f, 	0.8,0.8,0.8, // right
	};
	unsigned int indices[] = {  // empieza desde cero, repetir indices
		0, 1, 2, 0, 2, 3, // back
		3, 2, 4, 4, 2, 5, // top
		1, 5, 2, 1, 6, 5, // left
		0, 3, 4, 0, 4, 7, // right
		1, 0, 7, 1, 7, 6, // bot
		5, 6, 7, 4, 5, 7, // front
	};

	glGenVertexArrays(1, &VAOTriangulo);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOTriangulo);

		// bind the Vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// position de los attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// position del color Color

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// ya no no necesito el VAo y hago el un unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//Borro los buffers
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}

void openGlInit() {
	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 1.0f, 1.0f); 

	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	const float correction = 0.01f;
	const float red = 200;

	camRotY -= ypos/red;
	camRotX -= xpos/red;

	if (camRotY <= -glm::half_pi<float>()) camRotY = -glm::half_pi<float>()+correction;
	if (camRotY >= glm::half_pi<float>()) camRotY = glm::half_pi<float>()-correction;
	 
	glfwSetCursorPos(window, 0, 0);
}

void genChunks(std::vector<Chunk> *c) {
	c->clear();
	const int size = 10;
	for ( int x = 0; x < size; x++ ) {
		for ( int y = 0; y < size; y++ ) {
			c->push_back(Chunk(x, 0, y));
			(*c)[x*size+y].genTerrain();
			printf("Generating chunk %i/%i\n", y+size*x, size*size);
		}
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float temp = 0;
														
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Clases", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, getKeyboardInput);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	shaderProgram = setShaders("./bin/shaders/shader.vert", "./bin/shaders/shader.frag");
	
	drawCube();
	openGlInit();

	genChunks(&c);
	printf("World generation completed\n");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		k.keyHandler();

		if (pressingW) {
			camZ -= 0.02;
		}

		glm::mat4 view; // Se Calcula
		glm::mat4 projection; // Se calcula

		view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(camX - cos(camRotX)*cos(camRotY), camY + sin(camRotY), camZ + sin(camRotX)*cos(camRotY)), glm::vec3(0,1,0));
		projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 1000.0f);

		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (int k = 0; k < 10*10; k++) {
			c[k].draw();
		}
				
		glBindVertexArray(0); // no need to unbind it every time 
		 		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	
	glfwTerminate();
	return 0;
}

// input

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	

}

