#pragma once
#include "HardwareManager.h"
#include "Events.h"
#include "States.h"

class StateMachine {
public:
  StateMachine() {
    // Initialize states and transitions here
  }

  void handleEvent(const Event &event) {
    // Logic to handle different events
    if (auto *e = dynamic_cast<ButtonPressEvent const *>(&event)) {
      // Example: Transition to a different state based on button press
      // IdleState::dispatch(*e);
    }
  }

  // Other methods as needed
};