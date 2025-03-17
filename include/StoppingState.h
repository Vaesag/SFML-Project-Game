#pragma once
#include "State.h"
#include "CheckingState.h"

class StoppingState : public State {
private:
  
public:
    void enter(SlotMachine& machine) override {
        std::cout << "Stopping reels..." << std::endl;
    }

    void update(SlotMachine& machine) override {
       
        machine.stopSpin();

        if (machine.areAllReelsStopped()) {
            machine.changeState(new CheckingState());
        }
    }

    void exit(SlotMachine& machine) override {}
};