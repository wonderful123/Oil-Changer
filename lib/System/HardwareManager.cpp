#include "HardwareManager.h"

#include "ButtonController.h"
#include "ConfigManager.h"
#include "HardwareComponent.h"
#include "HardwareConfig.h"
#include "HardwareFactory.h"
#include "HardwareInitializer.h"
#include "HardwarePinConfig.h"
#include "IButton.h"
#include "IBuzzer.h"
#include "InteractionSettings.h"
#include "InteractionSettingsConfig.h"
#include "Logger.h"
#include "Mediator/ConcreteMediator.h"

HardwareManager::HardwareManager(
    std::shared_ptr<IMediator> mediator,
    std::shared_ptr<ConfigManager> configManager,
    std::shared_ptr<HardwareFactory> hardwareFactory,
    std::shared_ptr<ButtonController> buttonController)
    : IColleague(mediator),  // Call to IColleague constructor
      _mediator(mediator),
      _configManager(configManager),
      _hardwareFactory(hardwareFactory),
      _buttonController(buttonController) {
  _mediator->registerColleague(this);
}

void HardwareManager::initialize() {
  HardwareInitializer initializer(_configManager, _hardwareFactory,
                                  _buttonController, _buzzerManager, _mediator);
                                  
  if (initializer.initialize(_components)) {
    Logger::info(
        "[HardwareManager] All hardware components initialized successfully");
  } else {
    Logger::error(
        "[HardwareManager] Some hardware components failed to initialize");
  }
}

bool HardwareManager::isComponentInitialized(
    const std::string &componentId) const {
  // Iterate through all components in the _components map
  for (const auto &pair : _components) {
    const auto &id = pair.first;
    const auto &component = pair.second;
    // Check if the component type matches and whether it is initialized
    if (id == componentId && component->isInitialized()) {
      return true;
    }
  }
  return false;
}

std::shared_ptr<HardwareComponent> HardwareManager::getComponentById(
    const std::string &id) const {
  auto it = _components.find(id);
  if (it != _components.end()) {
    return it->second;
  }

  return nullptr;  // Return nullptr if component not found
}

void HardwareManager::receiveEvent(EventType eventType,
                                   const EventData *eventData) {
  Logger::info("[HardwareManager] Received an update notification.");

  // Handle different event types
  switch (eventType) {
    case OIL_LEVEL_CHANGED:
      // Handle oil level change
      break;
    case VOLTAGE_CHANGED:
      // Handle voltage change
      break;
    case FLOW_RATE_CHANGED:
      // Handle flow rate change
      break;
    default:
      Logger::warn("[HardwareManager] Unknown event type received.");
      break;
  }

  auto newSettingsConfig = _configManager->getInteractionSettingsConfig();
  if (newSettingsConfig) {
    InteractionSettings newSettings = newSettingsConfig->getSettings();
    _buttonController->setInteractionSettings(newSettings);
  }
}

void HardwareManager::notifyMediator(EventType eventType) {
  if (_mediator) {
    _mediator->notify(this, eventType, {/* Additional event data if needed */});
  }
}

void HardwareManager::triggerBuzzer() {
  _buzzerManager->triggerBuzzer(2731, 150);
}
