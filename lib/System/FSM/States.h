#pragma once
#include "Events.h"
#include "HardwareManager.h"
#include <tinyfsm.hpp>

// Forward declaration of all states
struct IdleState;
struct MotorRunningState;
struct MotorStoppedState;

// Base State
struct BaseState : tinyfsm::Fsm<BaseState> {
  virtual void
  react(ButtonPressEvent const &) { /* default reaction for unhandled events */
  }
  virtual void entry(void) { /* default action on state entry */
  }
  virtual void exit(void) { /* default action on state exit */
  }
};

// Idle State
struct IdleState : BaseState {
  void react(ButtonPressEvent const &e) override {
    if (e.buttonId == "ButtonStart") {
      // Logic for ButtonStart event
      transit<MotorRunningState>();
    }
  }
};

// Motor Running State
struct MotorRunningState : BaseState {
  void entry() override {
    // Code to start the motor
  }

  void react(ButtonPressEvent const &e) override {
    if (e.buttonId == "ButtonStop") {
      // Logic for ButtonStop event
      transit<MotorStoppedState>();
    }
  }
};

// Motor Stopped State
struct MotorStoppedState : BaseState {
  void entry() override {
    // Code to stop the motor
  }
};