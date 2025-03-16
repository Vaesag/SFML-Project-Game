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
