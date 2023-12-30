#include "ReadyState.h"

#include "ConcreteMediator.h"
#include "Logger.h"
#include "States.h"

Ready::Ready() {
  auto &systemFactory = SystemFactory::getInstance();
  _buzzerManager = systemFactory.getSystemController()->getBuzzerManager();
  _buttonController = systemFactory.getButtonController();
  _mediator = systemFactory.getMediator();
  setMediator(systemFactory.getMediator());
}

void Ready::entry() {
  StateMachine::entry();
  // Attach to the button controller to listen for button presses
  _buttonController->attach(
      std::dynamic_pointer_cast<IObserver>(shared_from_this()));

  setupBuzzerRapidBeepCallback();
}

void Ready::exit() {
  _buttonController->detach(
      std::dynamic_pointer_cast<IObserver>(shared_from_this()));

  // Clear the callback for rapid beep
  _buzzerManager->setOnRapidBeepCallback(nullptr);

  StateMachine::exit();
}

void Ready::handleButtonPress(const std::string &id) {
  EventData data;
  data.id = "fill_capacity";

  if (id == "ButtonPlus") {
    data.value = 0.1f; // Increment
    _mediator->notify(this, OIL_CHANGE_TRACKER_UPDATE, &data);
    Logger::info("[StateMachine: Ready] Notified mediator to increment "
                 "OilChangeTracker");
  } else if (id == "ButtonMinus") {
    data.value = -0.1f; // Decrement
    _mediator->notify(this, OIL_CHANGE_TRACKER_UPDATE, &data);
    Logger::info("[StateMachine: Ready] Notified mediator to decrement "
                 "OilChangeTracker");
  } else if (id == "ButtonStart") {
    Logger::info("[StateMachine: Ready] Starting oil change.");
    transit<Extracting>();
  }
}

void Ready::setupBuzzerRapidBeepCallback() {
  _buzzerManager->setOnRapidBeepCallback([this]() {
    // TODO: Set callback for rapid beep. This should increment or decrement the
    // fill capacity.
  });
}