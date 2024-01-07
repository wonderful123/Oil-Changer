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
#include "MotorSettingsConfig.h"

SystemController::SystemController(
    std::shared_ptr<ConfigManager> configManager,
    std::shared_ptr<HardwareManager> hardwareManager,
    std::shared_ptr<EventManager> eventManager)
    : _configManager(configManager), _hardwareManager(hardwareManager),
      _eventManager(eventManager) {}

void SystemController::initializeSystemComponents() {
  if (!loadInteractionSettings()) {
    Logger::error("[SystemController] Failed to load interaction settings");
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

  Logger::info("[SystemController] Creating MotorController");
  if (initializeMotorController(_eventManager) != Error::OK) {
    Logger::error("[SystemController] Failed to initialize Motor Controller");
    return;
  }

  Logger::info("[SystemController] System components initialized");
}

bool SystemController::loadInteractionSettings() {
  auto interactionSettingsConfig =
      _configManager->getConfig<InteractionSettingsConfig>(
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
  if (!eventManager)
    Logger::info("FUCKER");
  auto buzzer = _hardwareManager->getComponentById<IBuzzer>("Buzzer");
  if (!buzzer) {
    return Error::HardwareConfigBuzzerInitError;
  }

  _buzzerManager = std::make_shared<BuzzerManager>(buzzer, interactionSettings,
                                                   eventManager);
  _buzzerManager->initialize();

  return Error::OK;
}

Error SystemController::initializeMotorController(
    std::shared_ptr<EventManager> eventManager) {
  // Get settings config for motor controller
  auto motorSettingsConfig =
      _configManager->getConfig<MotorSettingsConfig>(ConfigType::MOTOR);
  if (!motorSettingsConfig) {
    Logger::error("[SystemController] Motor settings config not found");
    return Error(Error::MotorSettingsConfigNotLoaded);
  }

  auto motorSettings = motorSettingsConfig->getSettings();

  auto speedControlDAC = _hardwareManager->getComponentById<IDAC>("MotorSpeed");
  auto motorControlFill =
      _hardwareManager->getComponentById<IDigitalIO>("MotorControlFill");
  auto motorControlExtract =
      _hardwareManager->getComponentById<IDigitalIO>("MotorControlExtract");

  _motorController =
      std::make_shared<MotorController>(eventManager, motorSettings);
  _motorController->initialize(speedControlDAC, motorControlFill,
                               motorControlExtract);

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