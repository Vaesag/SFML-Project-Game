#pragma once
#include <SFML/Graphics.hpp>
#include "const.h"
#include "state.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

enum class Symbol {
    Strawberry,
    Grapes,
    Pineapple,
    Lemon
};

struct Reel {
    std::vector<Symbol> symbols = { Symbol::Strawberry, Symbol::Grapes, Symbol::Pineapple, Symbol::Lemon };
    std::vector<float> initialPositions;
    std::vector<sf::Sprite> spriteSymbols;
    float speed = 0.0f;
    bool spinning = false;

    Reel() : speed(0.0f), spinning(false) {}
};

class SlotMachine {
private:
    sf::RenderWindow& window;
    std::vector<Reel> reels;
    std::map<Symbol, sf::Texture> symbolTextures; 

    int score = 0;
    sf::Font font;
    sf::Text scoreText;

    sf::Clock spinClock; 
    sf::Clock stopClock;
	size_t reelIndex = 0;

    State* currentState = nullptr;

public:
    SlotMachine(sf::RenderWindow& win);

    void changeState(State* newState);
    void resetState();
    State* getCurrentState() const { return currentState; }

    void loadTexture(Symbol key, const std::string& path); 
    void setTextures();

    void startSpin();    
    void stopSpin();    
    bool areAllReelsStopped() const;

    void update();       
    void draw();         
    void checkWin();
};
