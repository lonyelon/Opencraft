#ifndef GAME_PLAYER
#define GAME_PLAYER

#include "../engine/Engine.hpp"
#include <game/world/cube/CubeTypes.hpp>

#include <memory>

class World;

class Cube;

class Player {
private:
    std::weak_ptr<World> world;
    std::shared_ptr<Camera> cam;
    CubeType selectedCube;
    float vspd;
    float movementSpeed;
public:
    Player(std::weak_ptr<World> world);

    void move(float mx, float my, float mz);

    void setSprint(bool sprint);

    [[deprecated]]
    std::shared_ptr<Camera> getCam() { return this->getCamera(); };
    std::shared_ptr<Camera> getCamera();

    std::shared_ptr<Cube> getPointer(float *x, float *y, float *z) const;

    std::shared_ptr<Cube> getPointedCube() const;

    bool getPointedPosition(float *x, float *y, float *z) const;

    float getSpeed() { return this->movementSpeed; };

    std::weak_ptr<World> getWorld() { return this->world; };

    void selectCube();

    void breakCube() const;

    void placeCube() const;

    void gravity(float ammount);

    void jump();
};

#endif
