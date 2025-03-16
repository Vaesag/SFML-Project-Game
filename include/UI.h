#pragma once
#include "const.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

class UI {
private:
    sf::Sprite background;
    sf::Sprite startBtn;
    sf::Sprite stopBtn;
    std::map<std::string, sf::Texture> textures;

public:
    UI(sf::RenderWindow& window);

    sf::Sprite& getSettingsButton() { return startBtn; }
    
	void setTextures();
    void loadTexture(const std::string& key, const std::string& path, sf::IntRect rect = sf::IntRect(), bool smooth = true);
    void drawUI(sf::RenderWindow& window);
};