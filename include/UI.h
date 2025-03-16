#pragma once
#include "const.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

enum class GameState { Waiting, Spinning, Stopping, Checking };

class UI {
private:
    sf::RenderWindow& window;
    sf::Sprite background;
    sf::Sprite startBtn;
    sf::Sprite stopBtn;
    std::map<std::string, sf::Texture> textures;

    GameState gameState = GameState::Waiting;

public:
    UI(sf::RenderWindow& window);

    sf::Sprite& getSettingsButton() { return startBtn; }
    
	void setTextures();
    void loadTexture(const std::string& key, const std::string& path, sf::IntRect rect = sf::IntRect(), bool smooth = true);
    void drawUI();
    void update();
};