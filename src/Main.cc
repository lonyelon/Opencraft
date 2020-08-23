#include <memory>
#include <filesystem>

#include <game/Game.hpp>

std::unique_ptr<Game> game;

int main(int argc, char **argv) {

    // Random seed for rand()
    srand(time(nullptr));

    std::string worldName;
    int seed;

    std::cout << "Enter a world name: ";
    std::cin >> worldName;

    if (!std::filesystem::is_directory("saves/" + worldName)) {
        std::cout << "Enter a world seed: ";
        std::cin >> seed;
    }

    game = std::make_unique<Game>();
    game->start(worldName, seed);
    game->loop();

    return 0;
}