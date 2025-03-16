#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct Reel {
   // std::vector<Symbol> symbols;  // Лента символов на барабане

    float rotation = 0.0f;        // Текущее смещение барабана
    float speed = 0.0f;           // Текущая скорость вращения
    bool spinning = false;        // Флаг, указывает на состояние вращения
    float elapsedTime = 0.0f;     // Прошедшее время с момента старта
    bool accelerating = true;     // Флаг ускорения
    bool decelerating = false;    // Флаг замедления
    float startDelay = 0.0f;      // Задержка старта вращения
    bool started = false;         // Флаг, указывает, что вращение началось

    float targetRotation = 0.0f;  // Целевой угол остановки
    float maxSpeed = 800.0f;      // Максимальная скорость вращения
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
