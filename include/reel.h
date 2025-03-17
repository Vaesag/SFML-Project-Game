#pragma once
#include <SFML/Graphics.hpp>
#include "const.h"
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
    std::vector<float> initialPositions;  // <-- Добавляем вектор исходных позиций
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

	size_t reelIndex = 0; // <-- Индекс текущего барабана

public:
    SlotMachine(sf::RenderWindow& win);
    bool areAllReelsStopped() const;

    void loadTexture(Symbol key, const std::string& path); // Загрузка текстур
    void setTextures();
    void resetState();
    void startSpin();    // Запуск вращения
    void stopSpin();     // Остановка вращения

    void update();       // Обновление логики вращения
    void draw();         // Отрисовка барабанов
   
    void printReels(); // Функция для отладки (вывод состояния барабанов)

    void checkWin(); // Функция проверки выигрыша
    void drawScore(sf::RenderWindow& window); // Отрисовка счёта
};
