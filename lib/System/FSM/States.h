#include <tinyfsm.hpp>
#include "Events.h"

// Idle State
struct IdleState : tinyfsm::Fsm<IdleState> {
  void react(ButtonPressEvent const &e) {
    // Handle ButtonPressEvent
  }

  // Entry logic for Idle State
  void entry() {
    // Initialization code for entering the state
  }

  // Exit logic for Idle State
  void exit() {
    // Cleanup code for exiting the state
  }
};