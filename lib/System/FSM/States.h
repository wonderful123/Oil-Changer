#pragma once

#include <tinyfsm.hpp>

#include "Events.h"
#include "ReadyState.h"
#include "StateMachine.h"

// Event forward declarations
class Initializing;
class Extracting;
class Filling;
class InterimTask;
class OilChangeComplete;

class Initializing : public StateMachine {
public:
  void react(InitializationEvent const &event) override {}

  void react(InitializationCompleteEvent const &) {
    transit<Ready>();
  }
};

class Extracting : public StateMachine {
public:
  void entry() override{};

  void react(tinyfsm::Event const &) {}
};

class InterimTask : public StateMachine {
public:
  void entry() override {
    // Display an interim task such as filter replacement
  }

  void react(StartButtonPressedEvent const &) {
    // Handle completion of interim task such as filter replacement
    transit<Filling>();
  }
};

class Filling : public StateMachine {
public:
  void entry() override{
      // Start oil filling
      // ...
  };

  void react(OilCapacityTargetReachedEvent const &) {}

  void react(FillLowPressureSwitchTriggeredEvent const &) {
    transit<OilChangeComplete>();
  }
};

class OilChangeComplete : public StateMachine {
public:
  void entry() override {
    // Display "Oil change complete" message
  }
};

class ConfigMode : public StateMachine {
public:
  void entry() override {
    // Display "Configuration mode" message
  }

  void react(SettingUpdatedEvent const &) {
    // Update setting
  }
};
