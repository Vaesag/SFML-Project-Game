#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

enum class Symbol {
    Strawberry,
    Grapes, 
    Pineapple,
    Lemon
};

struct Reel {
    std::vector<Symbol> symbols;  // Лента символов на барабане

    float speed = 0.0f;           // Текущая скорость вращения
    bool spinning = false;        // Флаг, указывает на состояние вращения
};

class SlotMachine {
private:
	sf::RenderWindow& window;
	std::vector<Reel> reels[4];       // Барабаны

	std::vector<Symbol> winLine;   // Выигрышная линия
	sf::Texture texture;           // Текстура символов
	sf::Sprite sprite;             // Спрайт символов
	void loadTexture(const std::string& path, sf::IntRect rect = sf::IntRect(), bool smooth = true);
	void setReels();
	void setWinLine();
	void drawReels();
	void drawWinLine();
	void drawSymbols();
	void updateReels();
	void updateWinLine();
	void updateSymbols();
public:
	SlotMachine(sf::RenderWindow& window);
	void draw();
	void update();
};