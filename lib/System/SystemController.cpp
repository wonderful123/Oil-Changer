#include "SystemController.h"

SystemController::SystemController(
    std::shared_ptr<HardwareManager> hardwareManager)
    : _hardwareManager(std::move(hardwareManager)) {}

void SystemController::initialize() {
  // Initialize hardware and other components
  _hardwareManager->initializeHardware();

  // Register this SystemController as an observer to ButtonController
  registerAsButtonObserver();

  // Additional initialization logic
}

void SystemController::onButtonPress(int buttonId) {
  // Handle button press event
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
  // Delegate to state machine for handling events
  _stateMachine.handleEvent(event);
}

void SystemController::changeSystemState() {
  // Logic to change the state of the system
}

void SystemController::registerAsButtonObserver() {
  // Logic to register this SystemController as an observer to the
  // ButtonController This might involve getting an instance/reference to
  // ButtonController and calling addObserver Example:
  // _buttonController->addObserver(shared_from_this());
}