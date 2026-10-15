#include "Game.hpp"
#include "Player.hpp"
#include "Asteroid.hpp"
#include "Projectile.hpp"
#include "Effect.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>

// Constructor to initialize the game with specified window dimensions
Game::Game(int width, int height)
    : W(width), H(height),
    window(sf::VideoMode(width, height), "Asteroids (OOP Clean)"),      // video mode means the width, height and depth of the rendering area of the window
    ui(resources, width, height)                                        // Initialize UIManager with ResourceManager and window dimensions
{
    window.setFramerateLimit(60);                     // Limit the frame rate to 60 FPS(Frames per Seconds)
    srand((unsigned)std::time(nullptr));              // Seed the random number generator with the current time
    loadAssets();                                     // Load all game assets (textures, sounds, fonts, etc.)
    resetGame();                                      // Reset the game state to start a new game
}

// Destructor to clean up remaining entities when the game closes
Game::~Game() {
    for (auto e : entities) {
        delete e;
    }
    entities.clear();
}

void Game::loadAssets() {
    // Textures
    resources.loadTexture("ship", "assets/images/spaceship.png");
    resources.loadTexture("bg", "assets/images/background3.jpg");
    resources.loadTexture("rock", "assets/images/rock.png");
    resources.loadTexture("rockSmall", "assets/images/rock_small.png");
    resources.loadTexture("fire", "assets/images/fire_blue.png");
    resources.loadTexture("explC", "assets/images/explosions/type_C.png");
    resources.loadTexture("explB", "assets/images/explosions/type_B.png");
    resources.loadTexture("heart", "assets/images/heart.png");

    // Font
    resources.loadFont("mainFont", "assets/fonts/PressStart2P-Regular.ttf");

    // Sounds
    resources.loadSound("title", "assets/sounds/title.wav");
    resources.loadSound("shoot", "assets/sounds/shoot.wav");
    resources.loadSound("destroy", "assets/sounds/destroy.wav");
    resources.loadSound("thrust", "assets/sounds/thrust.wav");
    resources.loadSound("start", "assets/sounds/start.wav");
    resources.loadSound("gameOver", "assets/sounds/gameover.wav");
    resources.loadSound("destroyShip", "assets/sounds/destroyShip.wav");

    sndTitle.setBuffer(resources.getSoundBuffer("title"));
    sndShoot.setBuffer(resources.getSoundBuffer("shoot"));
    sndDestroy.setBuffer(resources.getSoundBuffer("destroy"));
    sndThrust.setBuffer(resources.getSoundBuffer("thrust"));
    sndStart.setBuffer(resources.getSoundBuffer("start"));
    sndGameOver.setBuffer(resources.getSoundBuffer("gameOver"));
    sndDestroyShip.setBuffer(resources.getSoundBuffer("destroyShip"));

    // Background
    background.setTexture(resources.getTexture("bg"));                      // Set the background texture
    sf::Vector2u texSize = resources.getTexture("bg").getSize();            // Get the size of the background texture
    background.setScale((float)W / texSize.x, (float)H / texSize.y);        // Scale the background to fit the window

    // Animations ( texture, x, y, width, height, frameCount, frameTime )
    aExpl = Animation(resources.getTexture("explC"), 0, 0, 256, 256, 48, 0.5f);
    aExplShip = Animation(resources.getTexture("explB"), 0, 0, 192, 192, 64, 0.5f);
    aRock = Animation(resources.getTexture("rock"), 0, 0, 64, 64, 16, 0.2f);
    aRockSmall = Animation(resources.getTexture("rockSmall"), 0, 0, 64, 64, 16, 0.2f);
    aBullet = Animation(resources.getTexture("fire"), 0, 0, 32, 64, 16, 0.8f);
    aPlayer = Animation(resources.getTexture("ship"), 40, 0, 40, 40, 1, 0);
    aPlayerGo = Animation(resources.getTexture("ship"), 40, 40, 40, 40, 1, 0);
}

// Reset the game state to start a new game
void Game::resetGame() {
    // Properly clean up memory before clearing the list
    for (auto e : entities) {
        delete e;
    }
    entities.clear();           // Clear all existing entities

    lives = 3;                  // Reset player lives
    score = 0;                  // Reset score
	gameOverTimer = 0;          // Reset game over timer

    for (int i = 0; i < 8; ++i) {                                       // Spawn 8 asteroids at random positions
        spawnAsteroid(std::rand() % W, std::rand() % H, 25.f);          // Default radius 25.f
    }

    // Spawn the player using the new Player object
    Entity* player = new Player();
    player->init(EntityType::PLAYER, aPlayer, W / 2.f, H / 2.f, 0.f, 20.f);
    player->dx = 0; player->dy = 0;
    entities.push_back(player);
}

// Spawn a new asteroid at the given position with specified radius
void Game::spawnAsteroid(float x, float y, float radius) {
    Entity* a = new Asteroid();
    if (radius <= 15.f) {
        a->init(EntityType::ASTEROID, aRockSmall, x, y, (float)(std::rand() % 360), radius);

        a->dx = (std::rand() % 8 - 4) * 1.5f; // Small ones move a bit faster
        a->dy = (std::rand() % 8 - 4) * 1.5f;
    }
    else {
        a->init(EntityType::ASTEROID, aRock, x, y, (float)(std::rand() % 360), radius);
        a->dx = (std::rand() % 8 - 4);
        a->dy = (std::rand() % 8 - 4);
    }
    entities.push_back(a);
}

// Spawn a new projectile at the given position and angle
void Game::spawnProjectile(float x, float y, float angle) {
    Entity* b = new Projectile();
    b->init(EntityType::PROJECTILE, aBullet, x, y, angle, 10.f);

    float baseSpeed = 15.f; // INCREASED: Make the bullet naturally faster than the ship

    // Find the player to inherit their current velocity
    float playerDx = 0.f;
    float playerDy = 0.f;
    for (auto e : entities) {
        if (e->type == EntityType::PLAYER) {
            playerDx = e->dx;
            playerDy = e->dy;
            break;
        }
    }

    // Bullet speed = Player's current speed + Bullet's base speed
    b->dx = playerDx + std::cos(angle * 0.01745329252f) * baseSpeed;
    b->dy = playerDy + std::sin(angle * 0.01745329252f) * baseSpeed;

    b->lifeTimer = 120;
    entities.push_back(b);
    sndShoot.play();
}

// Spawn a visual effect at the given position
void Game::spawnEffect(const Animation& a, float x, float y) {
    Entity* e = new Effect();                                                   // Create a new effect object
    e->init(EntityType::EFFECT, a, x, y, 0.f, 1.f);                             // Radius 1.f 
    entities.push_back(e);                                                      // Add effect to the entities list
}

// Main game loop function
void Game::run() {
    while (window.isOpen()) {
        processEvents();        // handle input / events
        update();               // update game logic (movement, collisions)
        render();               // draw the current frame
    }
}

// Handles input and events
void Game::processEvents() {
    sf::Event event;                                            // Event object to capture window events
    while (window.pollEvent(event)) {                           // Poll events from the window
        if (event.type == sf::Event::Closed)                    // If the window is requested to be closed
            window.close();                                     // Close the window

        if (state == GameState::PLAYING && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {      // Toggle pause when 'P' is pressed
            paused = !paused;                                                                                               // Toggle pause state
        }

        // Main Menu and Game Over screen input handling
        if (state != GameState::PLAYING) {                                                                                  // If in MENU or GAME_OVER state
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {                             // Start or restart the game when Enter is pressed
                if (state == GameState::MENU) {                                                                             // From MENU to PLAYING
                    sndStart.play();                                                                                        // Play start sound effect
                    sndTitle.stop();                                                                                        // Stop title music
                    state = GameState::PLAYING;                                                                             // Change state to PLAYING
                    resetGame();                                                                                            // Reset the game state
                }
                else if (state == GameState::GAME_OVER) {                                                                   // From GAME_OVER to MENU
                    state = GameState::MENU;                                                                                // Change state to MENU
                }
            }
        }

        // Shooting projectiles when spacebar is pressed
        if (state == GameState::PLAYING && !paused) {                                                                       // Only allow shooting when the game is active and not paused
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {                             // Shoot projectile when Space is pressed
                for (auto e : entities) {                                                                                   // Find the player entity
                    if (e->type == EntityType::PLAYER) {                                                                    // If this entity is the player
                        spawnProjectile(e->x, e->y, e->angle);                                                              // Spawn a projectile at the player's position and angle
                        break;                                                                                              // Exit the loop after spawning the projectile
                    }
                }
            }
        }
    }

    // Continuous input handling for player movement
    if (state == GameState::PLAYING && !paused) {   // Only process movement when the game is active and not paused
        for (auto e : entities) {                   // Iterate through all entities
            if (e->type == EntityType::PLAYER) {    // Find the player entity
                // Rotation                         
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                    e->angle -= 3.f;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    e->angle += 3.f;

                // Thrust
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    e->thrust = true;
                    e->anim = aPlayerGo;

                    float acc = 0.2f;                                                      // Acceleration factor
                    e->dx += std::cos(e->angle * 0.01745329252f) * acc;                    // Convert angle to radians and calculate velocity components
                    e->dy += std::sin(e->angle * 0.01745329252f) * acc;                    // Convert angle to radians and calculate velocity components

                    if (sndThrust.getStatus() != sf::Sound::Playing)                       // Play thrust sound if not already playing
                        sndThrust.play();
                }
                else {
                    if (e->thrust && sndThrust.getStatus() == sf::Sound::Playing)          // Stop thrust sound if thrust just ended
                        sndThrust.stop();
                    e->thrust = false;                                                     // Reset thrust flag
                    e->anim = aPlayer;                                                     // Switch back to normal player animation
                }
            }
        }
    }
}

// Update game logic, entities, and state
void Game::update() {
    if (paused || state != GameState::PLAYING)
        return;

    for (auto e : entities) {
        e->update(W, H);
    }

    handleCollisions();
    removeDead();

	// wave logic: check if there are any asteroids left
    int asteroidCount = 0;
    for (auto e : entities) {
        if (e->type == EntityType::ASTEROID) {
            asteroidCount++;
        }
    }

    // If no asteroids are left, spawn a new wave of 8
    if (asteroidCount == 0) {
        for (int i = 0; i < 8; ++i) {
            // Spawn far away from the center to avoid hitting the player instantly
            float spawnX = (std::rand() % 2 == 0) ? 0.f : static_cast<float>(W);
            float spawnY = (std::rand() % 2 == 0) ? 0.f : static_cast<float>(H);
            spawnAsteroid(spawnX, spawnY, 25.f);
        }
    }
    

    if (score > highestScore)
        highestScore = score;

	// Handle game over timer countdown
    if (lives <= 0 && state == GameState::PLAYING) {
        if (gameOverTimer > 0) {
            gameOverTimer--;
            if (gameOverTimer == 0) {
                state = GameState::GAME_OVER;
                sndGameOver.play();
            }
        }
    }
}

// Check and handle collisions between entities
void Game::handleCollisions() {
    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i + 1; j < entities.size(); ++j) {
            Entity* a = entities[i];
            Entity* b = entities[j];

            if (!a->alive || !b->alive) continue;

            // Projectile vs Asteroid
            if (a->type == EntityType::PROJECTILE && b->type == EntityType::ASTEROID) {
                if (a->collidesWith(*b)) {
                    a->alive = false; b->alive = false;
                    score += 100;
                    spawnEffect(aExpl, b->x, b->y);
                    sndDestroy.play();
                    if (b->R > 15.f)
                        for (int k = 0; k < 2; ++k) spawnAsteroid(b->x, b->y, 15.f);
                }
            }
            else if (a->type == EntityType::ASTEROID && b->type == EntityType::PROJECTILE) {
                if (a->collidesWith(*b)) {
                    a->alive = false; b->alive = false;
                    score += 100;
                    spawnEffect(aExpl, a->x, a->y);
                    sndDestroy.play();
                    if (a->R > 15.f)
                        for (int k = 0; k < 2; ++k) spawnAsteroid(a->x, a->y, 15.f);
                }
            }

            // Player (a) vs Asteroid (b)
            if (a->type == EntityType::PLAYER && b->type == EntityType::ASTEROID) {
                if (a->collidesWith(*b)) {
                    b->alive = false; // Destroy the asteroid
                    spawnEffect(aExplShip, a->x, a->y);
                    sndDestroyShip.play();
                    lives--;

                    if (lives <= 0) {
                        a->alive = false; // DESTROY THE PLAYER (a)
                        gameOverTimer = GAME_OVER_DELAY;
                    }
                    else {
                        a->x = W / 2.f; a->y = H / 2.f;
                        a->dx = a->dy = 0.f;

                        // Clear spawn area
                        for (auto e : entities) {
                            if (e->type == EntityType::ASTEROID && e->alive) {
                                float dx = e->x - (W / 2.f);
                                float dy = e->y - (H / 2.f);
                                if (std::sqrt(dx * dx + dy * dy) < 150.f) {
                                    e->alive = false;
                                }
                            }
                        }
                    }
                }
            }
            // Asteroid (a) vs Player (b)
            else if (a->type == EntityType::ASTEROID && b->type == EntityType::PLAYER) {
                if (a->collidesWith(*b)) {
                    a->alive = false; // Destroy the asteroid
                    spawnEffect(aExplShip, b->x, b->y);
                    sndDestroyShip.play();
                    lives--;

                    if (lives <= 0) {
                        b->alive = false; // DESTROY THE PLAYER (b)
                        gameOverTimer = GAME_OVER_DELAY;
                    }
                    else {
                        b->x = W / 2.f; b->y = H / 2.f;
                        b->dx = b->dy = 0.f;

                        // Clear spawn area
                        for (auto e : entities) {
                            if (e->type == EntityType::ASTEROID && e->alive) {
                                float dx = e->x - (W / 2.f);
                                float dy = e->y - (H / 2.f);
                                if (std::sqrt(dx * dx + dy * dy) < 150.f) {
                                    e->alive = false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Remove entities that are no longer alive 
void Game::removeDead() {
    std::vector<Entity*> tmp;                 // Temporary vector to hold alive entities
    tmp.reserve(entities.size());             // Reserve space to avoid multiple allocations
    for (auto e : entities) {                 // Iterate through all entities
        if (e->alive) {                       // If the entity is alive
            tmp.push_back(e);                 // Add it to the temporary vector
        }
        else {
            delete e;                         // CRITICAL: Clean up memory for dead objects
        }
    }
    entities.swap(tmp);                       // Swap the temporary vector with the original
}

// Render the current game state to the window
void Game::render() {
    window.clear();                       // Clear the window with the default color (black)
    window.draw(background);              // Draw the background

    // Draw based on the current game state
    if (state == GameState::MENU) {
        if (sndTitle.getStatus() != sf::Sound::Playing)
            sndTitle.play();
        ui.drawMenu(window);
    }
    // Playing state: draw entities and HUD
    else if (state == GameState::PLAYING) {
        for (auto e : entities)
            e->draw(window);

        // If the player is dying, render the cinematic fade!
        if (lives <= 0) {
            float fadeProgress = 1.0f - (static_cast<float>(gameOverTimer) / GAME_OVER_DELAY);
            ui.drawFade(window, fadeProgress);
        }

        ui.drawHUD(window, score, highestScore, lives, paused);
    }
    
    // Game Over state: draw game over screen
    else if (state == GameState::GAME_OVER) {
        if (sndGameOver.getStatus() != sf::Sound::Playing)
            sndGameOver.play();
        ui.drawGameOver(window, score, highestScore);
    }

    window.display();       // Display the rendered frame on the window
}