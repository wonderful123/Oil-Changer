// DiagnosticsState.h
#pragma once

#include "Events.h"
#include "Logger.h"
#include "StateMachine.h"
#include <tinyfsm.hpp>

class Ready;

class Diagnostics : public StateMachine {
public:
  void entry() override {
    Logger::info("[State] Entering Diagnostics Mode");
    Logger::info("[State] Hold the STOP button to exit diagnostics mode");
    _eventManager->notify(Event::State, Parameter::Diagnostics);
  }

  void exit() override {
    Logger::info("[State] Exiting Diagnostics Mode");
    // Cleanup or finalize diagnostics routines
  }

  void react(ButtonPressEvent const &event) {
    notifyBuzzer(Parameter::SingleBeep);
    Logger::info("[Diagnostics] Button press event - id: " + event.id);
  }

  void react(ButtonHoldEvent const &event) {
    notifyBuzzer(Parameter::DoubleBeep);
    Logger::info("[Diagnostics] Button hold event - id: " + event.id);
  }

  void react(ButtonLongHoldEvent const &event) {
    notifyBuzzer(Parameter::RapidBeep);
    Logger::info("[Diagnostics] Button long hold event - id: " + event.id);
    if (event.id == "ButtonStop") {
      transit<Ready>();
    }
  }

  void react(PressureSwitchEvent const &event) {
    notifyBuzzer(Parameter::SingleBeep);
    if (event.pressureSwitch == PressureSwitch::Extract) {
      if (event.switchState == 0) {
        Logger::info("[Diagnostics] Extract Low Pressure Switch Closed");
      } else {
        Logger::info("[Diagnostics] Extract Low Pressure Switch Open");
      }
    }

    if (event.pressureSwitch == PressureSwitch::Fill) {
      if (event.switchState == 0) {
        Logger::info("[Diagnostics] Fill Low Pressure Switch Closed");
      } else {
        Logger::info("[Diagnostics] Fill Low Pressure Switch Open");
      }
    }
  }

  void react(ButtonReleaseEvent const &event) {
    clearBuzzerRapidBeepCallback();
    notifyBuzzer(Parameter::StopBeep);
    Logger::info("[Diagnostics] Button release event - id: " + event.id);
  }

  void clearBuzzerRapidBeepCallback() {
    _buzzerManager->setOnRapidBeepCallback([this]() {});
  }

  void notifyBuzzer(Parameter beepType) {
    _eventManager->notify(Event::Buzzer, beepType);
  }
};
