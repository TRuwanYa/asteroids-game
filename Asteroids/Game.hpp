#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <vector>
#include "Entity.hpp"
#include "Animation.hpp"
#include "ResourceManager.hpp"
#include "UIManager.hpp"
#include "Player.hpp"
#include "Asteroid.hpp"
#include "Projectile.hpp"
#include "Effect.hpp"

// Game is the central controller for your program
// It manages the game loop, state transitions, entity management, rendering, and user input handling.

enum class GameState { MENU, PLAYING, GAME_OVER };  // Different states of the game

class Game {
public:
	Game(int width = 800, int height = 600);    // Constructor to initialize the game with specified window dimensions
	~Game();									// Destructor to clean up resources
	void run();                                 // Main game loop

private:
	void loadAssets();                          // Load all game assets (textures, sounds, fonts, etc.)
	void resetGame();                           // Reset the game state to start a new game
	void spawnAsteroid(float x, float y, float radius = 25.f);      // Spawn a new asteroid at the given position with specified radius
	void spawnProjectile(float x, float y, float angle);            // Spawn a new projectile at the given position and angle
	void spawnEffect(const Animation& a, float x, float y);         // Spawn a visual effect at the given position
	void processEvents();                       // Handle user input and window events
	void update();                              // Update game logic, entities, and state
	void handleCollisions();                    // Check and handle collisions between entities
	void removeDead();                          // Remove entities that are no longer alive
	void render();                              // Render the current game state to the window

	// Sound effects
	sf::Sound sndTitle;
	sf::Sound sndShoot;
	sf::Sound sndDestroy;
	sf::Sound sndThrust;
	sf::Sound sndStart;
	sf::Sound sndGameOver;
	sf::Sound sndDestroyShip;

	int W, H;                                   // Window dimensions
	sf::RenderWindow window;                    // SFML window for rendering
	GameState state = GameState::MENU;          // Current game state

	ResourceManager resources;                  // Manages loading and accessing game resources (textures, fonts, sounds)
	UIManager ui;								// Manages the user interface (menus, HUD, game over screen)

	// Animations
	Animation aPlayer, aPlayerGo, aRock, aRockSmall, aBullet, aExpl, aExplShip;

	std::vector<Entity*> entities;				// All active game entities (player, asteroids, projectiles, effects)

	int lives = 3;
	int score = 0;
	int highestScore = 0;
	int gameOverTimer = 0;
	const int GAME_OVER_DELAY = 120; // 2 seconds (at 60 frames per second)

	bool paused = false;						// Is the game currently paused?
	sf::Sprite background;
};