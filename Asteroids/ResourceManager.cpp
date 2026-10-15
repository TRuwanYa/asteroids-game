#include "ResourceManager.hpp"

bool ResourceManager::loadTexture(const std::string& id, const std::string& filename) {
	sf::Texture tex;                        // Create a texture object; ex: like loading 'ship' from assets
	if (!tex.loadFromFile(filename)) {                                      // Try to load the texture from file
		std::cerr << "Failed to load texture: " << filename << "\n";        // Error handling
		return false;                                                       // Return false on failure
    }   
	textures[id] = std::move(tex);          // Store the texture in the map with the given ID
	return true;                            // Return true on success
}

// Same as above but for fonts
bool ResourceManager::loadFont(const std::string& id, const std::string& filename) {
    sf::Font font;                                                              
    if (!font.loadFromFile(filename)) {
        std::cerr << "Failed to load font: " << filename << "\n";
        return false;
    }
    fonts[id] = std::move(font);
    return true;
}

// Retrieval functions

const sf::Texture& ResourceManager::getTexture(const std::string& id) const {
    return textures.at(id);
}

const sf::Font& ResourceManager::getFont(const std::string& id) const {
    return fonts.at(id);
}

// Sound buffer loading and retrieval

bool ResourceManager::loadSound(const std::string& id, const std::string& filename) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "Failed to load sound: " << filename << "\n";
        return false;
    }
    sounds[id] = std::move(buffer);
    return true;
}

const sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& id) const {
    return sounds.at(id);
}
