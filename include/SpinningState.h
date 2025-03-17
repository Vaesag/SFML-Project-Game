#pragma once
#include "State.h"
#include "StoppingState.h"

class SpinningState : public State {
private:
    sf::Clock spinClock;

public:
    void enter(SlotMachine& machine) override {
        std::cout << "Spinning reels!" << std::endl;
        machine.startSpin();
        spinClock.restart();
    }
    void update(SlotMachine& machine) override {
        if (spinClock.getElapsedTime().asSeconds() >= 10.0f) {
            std::cout << "Auto-stop triggered!" << std::endl;
            machine.changeState(new StoppingState());
        }
    }
    void exit(SlotMachine& machine) override {}
};