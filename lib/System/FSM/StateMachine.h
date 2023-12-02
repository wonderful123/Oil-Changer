#pragma once

#include "FSM/Events.h"
#include "FSM/States.h"

class StateMachine {
public:
  void handleEvent(const ButtonPressEvent &event) {
    // Dispatch the event to the current state
    BaseState::dispatch(event);
  }
  // ... other methods
};