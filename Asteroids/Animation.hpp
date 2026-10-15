#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Sprite sheet animation handling happens here

class Animation {
public:
	Animation();                                                                            // default constructor
	Animation(const sf::Texture& t, int x, int y, int w, int h, int count, float speed);    // parameterized constructor

	void update();                  // advance the animation frame
	void reset();                   // reset to first frame
	bool isEnd() const;             // check if animation has reached the end (for non-looping animations)
	sf::Sprite& sprite();			// get the sprite to draw

private:
	float frame;					// current frame index (can be fractional for speed control)
	float speed;					// speed of animation (frames per update)
	std::vector<sf::IntRect> frames;// rectangles defining each frame in the sprite sheet
	sf::Sprite spr;					// sprite to display the current frame
};

