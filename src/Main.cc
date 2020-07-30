#include <engine/glfw.hpp>

#include <engine/keyboard/KeyboardInput.hpp>
#include <game/world/Chunk.hpp>
#include <game/world/World.hpp>
#include <engine/LoadTexture.hpp>
#include <engine/config/ConfigLoader.hpp>
#include <game/world/Sphere.h>
#include <game/Player.hpp>
#include <engine/model/ModelLoader.hpp>

#include <string>
#include <boost/thread.hpp>

int pint = 0;

World *world;
Player *p;
Model *cubeModel, *grassModel, *fluidModel;
int useMipmap = 1;
float renderDistance;

KeyHandler k;

void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;

int seed = 1000;

extern GLuint setShaders(const char *nVertx, const char *nFrag);
GLuint shaderProgram;

void openGlInit() {
	glClearDepth(1.0f);
	glClearColor(0.2f, 0.2f, 1.0f, 1.0f);

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_ALPHA );

	glCullFace(GL_BACK);
}

void windowResize(GLFWwindow *window, int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

int main() {

	srand(time(NULL));

	world = new World(  );
	p = new Player( world );
	ConfigLoader cf = ConfigLoader( "./bin/game.conf" );
	k = KeyHandler();

	ModelLoader *md = new ModelLoader();

	cubeModel = md->loadModel( "Cube.model" );
	grassModel = md->loadModel( "Grass.model" );
	fluidModel = md->loadModel( "Fluid.model" );
	if (cubeModel == NULL || grassModel == NULL ){
		printf("Error loading model.\n");
	}

	float fov = cf.getFloat( "render.fov" );
	renderDistance = cf.getFloat( "render.distance" );
	int worldSize = cf.getInt( "world.size" );
	useMipmap = cf.getInt( "render.mipmap" );

	world->setSize( worldSize );

	GLuint dirtTex;
	GLuint stoneTex;
	GLuint grassTex;
	GLuint skyTex;

	Sphere skyBox = Sphere( -renderDistance, 36, 18, true );

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

	glfwSetMouseButtonCallback(window, getMouseButton);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	shaderProgram = setShaders("./bin/shaders/shader.vert", "./bin/shaders/shader.frag");
	glUseProgram(shaderProgram);

	openGlInit();

	world->genChunks();
	printf("World generation completed\n");

	p->getCam()->setPos(0, 20, 0);
	p->getCam()->setRotation( glm::half_pi<float>() , glm::half_pi<float>()/3 );

	unsigned int windowSizeLoc = glGetUniformLocation(shaderProgram, "windowSize");
	glUniform2f(windowSizeLoc, SCR_WIDTH, SCR_HEIGHT);

	dirtTex = loadTexture( "textures.png" );
	skyTex = loadTexture( "sky.png" );

	printf("Textures loaded!\n");

	while (!glfwWindowShouldClose(window)) {
		double t = glfwGetTime();
		processInput(window);
		k.keyHandler();

		glm::mat4 view; // Se Calcula
		glm::mat4 projection; // Se calcula

		view = p->getCam()->getViewMatrix();
		projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, renderDistance*1.1f );

		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		unsigned int selCubeLoc = glGetUniformLocation(shaderProgram, "selectedCube");
		Cube *cs = p->getPointedCube();

		if (cs != NULL) {
			glUniform3f(selCubeLoc, cs->getX(), cs->getY(), cs->getZ());
		} else {
			glUniform3f(selCubeLoc, 0, 0, 0);
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glBindTexture( GL_TEXTURE_2D, dirtTex );

		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		world->draw();
		p->gravity(0.1);

		glBindTexture( GL_TEXTURE_2D, skyTex );
		model = glm::translate( glm::mat4(1.0f), glm::vec3( p->getCam()->getX(), p->getCam()->getY(), p->getCam()->getZ() ) );
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		skyBox.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	delete(world);
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
