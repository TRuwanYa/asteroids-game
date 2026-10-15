#include "Effect.hpp"

Effect::Effect() {
    type = EntityType::EFFECT;
}

void Effect::update(int W, int H)
{
    // Call base class update for movement and animation
    Entity::update(W, H);

    // EFFECTs die after animation ends
    if (anim.isEnd()) {
        alive = false;
    }
}