#include "UIManager.hpp"
#include <sstream>          

UIManager::UIManager(ResourceManager& res, int width, int height)          // Arguments: reference to ResourceManager, window width and height
	: resources(res), W(width), H(height) {                                // 'res' here referes to the ResourceManager instance in Game
}

// Draw the main menu
void UIManager::drawMenu(sf::RenderWindow& window) {
	const sf::Font& font = resources.getFont("mainFont");         // Get the main font from ResourceManager

	sf::Text title("ASTEROIDS", font, 64);                        // Create a text object for the title
	title.setFillColor(sf::Color::White);                         // Set the text color to white
	title.setOutlineColor(sf::Color::Blue);						  // outline color
	title.setOutlineThickness(6.f);								  // thickness in pixels
	sf::FloatRect tb = title.getLocalBounds();                    // Get the local bounds of the text for centering
	title.setOrigin(tb.width / 2.f, tb.height / 2.f);             // Set the origin to the center of the text
	title.setPosition(W / 2.f, H / 2.f - 100.f);                  // Position the title at the top center of the window
	window.draw(title);                                           // Draw the title to the window; (Draw comes as a member of Entity.hpp)

	sf::Text start("Press ENTER to Start", font, 28);             // Create a text object for the start prompt
	start.setFillColor(sf::Color::White);                         // Set the text color to white
	start.setOutlineColor(sf::Color::Black);
	start.setOutlineThickness(4.f);
	sf::FloatRect sb = start.getLocalBounds();                    // Get the local bounds of the text for centering
	start.setOrigin(sb.width / 2.f, sb.height / 2.f);             // Set the origin to the center of the text
	start.setPosition(W / 2.f, H / 2.f);                          // Position the start prompt at the center of the window
	window.draw(start);                                           // Draw the start prompt to the window
}

// Draw the in-game HUD
void UIManager::drawHUD(sf::RenderWindow& window, int score, int highScore, int lives, bool paused) {
	const sf::Font& font = resources.getFont("mainFont");                           // Get the main font from ResourceManager

	sf::Text scoreText("Score: " + std::to_string(score), font, 20);                // Create a text object for the score
	scoreText.setFillColor(sf::Color::White);                                       // Set the text color to white
	scoreText.setPosition(10, 10);                                                  // Position the score at the top-left corner of the window
	window.draw(scoreText);                                                         // Draw the score to the window

	sf::Text highText("High Score: " + std::to_string(highScore), font, 20);        // Create a text object for the high score
	highText.setFillColor(sf::Color::Yellow);                                       // Set the text color to yellow
	highText.setPosition(10, 35);                                                   // Position the high score below the score
	window.draw(highText);                                                          // Draw the high score to the window

	// Draw lives as hearts
	for (int i = 0; i < lives; ++i) {
		sf::Sprite h;             // Create a sprite for the heart icon
		h.setTexture(resources.getTexture("heart"));    // Set the texture to the heart image from ResourceManager
		h.setScale(0.8f, 0.8f);                         // Scale down the heart icon

		sf::FloatRect hb = h.getLocalBounds();          // Get the local bounds of the heart sprite
		float heartWidth = hb.width * 0.8f;             // Calculate the scaled width of the heart
		float margin = 30.f;                            // Margin from the right edge of the window
		h.setPosition(W - margin - (i + 1) * (heartWidth + 10.f), 20.f);        // Position hearts at the top-right corner with spacing
		window.draw(h);                                 // Draw the heart icon to the window
	}

	// Draw lives text
	std::stringstream ss;                               // Use stringstream for formatting
	ss << "You have " << lives << " lives";             // Format the lives text
	sf::Text livesText(ss.str(), font, 14);				// Create a text object for the lives text
	livesText.setFillColor(sf::Color::White);			// Set the text color to white

	sf::FloatRect lb = livesText.getLocalBounds();		// Get the local bounds of the lives text
	livesText.setOrigin(lb.left + lb.width, lb.top);	// Set the origin to the right edge for right alignment
	livesText.setPosition(W - 30.f, 60.f);				// Position the lives text below the hearts
	window.draw(livesText);								// Draw the lives text to the window

	if (paused) {										// If the game is paused, display the paused text
		sf::Text pauseText("PAUSED", font, 36);
		pauseText.setFillColor(sf::Color::Red);
		pauseText.setOutlineColor(sf::Color::Black);
		pauseText.setOutlineThickness(6.f);
		pauseText.setPosition(W / 2.f - pauseText.getGlobalBounds().width / 2.f, H / 2.f);
		window.draw(pauseText);
	}
}

// Draw the game over screen
void UIManager::drawGameOver(sf::RenderWindow& window, int score, int highScore) {

	sf::RectangleShape fadeRect(sf::Vector2f(static_cast<float>(W), static_cast<float>(H)));
	fadeRect.setFillColor(sf::Color(0, 0, 0, 220));
	window.draw(fadeRect);

	const sf::Font& font = resources.getFont("mainFont");

	sf::Text over("GAME OVER", font, 48);
	over.setFillColor(sf::Color::Red);
	over.setOutlineColor(sf::Color::Black);
	over.setOutlineThickness(6.f);
	over.setPosition(W / 2.f - over.getGlobalBounds().width / 2.f, H / 3.f);
	window.draw(over);

	sf::Text retry("Press ENTER to go to MENU", font, 24);
	retry.setFillColor(sf::Color::White);
	retry.setOutlineColor(sf::Color::Black);
	retry.setOutlineThickness(4.f);
	retry.setPosition(W / 2.f - retry.getGlobalBounds().width / 2.f, H / 2.f);
	window.draw(retry);

	sf::Text scoreText("Score: " + std::to_string(score), font, 24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setOutlineColor(sf::Color::Black);
	scoreText.setOutlineThickness(4.f);
	scoreText.setPosition(W / 2.f - scoreText.getGlobalBounds().width / 2.f, H / 2.f + 100);
	window.draw(scoreText);

	sf::Text highText("High Score: " + std::to_string(highScore), font, 24);
	highText.setFillColor(sf::Color::Yellow);
	highText.setOutlineColor(sf::Color::Black);
	highText.setOutlineThickness(4.f);
	highText.setPosition(W / 2.f - highText.getGlobalBounds().width / 2.f, H / 2.f + 140);
	window.draw(highText);
}

// Draw a fade effect (for transitions)
void UIManager::drawFade(sf::RenderWindow& window, float progress) {
	// Draw a black rectangle that slowly increases in opacity
	sf::RectangleShape fadeRect(sf::Vector2f(static_cast<float>(W), static_cast<float>(H)));
	sf::Uint8 alpha = static_cast<sf::Uint8>(progress * 220.f); // Max opacity 220/255
	fadeRect.setFillColor(sf::Color(0, 0, 0, alpha));
	window.draw(fadeRect);
}