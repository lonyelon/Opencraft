#include <filesystem>
#include <memory>
#include <climits>
#include <cstring>

#include <game/Game.hpp>

std::unique_ptr<Game> game;

int main(int argc, char **argv) {

    // Random seed for rand()
    srand(time(nullptr));

    std::string worldName;
    int seed;

    game = std::make_unique<Game>();
    
    game->debug_mode = argc == 2 && strcmp(argv[1], "--quick-load") == 0;
    if (!game->debug_mode) {
        std::cout << "Enter a world name: ";
        std::cin >> worldName;

        if (!std::filesystem::is_directory("saves/" + worldName)) {
            std::cout << "Enter a world seed: ";
            std::cin >> seed;
        }
    } else {
        worldName = "debug";
        seed = rand() % INT_MAX;
    }

    game->start(worldName, seed);
    game->loop();

    return 0;
}
