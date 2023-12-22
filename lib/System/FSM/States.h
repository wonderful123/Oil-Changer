#include <tinyfsm.hpp>

#include "Events.h"
#include "OilChangeTracker.h"
#include "StateMachine.h"

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
  }
};

class Ready : public StateMachine {
 public:
  void react(ButtonPressEvent const &event) override {
    // Handle button press related events
    // Specific handling logic for each button press event type
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

  void react(OilCapacityThresholdReachedEvent const &) {
    transit<InterimTask>();
  }

  void react(ExtractLowPressureSwitchTriggeredEvent const &) {
    transit<InterimTask>();
  }
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
  void entry() override {
    // Start oil filling
    // ...
  };

  void react(OilCapacityTargetReachedEvent const &) {
    transit<OilChangeComplete>();
  }

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
