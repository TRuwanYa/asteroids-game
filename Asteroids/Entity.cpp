#include "Entity.hpp"

void Entity::init(EntityType t, const Animation& a, float X, float Y, float Angle, float radius)
{
    type = t;
    anim = a;
    x = X;
    y = Y;
    angle = Angle;
    R = radius;
    alive = true;
    thrust = false;
    lifeTimer = 0;
    dx = dy = 0.f;
}

void Entity::update(int W, int H)
{
    // Shared generic movement
    x += dx;
    y += dy;

    // update animation
    anim.update();

    // lifetime countdown
    if (lifeTimer > 0) {
        if (--lifeTimer <= 0)
            alive = false;
    }
}

void Entity::draw(sf::RenderWindow& app)
{
    anim.sprite().setPosition(x, y);
    anim.sprite().setRotation(angle + 90.f);
    app.draw(anim.sprite());
}

bool Entity::collidesWith(const Entity& o) const
{
    float dx_ = o.x - x;
    float dy_ = o.y - y;
    float r = o.R + R;
    return (dx_ * dx_ + dy_ * dy_) < (r * r);
}