#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.hpp"

// An entity is a generic game object
enum class EntityType { PLAYER, ASTEROID, PROJECTILE, EFFECT };

struct Entity {
    EntityType type = EntityType::ASTEROID;         // default type
    Animation anim;                                 // animation (sprite + frames)
    float x = 0, y = 0;                             // position
    float dx = 0, dy = 0;                           // velocity
    float angle = 0;                                // orientation angle
    float R = 8;                                    // collision radius
    bool alive = true;                              // is alive
    bool thrust = false;                            // used only for player
    int lifeTimer = 0;                              // optional lifetime (ticks)

    // constructors
    Entity() = default;
    virtual ~Entity() = default; // Added virtual destructor for safe polymorphic deletion

    // Sets up the entity
    virtual void init(EntityType t, const Animation& a, float X, float Y, float Angle = 0.f, float radius = 8.f);

    // Virtual update to be overridden by derived classes
    virtual void update(int W, int H);
    void draw(sf::RenderWindow& app);
    bool collidesWith(const Entity& o) const;
};