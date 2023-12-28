#include "SystemController.h"

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
    std::shared_ptr<HardwareManager> hardwareManager,
    std::shared_ptr<ConfigManager> configManager)
    : IColleague(mediator),
      _mediator(mediator),
      _hardwareManager(hardwareManager),
      _configManager(configManager) {
  _mediator->registerColleague(this);
  _stateMachine.start();  // Initialize the state machine
}

void SystemController::initializeSystemComponents() {
  initializeButtonController();
  initializeBuzzerManager();
  Logger::info("[SystemFactory] System components initialized");
}

Error SystemController::initializeButtonController() {
  _buttonController = std::make_shared<ButtonController>(_mediator);
  // Check and log if button controller is not created
  if (!_buttonController) {
    Logger::error("[SystemController] Button controller not created");
    return Error::HardwareConfigButtonsNotFound;
  }

  auto buttonComponents = _hardwareManager->getComponentsByType<IButton>("Button");
  if (buttonComponents.empty()) {
    Logger::error(
        "[SystemController] No buttons found in hardware configuration");
    return Error::HardwareConfigButtonsNotFound;
  }

  // Register buttons
  for (const auto &button : buttonComponents) {
    _buttonController->registerButton(button->id(), button);
  }

  auto interactionSettingsConfig =
      _configManager->getConfig<InteractionSettingsConfig>(
          ConfigType::INTERACTION_SETTINGS);
  if (!interactionSettingsConfig) {
    return Error::ConfigManagerInteractionSettingsError;
  }

  auto interactionSettings = interactionSettingsConfig->getSettings();
  _buttonController->setInteractionSettings(interactionSettings);

  return Error::OK;
}

Error SystemController::initializeBuzzerManager() {
  auto buzzer = _hardwareManager->getComponentById<IBuzzer>("Buzzer");
  if (!buzzer) {
    return Error::HardwareConfigBuzzerInitError;
  }

  auto interactionSettingsConfig =
      _configManager->getConfig<InteractionSettingsConfig>(
          ConfigType::INTERACTION_SETTINGS);
  if (!interactionSettingsConfig) {
    return Error::ConfigManagerInteractionSettingsError;
  }

  auto settings = interactionSettingsConfig->getSettings();

  _buzzerManager = std::make_shared<BuzzerManager>(_mediator, buzzer, settings);

  return Error::OK;
}

void SystemController::handleEvent(const EventType &eventType) {
  // Handle events communicated by the mediator
  // Process the event as needed
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
}