#ifndef GAME_HITBOX_HPP
#define GAME_HITBOX_HPP

class Player;

class HitBox {
private:
    float x, y, z;
    float height, width, depth;
    Player *player;
public:
    HitBox( Player *p, float x, float y, float z, float w, float h, float d );
    void setPos( float x, float y, float z );
    int isColliding( );
};

#endif