#pragma once
#include "Entity.hpp"

struct Effect : public Entity {
    Effect();
    void update(int W, int H) override;
};