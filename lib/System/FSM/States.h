#include <tinyfsm.hpp>

#include "Events.h"
#include "OilChangeTracker.h"

class StateMachine;

// Event forward declarations
class Initializing;
class Ready;
class Extracting;
class Filling;
class InterimTask;
class OilChangeComplete;

class Initializing : public StateMachine {
 public:
  void entry() override {
    // Implementation details for entering the Initializing state
    // Load configurations, set up hardware, initialize display, start web
    // server
  }

  void react(InitializationEvent const &event) override {
    // Handle initialization related events
    // Specific handling logic for each initialization event type
    // Remove dynamic_cast usage
  }
};
#include <iostream>
class Ready : public StateMachine {
 public:
  void react(ButtonPressEvent const &event) override {
    // Handle button press related events
    // Specific handling logic for each button press event type
    // Remove dynamic_cast usage
  }

  // Default reaction for unhandled events in Ready state
  void react(tinyfsm::Event const &) { /* Default handler */
  }
};

class Extracting : public StateMachine {
 public:
  void entry() override {
    // Code for initializing the oil extraction process
  }

  void react(OilExtractionEvent const &event) override {
    // Handle specific Oil Extraction events
    if (auto e = dynamic_cast<const OilExtractionStartedEvent *>(&event)) {
      // Specific handling for Oil Extraction Started
    } else if (auto e = dynamic_cast<const OilCapacityThresholdReachedEvent *>(
                   &event)) {
      // Specific handling for Oil Capacity Threshold Reached
      transit<InterimTask>();
    } else if (auto e =
                   dynamic_cast<const ExtractLowPressureSwitchTriggeredEvent *>(
                       &event)) {
      // Specific handling for Low Pressure Switch Triggered
      transit<InterimTask>();
    } else if (auto e =
                   dynamic_cast<const OilExtractionCompletedEvent *>(&event)) {
      // Specific handling for Oil Extraction Completed
      transit<InterimTask>();
    }
    // Add handling for other OilExtractionEvent types if needed
  }
};

class InterimTask : public StateMachine {
 public:
  void react(InterimEvent const &event) {
    if (auto e = dynamic_cast<const InterimPeriodStartedEvent *>(&event)) {
      // Handle Interim Period Started Event
    } else if (auto e =
                   dynamic_cast<const InterimPeriodFinishedEvent *>(&event)) {
      // Handle Interim Period Finished Event
    }
  }

  void react(StartButtonPressedEvent const &) {
    // Handle completion of interim task such as filter replacement
    transit<Filling>();
  }

  // Other react methods if needed
};

class Filling : public StateMachine {
 public:
  void entry() override {
    // Start oil filling
    // ...
  }

  void react(OilFillingEvent const &event) override {
    if (auto e = dynamic_cast<const OilFillingStartedEvent *>(&event)) {
      // Handle the start of oil filling
    } else if (auto e = dynamic_cast<const OilCapacityTargetReachedEvent *>(
                   &event)) {
      // Oil filling completed
    } else if (auto e =
                   dynamic_cast<const FillLowPressureSwitchTriggeredEvent *>(
                       &event)) {
      transit<OilChangeComplete>();
    } else if (auto e =
                   dynamic_cast<const OilFillingCompletedEvent *>(&event)) {
      transit<OilChangeComplete>();
    }
  }
};

class OilChangeComplete : public StateMachine {
 public:
  void entry() override {
    // Display "Oil change complete" message
    // ...
  }
  // No specific react methods needed unless handling specific events
};

class ConfigMode : public StateMachine {
 public:
  void entry() override {
    // Display "Configuration mode" message
    // ...
  }

  void react(ConfigurationModeEvent const &event) override {
    if (auto e = dynamic_cast<const ConfigModeEnteredEvent *>(&event)) {
      // Enter configuration mode
    } else if (auto e = dynamic_cast<const SettingUpdatedEvent *>(&event)) {
      // Update setting
    } else if (auto e = dynamic_cast<const ConfigModeExitedEvent *>(&event)) {
      // Exit configuration mode
    }
  }
};

// // Idle: Waiting for start command
// class Idle : public StateMachine {
//  public:
//   void react(ButtonPressEvent const &e) {
//     std::cout << "[Idle] ButtonPressEvent: " << e.buttonId << std::endl;
//     auto &oilChangeTracker = OilChangeTracker::getInstance();

//     if (e.buttonId == "ButtonStart") {
//       transit<Extracting>();
//     } else if (e.buttonId == "ButtonPlus") {
//       oilChangeTracker.incrementFillCapacity(0.1);
//     } else if (e.buttonId == "ButtonMinus") {
//       oilChangeTracker.decrementFillCapacity(0.1);
//     }
//   }
// };
