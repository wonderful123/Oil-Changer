// States.h
#pragma once

#include "BuzzerManager.h"
#include "EventManager/EventManager.h"
#include "Events.h"
#include "StateMachine.h"
#include "SystemController.h"
#include "SystemFactory.h"
#include <memory>
#include <tinyfsm.hpp>

// Event forward declarations
class Initializing;
class Ready;
class ExtractingManual;
class FillingManual;
class Extracting;
class Filling;
class InterimTask;
class OilChangeComplete;

class Initializing : public StateMachine {
public:
  void react(InitializationEvent const &event) override {}

  void react(InitializationCompleteEvent const &) {
    Logger::info("===============================================");
    Logger::info(" SYSTEM INITIALIZATION: SUCCESSFULLY COMPLETED");
    Logger::info("===============================================");

    transit<Ready>();
  }
};

class Ready : public StateMachine {
public:
  void entry() { Logger::info("[State] Ready"); }

  void exit() { clearBuzzerRapidBeepCallback(); }

  void react(ButtonPressEvent const &event) {
    if (event.id == "ButtonStart") {
      notifyBuzzer(Parameter::SingleBeep);
    } else if (event.id == "ButtonPlus") {
      notifyBuzzer(Parameter::SingleBeep);
      notifyOilTracker(Parameter::FillCapacity, 0.1);
    } else if (event.id == "ButtonMinus") {
      notifyBuzzer(Parameter::SingleBeep);
      notifyOilTracker(Parameter::FillCapacity, -0.1);
    } else if (event.id == "ButtonManualExtract") {
      transit<ExtractingManual>();
    } else if (event.id == "ButtonManualFill") {
      transit<FillingManual>();
    }
  }

  void react(ButtonHoldEvent const &event) {
    if (event.id == "ButtonStart") {
      notifyBuzzer(Parameter::DoubleBeep);
      transit<Extracting>();
    } else if (event.id == "ButtonPlus") {
      setupBuzzerRapidBeepCallback(0.1);
      notifyBuzzer(Parameter::RapidBeep);
    } else if (event.id == "ButtonMinus") {
      setupBuzzerRapidBeepCallback(-0.1);
      notifyBuzzer(Parameter::RapidBeep);
    }
  }

  void react(ButtonReleaseEvent const &event) {
    clearBuzzerRapidBeepCallback();
    notifyBuzzer(Parameter::StopBeep);
  }

  void react(StartOilChangeEvent const &event) {
    Logger::info(
        "[StateMachine] Transition: Starting oil change extraction...");
    transit<Extracting>();
  }

  void setupBuzzerRapidBeepCallback(float fillCapacityAdjustmentValue) {
    _buzzerManager->setOnRapidBeepCallback([this,
                                            fillCapacityAdjustmentValue]() {
      notifyOilTracker(Parameter::FillCapacity, fillCapacityAdjustmentValue);
    });
  }

  void clearBuzzerRapidBeepCallback() {
    _buzzerManager->setOnRapidBeepCallback([this]() {});
  }

  void notifyBuzzer(Parameter beepType) {
    _eventManager->notify(Event::Buzzer, beepType);
  }

  void notifyOilTracker(Parameter parameter, float value) {
    _eventManager->notify(Event::OilChangeTracker, parameter, value);
  }
};

/*********************************************************************/

class ExtractingManual : public StateMachine {
public:
  void entry() override {
    Logger::debug("[StateMachine] Manual extraction started");
    _eventManager->notify(Event::Motor, Parameter::MotorExtract);
  }

  void react(ButtonReleaseEvent const &) {
    Logger::debug("[StateMachine] Manual extraction stopped");
    _eventManager->notify(Event::Motor, Parameter::MotorExtract);
    transit<Ready>();
  }
};

/*********************************************************************/

class FillingManual : public StateMachine {
public:
  void entry() override {
    Logger::debug("[StateMachine] Manual fill started");
    _eventManager->notify(Event::Motor, Parameter::MotorFill);
  }

  void react(ButtonReleaseEvent const &) {
    Logger::debug("[StateMachine] Manual fill stopped");
    _eventManager->notify(Event::Motor, Parameter::MotorStop);
    transit<Ready>();
  }
};

/*********************************************************************/

class Extracting : public StateMachine {
public:
  void entry() override {
    Logger::info("[State] Extracting...");
    _eventManager->notify(Event::Motor, Parameter::MotorExtract);
  };

  void react(ButtonPressEvent const &event) {
    if (event.id == "ButtonStop") {
      Logger::info("[State Extracting] Motor halt");
      _eventManager->notify(Event::Buzzer, Parameter::DoubleBeep);
      _eventManager->notify(Event::Motor, Parameter::MotorHalt);
    } else if (event.id == "ButtonStart") {
      _eventManager->notify(Event::Buzzer, Parameter::SingleBeep);
    }
  }

  void react(ButtonHoldEvent const &event) {
    if (event.id == "ButtonStart") {
      // ***** temporary command to move to next state.
      _eventManager->notify(Event::Buzzer, Parameter::DoubleBeep);
      transit<InterimTask>();
    }
  }

  void react(ExtractLowPressureSwitchTriggeredEvent const &) {
    _eventManager->notify(Event::Buzzer, Parameter::DoubleBeep);
    _eventManager->notify(Event::Motor, Parameter::MotorStop);
    transit<InterimTask>();
  }
};

/*********************************************************************/

class InterimTask : public StateMachine {
public:
  void entry() override {
    // Display an interim task such as filter replacement
    Logger::info("[State] Interim Event");
  }

  void react(ButtonPressEvent const &event) {
    // Handle completion of interim task such as filter replacement
    if (event.id == "ButtonManualExtract") {
      _eventManager->notify(Event::Motor, Parameter::MotorExtract);
    } else if (event.id == "ButtonManualFill") {
      _eventManager->notify(Event::Motor, Parameter::MotorFill);
    } else if (event.id == "ButtonStart") {
      _eventManager->notify(Event::Buzzer, Parameter::SingleBeep);
    }
  }

  void react(ButtonHoldEvent const &event) {
    if (event.id == "ButtonStart") {
      _eventManager->notify(Event::Buzzer, Parameter::DoubleBeep);
      transit<Filling>();
    }
  }
};

/*********************************************************************/

class Filling : public StateMachine {
public:
  void entry() override {
    Logger::info("[State] Filling...");
    _eventManager->notify(Event::Motor, Parameter::MotorFill);
  };

  void react(ButtonPressEvent const &event) {
    if (event.id == "ButtonStop") {
      Logger::info("[State Extracting] Motor halt");
      _eventManager->notify(Event::Buzzer, Parameter::DoubleBeep);
      _eventManager->notify(Event::Motor, Parameter::MotorHalt);
    } else if (event.id == "ButtonStart") {
      _eventManager->notify(Event::Buzzer, Parameter::SingleBeep);
    }
  }

  void react(ButtonHoldEvent const &event) {
    if (event.id == "ButtonStart") {
      // ***** temporary command to move to next state.
      _eventManager->notify(Event::Buzzer, Parameter::DoubleBeep);
      transit<OilChangeComplete>();
    }
  }

  void react(OilCapacityTargetReachedEvent const &) {
    _eventManager->notify(Event::Motor, Parameter::MotorStop);
    transit<OilChangeComplete>();
  }

  void react(FillLowPressureSwitchTriggeredEvent const &) {
    _eventManager->notify(Event::Motor, Parameter::MotorStop);
    transit<OilChangeComplete>();
  }
};

/*********************************************************************/

class OilChangeComplete : public StateMachine {
public:
  void entry() override { Logger::info("[State] Oil change complete"); }

  void react(ButtonPressEvent const &event) {
    if (event.id == "ButtonStart") {
      _eventManager->notify(Event::Buzzer, Parameter::SingleBeep);
    } else if (event.id == "ButtonManualExtract") {
      _eventManager->notify(Event::Motor, Parameter::MotorExtract);
    } else if (event.id == "ButtonManualFill") {
      _eventManager->notify(Event::Motor, Parameter::MotorFill);
    }
  };

  void react(ButtonHoldEvent const &event) {
    if (event.id == "ButtonStart") {
      _eventManager->notify(Event::Buzzer, Parameter::DoubleBeep);
      transit<Ready>();
    }
  }
};

/*********************************************************************/

class ConfigMode : public StateMachine {
public:
  void entry() override {
    // Display "Configuration mode" message
  }

  void react(SettingUpdatedEvent const &) {
    // Update setting
  }
};
