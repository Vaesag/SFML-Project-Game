#pragma once
#include "State.h"
#include "WaitingState.h"
#include <SFML/System/Clock.hpp>

class CheckingState : public State {
private:
    sf::Clock checkClock;
public:
    void enter(SlotMachine& machine) override {
        std::cout << "Checking win..." << std::endl;
        machine.checkWin();
        checkClock.restart();
    }

    void update(SlotMachine& machine) override {
        if (checkClock.getElapsedTime().asSeconds() > 3.0f) {
            machine.changeState(new WaitingState());
        }
    }

    void exit(SlotMachine& machine) override {}
};
