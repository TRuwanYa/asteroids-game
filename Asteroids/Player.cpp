#include "Player.hpp"
#include <cmath>

Player::Player() {
    type = EntityType::PLAYER;
}

void Player::update(int W, int H)
{
    // acceleration if thrust
    if (thrust) {
        dx += std::cos(angle * 0.01745329252f) * 0.2f;
        dy += std::sin(angle * 0.01745329252f) * 0.2f;
    }
    else {
        dx *= 0.99f;    // friction
        dy *= 0.99f;    // friction
    }

    // clamp speed
    float maxSpeed = 15.f;
    float speed = std::sqrt(dx * dx + dy * dy);
    if (speed > maxSpeed) {
        dx *= maxSpeed / speed;
        dy *= maxSpeed / speed;
    }

    // Call base class update for movement and animation
    Entity::update(W, H);

    // wrap around
    if (x > W) x = 0;
    if (x < 0) x = static_cast<float>(W);
    if (y > H) y = 0;
    if (y < 0) y = static_cast<float>(H);
}