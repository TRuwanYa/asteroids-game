#pragma once
#include "Entity.hpp"

struct Asteroid : public Entity {
    Asteroid();
    void update(int W, int H) override;
};