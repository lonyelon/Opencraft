#ifndef GAME_PLAYER
#define GAME_PLAYER 

#include "../engine/Engine.hpp"

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
    
    Cube* getPointer(float *x, float *y, float *z);
    Cube* getPointedCube();
    bool getPointedPosition(float *x, float *y, float *z);

    float getSpeed() { return this->movementSpeed; };
    World *getWorld() { return this->world; };

    void breakCube();
    void placeCube();
    void gravity(float ammount);
    void jump();
};

#endif