#pragma once
#include "Entity.hpp"

struct Projectile : public Entity {
    Projectile();
    void update(int W, int H) override;
};