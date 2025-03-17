#pragma once
#include "reel.h"

class SlotMachine;

class State {
public:
    virtual void enter(SlotMachine& machine) = 0;
    virtual void update(SlotMachine& machine) = 0;
    virtual void exit(SlotMachine& machine) = 0;
    virtual ~State() = default;
};