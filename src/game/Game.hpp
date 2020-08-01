//
// Created by lonyelon on 1/8/20.
//

#ifndef OPENCRAFT_GAME_HPP
#define OPENCRAFT_GAME_HPP

#include <engine/Engine.hpp>
#include <engine/keyboard/KeyboardInput.hpp>
#include <game/world/Chunk.hpp>
#include <game/world/World.hpp>
#include <engine/LoadTexture.hpp>
#include <engine/config/ConfigLoader.hpp>
#include <game/world/Sphere.h>
#include <game/Player.hpp>
#include <engine/model/ModelLoader.hpp>

#include <memory>

class Game {
public:
    World *world;
    Player *player;
    std::shared_ptr<Model> cubeModel, grassModel, fluidModel;

    int useMipmap = 1;
    float renderDistance, fov;
    unsigned int SCR_WIDTH = 1920;
    unsigned int SCR_HEIGHT = 1080;
    GLuint shaderProgram;
    KeyHandler k;
    GLFWwindow *window;

    GLuint dirtTex;
    GLuint skyTex;

    Sphere skybox;


    Game();

    void start();

    void loop();

    ~Game();
};


#endif //OPENCRAFT_GAME_HPP
