#pragma once

#include "Events.h"
#include <tinyfsm.hpp>

// Forward declarations
struct MotorRunningState;
struct MotorStoppedState;

// Base State
struct BaseState : tinyfsm::Fsm<BaseState> {
  virtual void react(ButtonPressEvent const &) {}
  virtual void entry(void) {}
  virtual void exit(void) {}
};

// Idle State
struct IdleState : BaseState {
  void react(ButtonPressEvent const &e) override {
    if (e.buttonId == "ButtonStart") {
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
