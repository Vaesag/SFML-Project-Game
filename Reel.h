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
    std::vector<Symbol> symbols;  // ����� �������� �� ��������

    float speed = 0.0f;           // ������� �������� ��������
    bool spinning = false;        // ����, ��������� �� ��������� ��������
};

class SlotMachine {
private:
	sf::RenderWindow& window;
	std::vector<Reel> reels[4];       // ��������

	std::vector<Symbol> winLine;   // ���������� �����
	sf::Texture texture;           // �������� ��������
	sf::Sprite sprite;             // ������ ��������
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