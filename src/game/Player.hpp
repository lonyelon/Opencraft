#ifndef GAME_PLAYER
#define GAME_PLAYER 

#include "../engine/glfw.hpp"

class World;

class Player {
private:
    World *world;
    Camera *cam;
    float vspd;
public:
    Player(  );
    Player( World *world );
    void move(  );
    Camera *getCam();
};

#endif