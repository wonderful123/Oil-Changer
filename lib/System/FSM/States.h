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

class Ready : public StateMachine, public IObserver {
public:
  void entry() override {
    StateMachine::entry();
    _buttonController->attach(
        std::dynamic_pointer_cast<IObserver>(shared_from_this()));
  }

  void onNotify(const std::string &event, const std::string &id) override {
    if (event == "button_pressed") {
      handleButtonPress(id);
    }
  }

  void handleButtonPress(const std::string &id) {
    auto &oilChangeTracker = OilChangeTracker::getInstance();
    if (id == "ButtonPlus") {
      oilChangeTracker.incrementFillCapacity(0.1);
      Logger::info("[StateMachine: Ready] Fill capacity incremented.");
    } else if (id == "ButtonMinus") {
      oilChangeTracker.decrementFillCapacity(0.1);
      Logger::info("[StateMachine: Ready] Fill capacity decremented.");
    } else if (id == "ButtonStart") {
      // Start oil change. Transit to extracting state
      Logger::info("[StateMachine: Ready] Starting oil change.");
      transit<Extracting>();
    }
  }

  // Default reaction for unhandled events in Ready state
  void react(tinyfsm::Event const &) { /* Default handler */
  }

  void exit() override {
    _buttonController->detach(
        std::dynamic_pointer_cast<IObserver>(shared_from_this()));
    StateMachine::exit();
  }
};

class Extracting : public StateMachine {
 public:
  void entry() override {
    Logger::info("[StateMachine: Extracting] Extracting oil.");
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
