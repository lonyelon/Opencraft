#include <memory>

#include <game/Game.hpp>

std::unique_ptr<Game> game;

int main(int argc, char **argv) {

    game = std::make_unique<Game>();
    game->start();
    game->loop();

    return 0;
}