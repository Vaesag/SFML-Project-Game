#pragma once
#include "State.h"

class WaitingState : public State {
public:
    void enter(SlotMachine& machine) override { machine.resetState(); }
    void update(SlotMachine& machine) override {}
    void exit(SlotMachine& machine) override {}
};