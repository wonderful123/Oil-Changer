#include "ReadyState.h"
#include "ButtonController.h"
#include "BuzzerManager.h"
#include "Logger.h"
#include "Mediator/ConcreteMediator.h"
#include "States.h"
#include "SystemController.h"
#include "SystemFactory.h"

Ready::Ready() {}

void Ready::entry() {
  auto &systemFactory = SystemFactory::getInstance();
  _buzzerManager = systemFactory.getSystemController()->getBuzzerManager();
  _buttonController = systemFactory.getButtonController();
  _mediator = systemFactory.getMediator();
  setMediator(systemFactory.getMediator());

  // Attach to the button controller to listen for button presses
  _buttonController->attach(this);

  setupBuzzerRapidBeepCallback();
}

void Ready::exit() {
  _buttonController->detach(this);

  // Clear the callback for rapid beep
  _buzzerManager->setOnRapidBeepCallback(nullptr);

  StateMachine::exit();
}

void Ready::handleButtonPress(const std::string &buttonId) {
  EventData data;
  data.id = "fill_capacity";

  if (buttonId == "ButtonPlus") {
    data.value = 0.1; // Increment
    _mediator->notify(this, EventType::OIL_CHANGE_TRACKER_UPDATE, &data);
  } else if (buttonId == "ButtonMinus") {
    data.value = -0.1; // Decrement
    _mediator->notify(this, EventType::OIL_CHANGE_TRACKER_UPDATE, &data);
  } else if (buttonId == "ButtonStart") {
    Logger::info(
        "[StateMachine] Transition: Starting oil change extraction...");
    transit<Extracting>();
  }
}

void Ready::onNotify(const std::string &event, const std::string &buttonId) {
  Logger::info("[Ready] Button event: " + event);
  if (event == "button_pressed") {
    handleButtonPress(buttonId);
  } else if (event == "button_auto_repeat") {
    setupBuzzerRapidBeepCallback();
  }
}

void Ready::setupBuzzerRapidBeepCallback() {
  Logger::info("[Ready] setupBuzzerRapidBeepCallback");
  _buzzerManager->setOnRapidBeepCallback([this]() {
    EventData data;
    data.id = "fill_capacity";
    data.value = 0.1f; // Increment
    _mediator->notify(this, EventType::OIL_CHANGE_TRACKER_UPDATE, &data);
  });
}