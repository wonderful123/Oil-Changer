#include "SystemController.h"
#include "FSM/States.h"
#include "Logger.h"

#include "ESP32/ESP32SerialDisplay.h"

FSM_INITIAL_STATE(BaseState, IdleState);

SystemController::SystemController(
    std::shared_ptr<HardwareManager> hardwareManager,
    std::shared_ptr<ButtonController> buttonController)
    : _hardwareManager(std::move(hardwareManager)),
      _buttonController(std::move(buttonController)) {}

void SystemController::initialize() {
  _hardwareManager->initializeHardware();
  registerAsButtonObserver();

  Logger::info("[SystemController] Starting state machine...");
  // Start state machine in IdleState
  IdleState::start(); 

  auto component = _hardwareManager->getComponentById("Serial");
  if (!component) {
    Logger::warn("[SystemController] Failed to find component with id Serial");
    return;
  }

  // Attempt to cast to a shared pointer of type ISerial
  auto serial = static_cast<ISerial *>(component.get());
  if (!serial) {
    Logger::warn("[SystemController] Failed to cast to ISerial");
    return;
  }
  Logger::info("[SystemController] Created Serial component");
  SerialDisplay display(*serial);
  Logger::info("[SystemController] Created SerialDisplay component");
  display.displayData("Hello, World!");
  Logger::info("[SystemController] Displayed data on SerialDisplay");
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

void SystemController::update(EventType eventType) {
  _buttonController->processButtonStates();
}