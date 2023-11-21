#pragma once

#include <tinyfsm.hpp>

// Idle State
struct IdleState : tinyfsm::Fsm<IdleState> {
  void
  react(tinyfsm::Event const &) override { /* React to events if necessary */
  }
  void entry() override { /* Entry logic for Idle State */
  }
  void exit() override { /* Exit logic for Idle State */
  }
};
