#include "Projectile.hpp"

Projectile::Projectile() {
    type = EntityType::PROJECTILE;
}

void Projectile::update(int W, int H)
{
    // Call base class update for movement and animation
    Entity::update(W, H);

    // projectiles die if off-screen
    if (x < 0 || x > W || y < 0 || y > H) {
        alive = false;
    }
}