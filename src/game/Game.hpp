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

class Game {
private:
    std::shared_ptr<World> world;
    std::shared_ptr<Player> player;
    std::shared_ptr<KeyHandler> keyHandler;

    Sphere skybox;
    std::unique_ptr<Model> sky;
public:
    std::shared_ptr<Model> cubeModel, grassModel, fluidModel;

    int useMipmap = 1;
    float renderDistance, fov;
    unsigned int SCR_WIDTH = 2560;
    unsigned int SCR_HEIGHT = 1440;
    GLuint shaderProgram;

    GLFWwindow *window;
    GLuint dirtTex;
    GLuint skyTex;

    bool debug_mode = false;

    Game() = default;

    void start(std::string worldName, int seed);

    void loop();

    std::shared_ptr<World> getWorld() { return this->world; }

    std::shared_ptr<Player> getPlayer() { return this->player; }

    std::shared_ptr<KeyHandler> getKeyHandler() { return this->keyHandler; }

    ~Game();
};


#endif //OPENCRAFT_GAME_HPP
