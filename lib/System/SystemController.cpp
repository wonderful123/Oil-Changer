#include "SystemController.h"

// External dependencies
#include <memory>

// Project-specific headers
#include "AutoRepeatHandler.h"
#include "ButtonController.h"
#include "BuzzerManager.h"
#include "ConfigManager.h"
#include "ConfigTypes.h"
#include "HardwareFactory.h"
#include "HardwareManager.h"
#include "IButton.h"
#include "IBuzzer.h"
#include "InteractionSettings.h"
#include "InteractionSettingsConfig.h"
#include "Logger.h"

SystemController::SystemController(
    std::shared_ptr<IMediator> mediator,
    std::shared_ptr<HardwareManager> hardwareManager)
    : _hardwareManager(hardwareManager) {}

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
  _buttonController->attach(_autoRepeatHandler.get());
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
  _buttonController->attach(_buzzerManager.get()); // Attach BuzzerManager as an
                                             // observer to ButtonController
  return Error::OK;
}

void SystemController::performPeriodicUpdate() {
  if (_buttonController) {
    _buttonController->processButtonStates();
  }
  if (_autoRepeatHandler) {
    _autoRepeatHandler->checkAutoRepeat();
  }
  if (_mediator) {
    _mediator->processEvents();
  }
}

std::shared_ptr<ButtonController> SystemController::getButtonController() {
  return _buttonController;
}

std::shared_ptr<BuzzerManager> SystemController::getBuzzerManager() {
  return _buzzerManager;
}