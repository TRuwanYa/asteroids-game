#include "Animation.hpp"

Animation::Animation() : frame(0), speed(0.0f) {}

Animation::Animation(const sf::Texture& t, int x, int y, int w, int h, int count, float speed)
    : frame(0), speed(speed)
{
    //Cuts out count frames starting at (x,y), each frame of size (w,h)
    //Example: explosion sprite sheet with 8 frames in one row -> creates 8 rectangles.
    for (int i = 0; i < count; ++i)
        frames.emplace_back(x + i * w, y, w, h);

	// setup sprite -> So the sprite is centered and starts at frame 0
    spr.setTexture(t);
    if (!frames.empty()) {
        spr.setTextureRect(frames[0]);
        spr.setOrigin(frames[0].width / 2.f, frames[0].height / 2.f);
    }
}

void Animation::update()
{
    if (frames.empty()) return;
    frame += speed;                                 // Increases the frame by speed 
	int n = static_cast<int>(frames.size());        // total number of frames
	if (frame >= n) frame -= n; // loop             // loops back to start if over the last frame
	spr.setTextureRect(frames[static_cast<int>(frame)]);        // Updates the displayed rectangle (so the sprite shows the correct frame)
}

// Restart at frame 0 (useful when reusing the animation)
void Animation::reset()
{
    frame = 0;
    if (!frames.empty()) spr.setTextureRect(frames[0]);
}

// Returns true if the animation has reached its last frame
// (important for effects like explosions that should disappear after finishing)
bool Animation::isEnd() const
{
    if (frames.empty()) return true;
    return (frame + speed >= static_cast<float>(frames.size()));
}

sf::Sprite& Animation::sprite()         // get the sprite to draw
{
    return spr;
}
