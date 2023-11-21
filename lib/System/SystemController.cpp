#include "SystemController.h"

SystemController::SystemController(
    std::shared_ptr<HardwareManager> hardwareManager,
    std::shared_ptr<ButtonController> buttonController)
    : _hardwareManager(std::move(hardwareManager)),
      _buttonController(std::move(buttonController)) {}

void SystemController::initialize() {
  _hardwareManager->initializeHardware();
  registerAsButtonObserver();
}

void SystemController::onButtonPress(int buttonId) {
  ButtonPressEvent pressEvent(buttonId);
  _stateMachine.handleEvent(pressEvent);
}

void SystemController::startSystem() {
  // Logic to start the system
}

void SystemController::stopSystem() {
  // Logic to stop the system
}

void SystemController::handleEvent(const Event &event) {
  _stateMachine.handleEvent(event);
}

void SystemController::changeSystemState() {
  // Logic to change the state of the system
}

void SystemController::registerAsButtonObserver() {
  if (_buttonController) {
    _buttonController->addObserver(shared_from_this());
  }
}
