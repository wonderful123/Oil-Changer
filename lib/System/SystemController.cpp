#include "SystemController.h"
#include "FSM/States.h"

SystemController::SystemController(
    std::shared_ptr<HardwareManager> hardwareManager,
    std::shared_ptr<ButtonController> buttonController)
    : _hardwareManager(std::move(hardwareManager)),
      _buttonController(std::move(buttonController)) {}

void SystemController::initialize() {
  _hardwareManager->initializeHardware();
  registerAsButtonObserver();
}

void SystemController::startSystem() {
  // Logic to start the system
}

void SystemController::stopSystem() {
  // Logic to stop the system
}
void SystemController::onButtonPress(const std::string &id) {
  _hardwareManager->triggerBuzzer();

  // Create a ButtonPressEvent
  ButtonPressEvent event(id);

  // Use the state machine to handle the event
  // Assuming _stateMachine is an instance of StateMachine
  _stateMachine.handleEvent(event);

  // Note: The actual state transition logic is handled within the state classes
}

void SystemController::changeSystemState() {
  // Logic to change the state of the system
}

void SystemController::registerAsButtonObserver() {
  if (_buttonController) {
    _buttonController->addObserver(shared_from_this());
  }
}

void SystemController::update() {
  // Implementation details...
}