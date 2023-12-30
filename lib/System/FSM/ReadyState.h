#include <tinyfsm.hpp>

#include "Events.h"
#include "IBuzzer.h"
#include "Observer/IObserver.h"
#include "Observer/ISubject.h"
#include "OilChangeTracker.h"
#include "StateMachine.h"
#include "SystemFactory.h"
#include <memory>

class StateMachine;
class Extracting;

class Ready : public StateMachine,
              public IObserver,
              public std::enable_shared_from_this<Ready> {
public:
  Ready(std::shared_ptr<IBuzzer> buzzer,
        std::shared_ptr<OilChangeTracker> tracker)
      : _buzzer(buzzer), _tracker(tracker) {
        _buttonController = SystemFactory::getInstance().getButtonController();
      }

  void entry() override {
    StateMachine::entry();
    _buttonController->attach(
        std::dynamic_pointer_cast<IObserver>(shared_from_this()));
  }

  void exit() override {
    _buttonController->detach(
        std::dynamic_pointer_cast<IObserver>(shared_from_this()));
    StateMachine::exit();
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

private:
  std::shared_ptr<IBuzzer> _buzzer;
  std::shared_ptr<OilChangeTracker> _tracker;
  std::shared_ptr<ButtonController> _buttonController;

  void setupBuzzer() {
    _buzzer->setOnBeepCallback([this]() { notify("buzzer_beep", ""); });
  }
};