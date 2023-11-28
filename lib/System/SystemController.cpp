#include "SystemController.h"
#include "FSM/States.h"
#include "Logger.h"

SystemController::SystemController(
    std::shared_ptr<HardwareManager> hardwareManager,
    std::shared_ptr<ButtonController> buttonController)
    : _hardwareManager(std::move(hardwareManager)),
      _buttonController(std::move(buttonController)) {}

void SystemController::initialize() {
  _hardwareManager->initializeHardware();
  registerAsButtonObserver();
}

void SystemController::onButtonPress(const std::string &id) {
  _hardwareManager->triggerBuzzer();
  ButtonPressEvent event(id);
  // Use the state machine to handle the event
  _stateMachine.handleEvent(event);
}

void SystemController::registerAsButtonObserver() {
  if (_buttonController) {
    _buttonController->addObserver(shared_from_this());
  }
}

void SystemController::update() { _buttonController->processButtonStates(); }
