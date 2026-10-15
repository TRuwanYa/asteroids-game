#pragma once
#include "Entity.hpp"

struct Player : public Entity {
    Player();
    void update(int W, int H) override;
};