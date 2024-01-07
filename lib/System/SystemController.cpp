// SystemController.cpp

#include "SystemController.h"

// External dependencies
#include <memory>

// Project-specific headers
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
#include "MotorController.h"

SystemController::SystemController(
    std::shared_ptr<HardwareManager> hardwareManager,
    std::shared_ptr<EventManager> eventManager)
    : _hardwareManager(hardwareManager), _eventManager(eventManager) {}

void SystemController::initializeSystemComponents() {
  if (!loadInteractionSettings()) {
    Logger::error("[SystemController] Failed to load interaction settings");
    return;
  }

  Logger::info("[SystemController] Creating EventManager");
  if (initializeMotorController(_eventManager) != Error::OK) {
    Logger::error("[SystemController] Failed to initialize Event Manager");
    return;
  }

  Logger::info("[SystemController] Creating ButtonController");
  if (initializeButtonController(_interactionSettings) != Error::OK) {
    Logger::error("[SystemController] Failed to initialize Button Controller");
    return;
  }

  Logger::info("[SystemController] Creating BuzzerManager");
  if (initializeBuzzerManager(_interactionSettings, _eventManager) !=
      Error::OK) {
    Logger::error("[SystemController] Failed to initialize Buzzer Manager");
    return;
  }

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

Error SystemController::initializeButtonController(
    std::shared_ptr<InteractionSettings> &interactionSettings) {
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
  for (auto button : buttonComponents) {
    _buttonController->registerButton(button);
  }

  return Error::OK;
}

Error SystemController::initializeBuzzerManager(
    std::shared_ptr<InteractionSettings> &interactionSettings,
    std::shared_ptr<EventManager> eventManager) {
  auto buzzer = _hardwareManager->getComponentById<IBuzzer>("Buzzer");
  if (!buzzer) {
    return Error::HardwareConfigBuzzerInitError;
  }

  _buzzerManager = std::make_shared<BuzzerManager>(buzzer, interactionSettings, eventManager);

  return Error::OK;
}

Error SystemController::initializeMotorController(
    std::shared_ptr<EventManager> eventManager) {
  auto speedControlDAC = _hardwareManager->getComponentById<IDAC>("MotorSpeed");
  auto motorControlFill =
      _hardwareManager->getComponentById<IDigitalIO>("MotorControlFill");
  auto motorControlExtract =
      _hardwareManager->getComponentById<IDigitalIO>("MotorControlExtract");

  _motorController = std::make_shared<MotorController>(eventManager);

  _motorController->registerDacComponent(speedControlDAC);
  _motorController->registerDigitalIO(motorControlFill, motorControlExtract);

  return Error::OK;
}

void SystemController::performPeriodicUpdate() {
  if (_buttonController) {
    _buttonController->processButtonStates();
  }

  if (_motorController) {
    _motorController->update(); // For motor speed ramping
  }
}

std::shared_ptr<ButtonController> SystemController::getButtonController() {
  return _buttonController;
}

std::shared_ptr<BuzzerManager> SystemController::getBuzzerManager() {
  return _buzzerManager;
}