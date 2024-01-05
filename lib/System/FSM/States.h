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

// class Ready : public StateMachine {
// public:
//   void exit() {
//     clearBuzzerRapidBeepCallback();
//   }

//   void react(ButtonPressEvent const &event) {
//     Logger::debug("[StateMachine::Ready] Button pressed: " + event.id);
//     if (event.id == "ButtonStart") {
//       notifyBuzzer("beep");
//       //transit<Extracting>();
//     } else if (event.id == "ButtonPlus") {
//       notifyBuzzer("beep");
//       notifyOilTracker("fill_capacity", 0.1);
//     } else if (event.id == "ButtonMinus") {
//       notifyBuzzer("beep");
//       notifyOilTracker("fill_capacity", -0.1);
//     } else if (event.id == "ButtonManualExtract") {
//       transit<ExtractingManual>();
//     } else if (event.id == "ButtonManualFill") {
//       transit<FillingManual>();
//     }
//   }

//   void react(ButtonHoldEvent const &event) {
//     if (event.id == "ButtonPlus") {
//       setupBuzzerRapidBeepCallback(0.1);
//     } else if (event.id == "ButtonMinus") {
//       setupBuzzerRapidBeepCallback(-0.1);
//     }
//     notifyBuzzer("rapid_beep");
//   }

//   void react(ButtonReleaseEvent const &event) {
//     clearBuzzerRapidBeepCallback();
//     notifyBuzzer("stop");
//   }

//   void react(StartOilChangeEvent const &event) {
//     Logger::info(
//         "[StateMachine] Transition: Starting oil change extraction...");
//     transit<Extracting>();
//   }

// private:
//   void setupBuzzerRapidBeepCallback(float fillCapacityAdjustmentValue) {
//     _buzzerManager->setOnRapidBeepCallback([this]() {
//       EventData data("fill_capacity", 0.1);
//       _eventManager->notify(EventType::OIL_CHANGE_TRACKER_UPDATE, data);
//     });
//   }
//   // void setupBuzzerRapidBeepCallback(float fillCapacityAdjustmentValue) {
//   //   _buzzerManager->setOnRapidBeepCallback(
//   //       [this]() {
//   //         notifyOilTracker("fill_capacity", 0.1);
//   //       });
//   // }

//   void notifyOilTracker(const std::string dataId, float value) {
//     EventData eventData(dataId, value);
//     _eventManager->notify(EventType::OIL_CHANGE_TRACKER_UPDATE, eventData);
//   }

//   // Clear buzzer rapid beep callback
//   void clearBuzzerRapidBeepCallback() {
//     _buzzerManager->setOnRapidBeepCallback(nullptr);
//   }
// };

class Ready : public StateMachine {
public:
  void exit() { clearBuzzerRapidBeepCallback(); }

  void react(ButtonPressEvent const &event) {
    Logger::debug("[StateMachine] Button pressed: " + event.id);
    if (event.id == "ButtonStartx") {
      notifyBuzzer("beep");
      transit<Extracting>();
    } else if (event.id == "ButtonStart") {
      notifyBuzzer("beep");
      notifyOilTracker("fill_capacity", 0.1);
    } else if (event.id == "ButtonMinus") {
      notifyBuzzer("beep");
      notifyOilTracker("fill_capacity", -0.1);
    } else if (event.id == "ButtonManualExtract") {
      transit<ExtractingManual>();
    } else if (event.id == "ButtonManualFill") {
      transit<FillingManual>();
    }
  }

  void react(ButtonHoldEvent const &event) {
    if (event.id == "ButtonStart") {
      setupBuzzerRapidBeepCallback(0.1);
      notifyBuzzer("rapid_beep");
    } else if (event.id == "ButtonMinus") {
      setupBuzzerRapidBeepCallback(-0.1);
      notifyBuzzer("rapid_beep");
    }
  }

  void react(ButtonReleaseEvent const &event) {
    clearBuzzerRapidBeepCallback();
    notifyBuzzer("stop");
  }

  void react(StartOilChangeEvent const &event) {
    Logger::info(
        "[StateMachine] Transition: Starting oil change extraction...");
    transit<Extracting>();
  }

  void setupBuzzerRapidBeepCallback(float fillCapacityAdjustmentValue) {
    _buzzerManager->setOnRapidBeepCallback(
        [this, fillCapacityAdjustmentValue]() {
          notifyOilTracker("fill_capacity", fillCapacityAdjustmentValue);
        });
  }

  void clearBuzzerRapidBeepCallback() {
    _buzzerManager->setOnRapidBeepCallback([this]() {});
  }

  void notifyBuzzer(const std::string &dataId) {
    EventData eventData(dataId);
    _eventManager->notify(EventType::BUZZER_BEEP, eventData);
  }

  void notifyOilTracker(const std::string dataId, float value) {
    EventData eventData(dataId, value);
    _eventManager->notify(EventType::OIL_CHANGE_TRACKER_UPDATE, eventData);
  }
};

class ExtractingManual : public StateMachine {
public:
  void entry() override {
    Logger::debug("[StateMachine] Manual extraction started");
    _eventManager->notify(EventType::MANUAL_EXTRACT_START);
  }

  void react(ButtonReleaseEvent const &) {
    Logger::debug("[StateMachine] Manual extraction stopped");
    _eventManager->notify(EventType::MANUAL_EXTRACT_STOP);
    transit<Ready>();
  }
};

class FillingManual : public StateMachine {
public:
  void entry() override {
    Logger::debug("[StateMachine] Manual fill started");
    _eventManager->notify(EventType::MANUAL_FILL_START);
  }

  void react(ButtonReleaseEvent const &) {
    Logger::debug("[StateMachine] Manual fill stopped");
    _eventManager->notify(EventType::MANUAL_FILL_STOP);
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

  void react(StartOilChangeEvent const &) {
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
