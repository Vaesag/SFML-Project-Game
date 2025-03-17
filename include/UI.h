#pragma once
#include "reel.h"

enum class GameState { Waiting, Spinning, Stopping, Checking };

class UI {
private:
    sf::RenderWindow& window;
    sf::Sprite background;
    sf::Sprite startBtn;
    sf::Sprite stopBtn;
    std::map<std::string, sf::Texture> textures;

    GameState gameState = GameState::Waiting;
    sf::Clock stopClock;
    sf::Clock checkClock;


public:
    UI(sf::RenderWindow& window);

	void setTextures();
    void loadTexture(const std::string& key, const std::string& path, sf::IntRect rect = sf::IntRect(), bool smooth = true);
    void drawUI();
    void update(SlotMachine& slotMachine);
};