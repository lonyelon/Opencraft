#ifndef GAME_HITBOX_HPP
#define GAME_HITBOX_HPP

class HitBox {
private:
    float x, y, z;
    float height, width;
public:
    HitBox( float x, float y, float z, float w, float h );
    void setPos( float x, float y, float z );
    bool isColliding(  );
};

#endif