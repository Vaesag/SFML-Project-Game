#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct Reel {
   // std::vector<Symbol> symbols;  // ����� �������� �� ��������

    float rotation = 0.0f;        // ������� �������� ��������
    float speed = 0.0f;           // ������� �������� ��������
    bool spinning = false;        // ����, ��������� �� ��������� ��������
    float elapsedTime = 0.0f;     // ��������� ����� � ������� ������
    bool accelerating = true;     // ���� ���������
    bool decelerating = false;    // ���� ����������
    float startDelay = 0.0f;      // �������� ������ ��������
    bool started = false;         // ����, ���������, ��� �������� ��������

    float targetRotation = 0.0f;  // ������� ���� ���������
    float maxSpeed = 800.0f;      // ������������ �������� ��������
};

//class Reel {
//private:
//    sf::RenderWindow& window;
//    sf::Sprite background;
//    sf::Sprite startBtn;
//    sf::Sprite stopBtn;
//    std::map<std::string, sf::Texture> textures;
//
//public:
//    UI(sf::RenderWindow& window);
//
//    sf::Sprite& getSettingsButton() { return startBtn; }
//
//    void setTextures();
//    void loadTexture(const std::string& key, const std::string& path, sf::IntRect rect = sf::IntRect(), bool smooth = true);
//    void drawUI();
//};
