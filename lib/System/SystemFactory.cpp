// SystemFactory.cpp
#include "SystemFactory.h"
#include "ButtonController.h"
#include "ConfigManager.h"
#include "DisplayManager.h"
#include "EventManager/EventManager.h"
#include "FSM/StateMachine.h"
#include "FSM/States.h"
#include "HardwareManager.h"
#include "IADC.h"
#include "IBuzzer.h"
#include "IDAC.h"
#include "IFlowMeter.h"
#include "InteractionSettings.h"
#include "InteractionSettingsConfig.h"
#include "Logger.h"
#include "MotorController.h"
#include "MotorSettingsConfig.h"
#include "OilChangeTracker.h"
#include "SensorManager.h"
#include "SystemController.h"

FSM_INITIAL_STATE(StateMachine, Initializing);

SystemFactory &SystemFactory::getInstance() {
  static SystemFactory instance;
  return instance;
}

void SystemFactory::initializeSystem(
    std::shared_ptr<IFileHandler> fileHandler) {
  _fileHandler = std::move(fileHandler);
  createEventManager();
  createConfigManager();
  createOilChangeTracker();
  createHardwareManager();
  createDisplayManager();
  loadInteractionSettings();
  createButtonController();
  createMotorController();
  createSensorManager();
  createSystemController();
  createBuzzerManager();
  createStateMachine();

  Logger::info("[SystemFactory] All system components initialized");
  sendFSMEvent(InitializationCompleteEvent());
}

void SystemFactory::createEventManager() {
  _eventManager = std::make_shared<EventManager>();
  Logger::info("[SystemFactory] Created EventManager");
}

void SystemFactory::createConfigManager() {
  Logger::info("[SystemFactory] Creating ConfigManager");
  _configManager = std::make_shared<ConfigManager>(_fileHandler);
}

void SystemFactory::createHardwareManager() {
  Logger::info("[SystemFactory] Creating HardwareManager");
  _hardwareManager = std::make_shared<HardwareManager>(_configManager);
  _hardwareManager->initialize();
}

void SystemFactory::createSystemController() {
  Logger::info("[SystemFactory] Creating SystemController...");
  _systemController = std::make_shared<SystemController>(
      _buttonController, _motorController, _sensorManager, _displayManager);
}

void SystemFactory::createOilChangeTracker() {
  Logger::info("[SystemFactory] Creating OilChangeTracker");
  _oilChangeTracker = std::make_shared<OilChangeTracker>(_eventManager);
  _oilChangeTracker->initialize();
}

void SystemFactory::createStateMachine() {
  StateMachine::setSharedResources(_eventManager, _buzzerManager);
  StateMachine::start();
  Logger::info("[SystemFactory] Created StateMachine");
}

void SystemFactory::createDisplayManager() {
  auto displays = _hardwareManager->getDisplays();
  if (displays.empty()) {
    Logger::error(
        "[SystemFactory] No displays found in hardware configuration");
    return;
  }
  _displayManager = std::make_shared<DisplayManager>(
      _eventManager, _oilChangeTracker, displays);
  _displayManager->initialize();
  Logger::info("[SystemFactory] Created DisplayManager");
}

void SystemFactory::createButtonController() {
  Logger::info("[SystemFactory] Creating ButtonController");
  _buttonController = std::make_shared<ButtonController>(_interactionSettings);

  auto buttonComponents =
      _hardwareManager->getComponentsByType<IButton>("Button");
  if (buttonComponents.empty()) {
    Logger::error(
        "[SystemController] No buttons found in hardware configuration");
    // return Error(Error::HardwareConfigButtonsNotFound);
  }

  // Register buttons
  for (auto button : buttonComponents) {
    _buttonController->registerButton(button);
  }
}

void SystemFactory::createBuzzerManager() {
  auto buzzer = _hardwareManager->getComponentById<IBuzzer>("Buzzer");
  if (!buzzer) {
    Logger::error("[SystemFactory] Buzzer initialization error");
    return;
  }
  _buzzerManager = std::make_shared<BuzzerManager>(buzzer, _interactionSettings,
                                                   _eventManager);
  _buzzerManager->initialize();
  Logger::info("[SystemFactory] Created BuzzerManager");
}

void SystemFactory::createMotorController() {
  Logger::info("[SystemFactory] Creating MotorController");
  // Get settings config for motor controller
  auto motorSettingsConfig =
      _configManager->getConfig<MotorSettingsConfig>(ConfigType::MOTOR);
  if (!motorSettingsConfig) {
    Logger::error("[SystemController] Motor settings config not found");
  }

  auto motorSettings = motorSettingsConfig->getSettings();

  auto speedControlDAC = _hardwareManager->getComponentById<IDAC>("MotorSpeed");
  auto motorControlFill =
      _hardwareManager->getComponentById<IDigitalIO>("MotorControlFill");
  auto motorControlExtract =
      _hardwareManager->getComponentById<IDigitalIO>("MotorControlExtract");

  _motorController =
      std::make_shared<MotorController>(_eventManager, motorSettings);
  _motorController->initialize(speedControlDAC, motorControlFill,
                               motorControlExtract);
}

void SystemFactory::createSensorManager() {
  Logger::info("[SystemFactory] Creating SensorManager");
  SensorManagerComponents components;
  components.oilChangeTracker = _oilChangeTracker;
  components.voltageSensor =
      _hardwareManager->getComponentById<IADC>("VoltageSense");
  components.oilTemperatureSensor =
      _hardwareManager->getComponentById<IADC>("OilTemp");
  components.fillFlowMeter =
      _hardwareManager->getComponentById<IFlowMeter>("FlowMeterFill");
  components.fillPressureSwitch =
      _hardwareManager->getComponentById<IDigitalIO>("LPSFill");
  components.extractFlowMeter =
      _hardwareManager->getComponentById<IFlowMeter>("FlowMeterExtract");
  components.extractPressureSwitch =
      _hardwareManager->getComponentById<IDigitalIO>("LPSExtract");

  _sensorManager = std::make_shared<SensorManager>();
  _sensorManager->initialize(components);
}

bool SystemFactory::loadInteractionSettings() {
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

std::shared_ptr<ConfigManager> SystemFactory::getConfigManager() const {
  return _configManager;
}

std::shared_ptr<EventManager> SystemFactory::getEventManager() const {
  return _eventManager;
}

std::shared_ptr<SystemController> SystemFactory::getSystemController() const {
  return _systemController;
}

std::shared_ptr<BuzzerManager> SystemFactory::getBuzzerManager() const {
  return _buzzerManager;
}

std::shared_ptr<HardwareManager> SystemFactory::getHardwareManager() const {
  return _hardwareManager;
}

std::shared_ptr<ButtonController> SystemFactory::getButtonController() const {
  return _buttonController;
}

std::shared_ptr<DisplayManager> SystemFactory::getDisplayManager() const {
  return _displayManager;
}