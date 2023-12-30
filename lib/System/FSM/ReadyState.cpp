#include "ReadyState.h"

#include "ConcreteMediator.h"
#include "Logger.h"
#include "States.h"

Ready::Ready() {
  auto &systemFactory = SystemFactory::getInstance();
  _buzzerManager = systemFactory.getSystemController()->getBuzzerManager();
  _buttonController = systemFactory.getButtonController();
  _mediator = systemFactory.getMediator();
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
  if (id == "ButtonPlus") {
    _mediator->notify("increment_fill_capacity"); // Mediator handles increment
    Logger::info("[StateMachine: Ready] Fill capacity incremented.");
  } else if (id == "ButtonMinus") {
    _mediator.decrementFillCapacity(); // Mediator handles decrement
    Logger::info("[StateMachine: Ready] Fill capacity decremented.");
  } else if (id == "ButtonStart") {
    Logger::info("[StateMachine: Ready] Starting oil change.");
    transit<Extracting>();
  }
}

void Ready::handleButtonPress(const std::string &id) {
  auto &oilChangeTracker = OilChangeTracker::getInstance();

  if (id == "ButtonPlus") {
    // Notify the mediator to increment the fill capacity

    Logger::info("[StateMachine: Ready] Fill capacity incremented.");
  } else if (id == "ButtonMinus") {
    // Notify the mediator to decrement the fill capacity
    Logger::info("[StateMachine: Ready] Fill capacity decremented.");
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