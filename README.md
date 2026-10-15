# Asteroids: Object-Oriented C++ Implementation

**Academic Context:** Faculty of Engineering, University of Sri Jayewardenepura (USJ)  
**Course Level:** 2nd Year, 3rd Semester  
**Module:** Object-Oriented Programming (OOP)   

A modernized, C++ implementation of the classic arcade game *Asteroids*. This project was developed from the ground up to demonstrate core Object-Oriented Programming principles, robust memory management, and real-time rendering using the SFML multimedia library.

## Core Game Features
* **Wave-Based Progression:** The game continuously monitors the entity list; clearing the board dynamically triggers the next wave of incoming asteroids.
* **Momentum Physics:** The player's ship features vector-based thrust, friction, and rotational mechanics with screen-wrapping.
* **Dynamic Projectiles:** Lasers inherit the player's current velocity matrix to maintain realistic forward momentum regardless of flight speed.
* **Entity Fragmentation:** Large asteroids fracture into multiple smaller, faster-moving rocks upon taking damage.
* **Cinematic States:** A custom UI manager handles a main menu, real-time HUD (score, high score, lives tracking), and a timed fade-to-black sequence upon the final player death.

## OOP Architecture & Design Patterns
* **Polymorphism & Inheritance:** A base virtual `Entity` class establishes the core contract for game objects. `Player`, `Asteroid`, `Projectile`, and `Effect` structs inherit from this base, overriding the `update()` function to run their unique behaviors within a single generic loop.
* **Encapsulation:** Distinct manager classes separate the game logic cleanly:
  * `Game`: The central engine handling the main loop, state machines, and collision logic.
  * `UIManager`: Dedicated exclusively to frontend rendering, decoupling UI code from backend mechanics.
  * `ResourceManager`: A centralized loader handling textures, fonts, and audio buffers to optimize memory footprint.
* **Dynamic Memory Management:** Game entities are instantiated dynamically via pointers (`std::vector<Entity*>`). Custom destructors and cleanup functions (`removeDead()`) actively prevent memory leaks during wave resets and application exits.

## Tech Stack
* **Language:** C++17
* **Multimedia Library:** SFML 2.6.1 (Graphics, Audio, Window, System)
* **Build System:** CMake

## Build Instructions
1. Install SFML 2.6.1 and ensure it is accessible via your system's PATH.
2. Clone this repository and create a `build` directory.
3. Run `cmake ..` followed by your compiler's specific make command (e.g., `make` or `msbuild`).
4. Ensure the `assets` folder (containing `images/`, `sounds/`, and `fonts/`) is located in the same root directory as the compiled executable before running.

## Acknowledgements & Asset Credits
While the core Object-Oriented architecture, physics calculations, and memory management systems were heavily refactored and written for this project, the visual and audio assets (sprites, backgrounds, sounds) and the original procedural baseline were sourced externally.

* **Original Base Concept & Assets:** [YouTube Tutorial Series](https://www.youtube.com/watch?v=rWaSo2usU4A&list=PLB_ibvUSN7mzUffhiay5g5GUHyJRO4DYr&index=16)
* **Unmodified Source Code:** The `Animation.cpp` and `Animation.hpp` files were kept verbatim from the original source to handle raw sprite rendering, while the rest of the game engine and entity logic were completely refactored.
* **Fonts:** "Press Start 2P" (Licensed under the SIL Open Font License).
* **Additional Audio/Visuals:** Sourced from various open-source and freeware asset repositories.

> *Note: I do not claim ownership over the image, font, and sound files located in the `assets/` directory. They belong to their respective original creators and are used here strictly for educational, non-commercial purposes.*

## License
The C++ source code modifications, OOP structural design, and CMake configurations are provided under the **MIT License**. You are free to use, modify, and distribute the code for educational purposes. The media assets remain subject to the licenses of their original copyright holders.

## Authour
**Ruwanya Herath**