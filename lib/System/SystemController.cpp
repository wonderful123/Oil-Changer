#include "SystemController.h"

// External dependencies
#include <memory>

// Project-specific headers
#include "AutoRepeatHandler.h"
#include "ButtonController.h"
#include "ConfigTypes.h"
#include "FSM/StateMachine.h"
#include "FSM/States.h"
#include "HardwareFactory.h"
#include "HardwareManager.h"
#include "IButton.h"
#include "IBuzzer.h"
#include "Logger.h"

FSM_INITIAL_STATE(StateMachine, Ready);

SystemController::SystemController(
    std::shared_ptr<IMediator> mediator,
    std::shared_ptr<HardwareManager> hardwareManager)
    : IColleague(mediator), _mediator(mediator),
      _hardwareManager(hardwareManager) {
  _mediator->registerColleague(this);
  _stateMachine.start(); // Initialize the state machine
}

void SystemController::initializeSystemComponents() {
  if (!loadInteractionSettings()) {
    Logger::error("[SystemController] Failed to load interaction settings");
    return;
  }

  if (initializeButtonController() != Error::OK) {
    Logger::error("[SystemController] Failed to initialize Button Controller");
    return;
  }

  if (initializeBuzzerManager(_interactionSettings) != Error::OK) {
    Logger::error("[SystemController] Failed to initialize Buzzer Manager");
    return;
  }

  initializeAutoRepeatHandler(_interactionSettings);
  Logger::info("[SystemController] System components initialized");
}

bool SystemController::loadInteractionSettings() {
  auto configManager = ConfigManager::getInstance();
  auto interactionSettingsConfig =
      configManager->getConfig<InteractionSettingsConfig>(
          ConfigType::INTERACTION_SETTINGS);

  if (!interactionSettingsConfig) {
    Logger::error("[SystemController] Interaction settings config not found");
    return false;
  }

  _interactionSettings = interactionSettingsConfig->getSettings();
  return true;
}

void SystemController::initializeAutoRepeatHandler(
    std::shared_ptr<InteractionSettings> &interactionSettings) {
  _autoRepeatHandler = std::make_shared<AutoRepeatHandler>(_buttonController,
                                                           interactionSettings);
  _buttonController->attach(_autoRepeatHandler);
}

Error SystemController::initializeButtonController() {
  _buttonController = std::make_shared<ButtonController>();

  // Check and log if button controller is not created
  if (!_buttonController) {
    Logger::error("[SystemController] Button controller not created");
    return Error::HardwareConfigButtonsNotFound;
  }

  auto buttonComponents =
      _hardwareManager->getComponentsByType<IButton>("Button");
  if (buttonComponents.empty()) {
    Logger::error(
        "[SystemController] No buttons found in hardware configuration");
    return Error::HardwareConfigButtonsNotFound;
  }

  // Register buttons
  for (const auto &button : buttonComponents) {
    _buttonController->registerButton(button->id(), button);
  }

  return Error::OK;
}

Error SystemController::initializeBuzzerManager(
    std::shared_ptr<InteractionSettings> &interactionSettings) {
  auto buzzer = _hardwareManager->getComponentById<IBuzzer>("Buzzer");
  if (!buzzer) {
    return Error::HardwareConfigBuzzerInitError;
  }

  _buzzerManager = std::make_shared<BuzzerManager>(buzzer, interactionSettings);
  _buttonController->attach(_buzzerManager); // Attach BuzzerManager as an
                                             // observer to ButtonController
  return Error::OK;
}

void SystemController::receiveEvent(EventType eventType,
                                    const EventData *eventData) {
  // Handle events communicated by the mediator
  if (eventType == EventType::BUTTON_PRESSED) {
    onButtonPress(eventData->id);
  }
  // Other event types can be handled here as needed
}

void SystemController::onButtonPress(const std::string &id) {
  // Handle button press event
  Logger::info("[SystemController] Button pressed: " + id);
  ButtonPressEvent event(id);
  _stateMachine.dispatch(event);
}

void SystemController::update(EventType eventType) {
  // Notify the mediator about the event
  _mediator->notify(this, eventType);
}

void SystemController::performPeriodicUpdate() {
  _buttonController->processButtonStates();
  _autoRepeatHandler->checkAutoRepeat();
}

std::shared_ptr<ButtonController> SystemController::getButtonController() {
  return std::shared_ptr<ButtonController>();
}
