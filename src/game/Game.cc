//
// Created by lonyelon on 1/8/20.
//

#include "Game.hpp"

#include <filesystem>

extern std::unique_ptr<Game> game;

extern GLuint setShaders(const char *nVertx, const char *nFrag);

void openGlInit() {
    glClearDepth(1.0f);
    glClearColor(0.2f, 0.2f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA);

    glCullFace(GL_BACK);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        exit(0);
    }
}

void windowResize(GLFWwindow *window, int width, int height) {
    game->SCR_WIDTH = width;
    game->SCR_HEIGHT = height;
}

Game::Game() {

}

void Game::start() {
    // Random seed for rand()
    srand(time(NULL));

    // TODO move this to main
    std::string worldName;
    int seed;

    std::cout << "Enter a world name: ";
    std::cin >> worldName;

    if (!std::filesystem::is_directory("saves/" + worldName)) {
        std::cout << "Enter a world seed: ";
        std::cin >> seed;
    }

    this->world = new World(worldName, seed);
    this->player = new Player(this->world);

    ConfigLoader cf = ConfigLoader("./bin/game.conf");
    this->k = KeyHandler();

    // Load models
    ModelLoader *md = new ModelLoader();
    this->cubeModel = md->loadModel("Cube.model");
    this->grassModel = md->loadModel("Grass.model");
    this->fluidModel = md->loadModel("Fluid.model");
    if (this->cubeModel == NULL || this->grassModel == NULL || this->fluidModel == NULL) {
        printf("Error loading models.\n");
    }

    this->fov = cf.getFloat("render.fov");
    this->renderDistance = cf.getFloat("render.distance");
    int worldSize = cf.getInt("world.size");
    this->useMipmap = cf.getInt("render.mipmap");

    this->world->setSize(worldSize);

    this->skybox = Sphere(-renderDistance, 36, 18, true);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(this->SCR_WIDTH, this->SCR_HEIGHT, "Clases", NULL, NULL);
    if (this->window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(this->window);
    glfwSetKeyCallback(this->window, getKeyboardInput);
    glfwSetInputMode(this->window, GLFW_STICKY_KEYS, 1);

    glfwSetCursorPosCallback(this->window, getMouseInput);
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowSizeCallback(this->window, windowResize);
    glfwSetFramebufferSizeCallback(this->window, windowResize);

    glfwSetMouseButtonCallback(this->window, getMouseButton);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    shaderProgram = setShaders("./bin/shaders/shader.vert", "./bin/shaders/shader.frag");
    glUseProgram(shaderProgram);

    openGlInit();

    world->genChunks();
    printf("World generation completed\n");

    this->player->getCam()->setPos(0, 20, 0);
    this->player->getCam()->setRotation(glm::half_pi<float>(), glm::half_pi<float>() / 3);

    unsigned int windowSizeLoc = glGetUniformLocation(shaderProgram, "windowSize");
    glUniform2f(windowSizeLoc, SCR_WIDTH, SCR_HEIGHT);

    this->dirtTex = loadTexture("textures.png");
    this->skyTex = loadTexture("sky.png");

    printf("Textures loaded!\n");
}

void Game::loop() {
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        k.keyHandler();

        glm::mat4 view; // Se Calcula
        glm::mat4 projection; // Se calcula

        view = this->player->getCam()->getViewMatrix();
        projection = glm::perspective(glm::radians(this->fov), (float) this->SCR_WIDTH / (float) this->SCR_HEIGHT, 0.1f,
                                      renderDistance * 1.1f);

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int selCubeLoc = glGetUniformLocation(shaderProgram, "selectedCube");
        Cube *cs = game->player->getPointedCube();

        if (cs != NULL) {
            glUniform3f(selCubeLoc, cs->getX(), cs->getY(), cs->getZ());
        } else {
            glUniform3f(selCubeLoc, 0, 0, 0);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, this->dirtTex);

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        world->draw();
        this->player->gravity(0.1);

        glBindTexture(GL_TEXTURE_2D, skyTex);
        model = glm::translate(glm::mat4(1.0f),
                               glm::vec3(this->player->getCam()->getX(), this->player->getCam()->getY(),
                                         this->player->getCam()->getZ()));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        this->skybox.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Game::~Game() {
    glfwTerminate();
    glfwSetWindowShouldClose(window, true);
    delete (this->world);
}