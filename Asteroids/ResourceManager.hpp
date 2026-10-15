#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <map>					
#include <string>
#include <iostream>

// We use <map> contains 'key-value' partners (Keys are unique and automatically kept in sorted order)
// We can; insert a pair (map["id"] = value), retrieve a value (map.at("id")) and check if a key exists (map.find("id"))

// This class is a central manager for game resources: textures, fonts, and sound buffers.
// 1. Loading: It provides methods to load textures, fonts, and sound buffers from files and associate them with string IDs.
// 2. Retrieval: It allows retrieval of these resources by their IDs, ensuring that resources are loaded only once and reused efficiently.
// 3. Storage: Internally, it uses maps to store the resources, allowing for quick access and management.

class ResourceManager {
public:
	bool loadTexture(const std::string& id, const std::string& filename);               // Load a texture from file and associate it with an ID
	bool loadFont(const std::string& id, const std::string& filename);                  // Load a font from file and associate it with an ID

	const sf::Texture& getTexture(const std::string& id) const;                         // Retrieve a texture by its ID
	const sf::Font& getFont(const std::string& id) const;                               // Retrieve a font by its ID

	bool loadSound(const std::string& id, const std::string& filename);                 // Load a sound buffer from file and associate it with an ID
	const sf::SoundBuffer& getSoundBuffer(const std::string& id) const;					// Retrieve a sound buffer by its ID



private:
	std::map<std::string, sf::Texture> textures;										// Map to store textures with string IDs
	std::map<std::string, sf::Font> fonts;												// Map to store fonts with string IDs
	std::map<std::string, sf::SoundBuffer> sounds;										// Map to store sound buffers with string IDs
};

// So instead of passing around raw file paths everywhere, you just use IDs.
