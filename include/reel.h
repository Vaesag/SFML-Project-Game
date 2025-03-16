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
