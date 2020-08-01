#ifndef GAME_PLAYER
#define GAME_PLAYER

#include "../engine/Engine.hpp"

#include <memory>

class World;

class Cube;

class Player {
private:
    std::shared_ptr<World> world;
    std::shared_ptr<Camera> cam;
    float vspd;
    float movementSpeed;
public:
    Player(std::shared_ptr<World> world);

    void move(float mx, float my, float mz);

    void setSprint(bool sprint);

    std::shared_ptr<Camera> getCam();

    std::shared_ptr<Cube> getPointer(float *x, float *y, float *z);

    std::shared_ptr<Cube> getPointedCube();

    bool getPointedPosition(float *x, float *y, float *z);

    float getSpeed() { return this->movementSpeed; };

    std::shared_ptr<World> getWorld() { return this->world; };

    void breakCube();

    void placeCube();

    void gravity(float ammount);

    void jump();
};

#endif