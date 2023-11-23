#pragma once

#include "Events.h"
#include "HardwareManager.h"
#include "States.h"

class StateMachine {
public:
  void handleEvent(const ButtonPressEvent &event) {
    if (event.buttonId == "ButtonStart") {
      // Transition to a specific state or handle event
      IdleState::dispatch(event);
    } else if (event.buttonId == "ButtonStop") {
      // Transition to a specific state or handle event
      IdleState::dispatch(event);
    }
  }
  // ... other methods
};