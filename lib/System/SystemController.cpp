#include "SystemController.h"
#include "ESP32/ESP32InterfaceDisplay.h"
#include "FSM/States.h"
#include "Logger.h"

FSM_INITIAL_STATE(BaseState, IdleState);

SystemController::SystemController() {
  _hardwareFactory = DIContainer::resolve<HardwareFactory>();
  _buttonController = DIContainer::resolve<ButtonController>();
}

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
  auto component2 = _hardwareManager->getComponentById("Display1");
  Logger::info("[SystemController] Created SerialDisplay component");
  auto display = static_cast<IDisplay *>(component2.get());
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