#include "Asteroid.hpp"

Asteroid::Asteroid() {
    type = EntityType::ASTEROID;
}

void Asteroid::update(int W, int H)
{
    // Call base class update for movement and animation
    Entity::update(W, H);

    // wrap around
    if (x > W) x = 0;
    if (x < 0) x = static_cast<float>(W);
    if (y > H) y = 0;
    if (y < 0) y = static_cast<float>(H);
}