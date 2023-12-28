#include "SystemController.h"

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
  auto configManager =
      ConfigManager::getInstance(); // Access the singleton instance
  auto interactionSettingsConfig =
      configManager->getConfig<InteractionSettingsConfig>(
          ConfigType::INTERACTION_SETTINGS);
  if (!interactionSettingsConfig) {
    Logger::error("[SystemController] Failed to load interaction settings");
    return; // Handle error appropriately
  }
  auto interactionSettings = interactionSettingsConfig->getSettings();

  initializeButtonController(interactionSettings);
  initializeBuzzerManager(interactionSettings);

  _autoRepeatHandler = std::make_shared<AutoRepeatHandler>(_buttonController,
                                                           interactionSettings);
  // Attach auto repeat handler to the button controller events
  _buttonController->attach(_autoRepeatHandler);

  Logger::info("[SystemFactory] System components initialized");
}

Error SystemController::initializeButtonController(
    InteractionSettings interactionSettings) {
  _buttonController = std::make_shared<ButtonController>(interactionSettings);

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
    InteractionSettings interactionSettings) {
  auto buzzer = _hardwareManager->getComponentById<IBuzzer>("Buzzer");
  if (!buzzer) {
    return Error::HardwareConfigBuzzerInitError;
  }

  _buzzerManager =
      std::make_shared<BuzzerManager>(_mediator, buzzer, interactionSettings);

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
  _autoRepeatHandler->checkAutoRepeat();
}