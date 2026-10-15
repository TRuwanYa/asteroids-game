#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ResourceManager.hpp"

// This class is dedicated to drawing the user interface (UI) of your Asteroids game; MENU, Heads Up Display(HUD) and GAME OVER

class UIManager {
public:
	UIManager(ResourceManager& resources, int width, int height);                   // Constructor

	void drawMenu(sf::RenderWindow& window);                                        // Draw the main menu
	void drawHUD(sf::RenderWindow& window, int score, int highScore, int lives, bool paused);   // Draw the in-game HUD
	void drawGameOver(sf::RenderWindow& window, int score, int highScore);			// Draw the game over screen
	void drawFade(sf::RenderWindow& window, float progress);						// Draw a fade effect (for transitions)

private:
	ResourceManager& resources;						// Reference to the resource manager for accessing fonts and textures
	int W, H;										// Window dimensions
};

