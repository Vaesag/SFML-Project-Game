#pragma once
#include "State.h"

class SpinningState : public State {
public:
    void enter(SlotMachine& machine) override {
        std::cout << "Spinning reels!" << std::endl;
        machine.startSpin();
    }
    void update(SlotMachine& machine) override {}
    void exit(SlotMachine& machine) override {}
};