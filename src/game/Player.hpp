#ifndef GAME_PLAYER
#define GAME_PLAYER 

#include "../engine/glfw.hpp"

class World;
class Cube;

class Player {
private:
    World *world;
    Camera *cam;
    float vspd;
    float movementSpeed;
public:
    Player( World *world );

    void move( float mx, float my, float mz );
    void setSprint(bool sprint);

    Camera *getCam();
    Cube* getPointedCube();
    float getSpeed() { return this->movementSpeed; };
    World *getWorld() { return this->world; };

    void breakCube();
    void gravity(float ammount);
    void jump();
};

#endif