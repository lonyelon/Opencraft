#include "Game.hpp"

#include <filesystem>

extern std::unique_ptr<Game> game;

extern GLuint setShaders(const char *nVertx, const char *nFrag);

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        exit(0);
    }
}

void windowResize(GLFWwindow *window, int width, int height) {
    game->SCR_WIDTH = width;
    game->SCR_HEIGHT = height;
}

void _initOpenGL(Game *game) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    game->window = glfwCreateWindow(game->SCR_WIDTH, game->SCR_HEIGHT, "Opencraft", glfwGetPrimaryMonitor(), nullptr);
    if (game->window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(game->window);
    glfwSetKeyCallback(game->window, getKeyboardInput);
    glfwSetInputMode(game->window, GLFW_STICKY_KEYS, 1);

    glfwSetCursorPosCallback(game->window, getMouseInput);
    glfwSetInputMode(game->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowSizeCallback(game->window, windowResize);
    glfwSetFramebufferSizeCallback(game->window, windowResize);

    glfwSetMouseButtonCallback(game->window, getMouseButton);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glClearDepth(1.0f);
    glClearColor(0.2f, 0.2f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA);

    glCullFace(GL_BACK);
}

void Game::start(std::string worldName, int seed) {
    this->keyHandler = std::make_shared<KeyHandler>();

    ModelLoader *md = new ModelLoader();
    this->cubeModel = md->loadModel("Cube.model");
    this->grassModel = md->loadModel("Grass.model");
    this->fluidModel = md->loadModel("Fluid.model");
    if (this->cubeModel == nullptr || this->grassModel == nullptr || this->fluidModel == nullptr)
        printf("Error loading models.\n");
    else
        printf("Models loaded!.\n");

    // Read configuration.
    ConfigLoader cf = ConfigLoader("./bin/game.conf");
    this->fov = cf.getFloat("render.fov");
    this->renderDistance = cf.getFloat("render.distance");
    this->useMipmap = cf.getInt("render.mipmap");
    printf("Configuration loaded!");

    // Skybox.
    this->skybox = Sphere(-renderDistance, 36, 18, true);

    // OpenGL.
    _initOpenGL(this);

    // Shaders.
    shaderProgram = setShaders("./bin/shaders/shader.vert", "./bin/shaders/shader.frag");
    glUseProgram(shaderProgram);

    // Textures
    this->dirtTex = loadTexture("textures.png");
    this->skyTex = loadTexture("sky.png");
    printf("Textures loaded!.\n");

    // Create player and world
    this->world = std::make_shared<World>(worldName, seed);
    this->player = std::make_shared<Player>(this->world);

    // Create world.
    int worldSize = cf.getInt("world.size");
    this->world->setSize(worldSize);

    // Create player.
    std::stringstream playerDataFilePath;
    playerDataFilePath << "saves/" << this->world->getName() << "/playerData.txt";
    std::ifstream playerDataFile(playerDataFilePath.str());
    if (playerDataFile.is_open()) {
        float x, y, z;
        playerDataFile >> x >> y >> z;
        this->player->getCam()->setPos(x, y, z);
    } else {
        this->player->getCam()->setPos(0, 25, 0);
    };
    this->player->getCam()->setRotation(glm::half_pi<float>(), glm::half_pi<float>() / 3);

    world->genChunks();
    printf("World generation completed\n");

    // Window.
    unsigned int windowSizeLoc = glGetUniformLocation(shaderProgram, "windowSize");
    glUniform2f(windowSizeLoc, SCR_WIDTH, SCR_HEIGHT);
}

void Game::loop() {
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        keyHandler->keyHandler();

        glm::mat4 view = this->player->getCam()->getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(this->fov),
                                                (float) this->SCR_WIDTH / (float) this->SCR_HEIGHT,
                                                0.1f,
                                                renderDistance * 1.1f);

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int selCubeLoc = glGetUniformLocation(shaderProgram, "selectedCube");
        std::shared_ptr<Cube> selectedCube = game->getPlayer()->getPointedCube();

        if (selectedCube != nullptr)
            glUniform3f(selCubeLoc, selectedCube->getX(), selectedCube->getY(), selectedCube->getZ());
        else
            glUniform3f(selCubeLoc, 0, 0, 0);

        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, this->dirtTex);

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        world->draw();
        this->player->gravity(0.1);

        glBindTexture(GL_TEXTURE_2D, skyTex);
        model = glm::translate(glm::mat4(1.0f),
                               glm::vec3(this->player->getCam()->getX(),
                                         this->player->getCam()->getY(),
                                         this->player->getCam()->getZ()));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        this->skybox.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Game::~Game() {
    printf("Closing game\n");

    this->world->saveWorld();

    glfwTerminate();
    glfwSetWindowShouldClose(window, true);
}
