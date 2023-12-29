#include <tinyfsm.hpp>

#include "Events.h"
#include "IBuzzer.h"
#include "Observer/IObserver.h"
#include "OilChangeTracker.h"
#include "StateMachine.h"
#include <memory>

class StateMachine;

class Ready : public StateMachine, public IObserver {
public:
  Ready(std::shared_ptr<Buzzer> buzzer,
        std::shared_ptr<OilChangeTracker> tracker)
      : _buzzer(buzzer), _tracker(tracker) {}

  void entry() override {
    StateMachine::entry();
    setupBuzzer();
  }

  void exit() override {
    _buzzer->stop(); // Stop the buzzer when exiting the Ready state
    StateMachine::exit();
  }

  void onNotify(const std::string &event, const std::string &id) override {
    if (event == "buzzer_beep") {
      _tracker->incrementFillCapacity(0.1);
    }
  }

private:
  std::shared_ptr<IBuzzer> _buzzer;
  std::shared_ptr<OilChangeTracker> _tracker;

  void setupBuzzer() {
    _buzzer->setOnBeepCallback([this]() { notify("buzzer_beep", ""); });
    _buzzer->startAutoRepeat(); // Start the auto-repeat beeping
  }
};