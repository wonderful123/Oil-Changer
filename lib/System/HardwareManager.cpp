#include "HardwareManager.h"

#include "ButtonController.h"
#include "ConfigManager.h"
#include "HardwareComponent.h"
#include "HardwareConfig.h"
#include "HardwareFactory.h"
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

void HardwareManager::initializeHardware() {
  Logger::info("[HardwareManager] Starting hardware initialization");
  auto hardwareConfig = _configManager->getHardwareConfig();
  if (!hardwareConfig) {
    Logger::error("[HardwareManager] Hardware configuration is not available");
    return;
  }

  bool allComponentsInitialized = true;
  for (const auto &config : hardwareConfig->getHardwarePinConfigs()) {
    if (!initializeComponent(config)) {
      allComponentsInitialized = false;
      break;  // Stop if any component fails
    }
  }

  if (allComponentsInitialized) {
    Logger::info("[HardwareManager] All hardware components initialized");
  } else {
    Logger::error(
        "[HardwareManager] Some hardware components failed to initialize");
  }
}

bool HardwareManager::initializeComponent(const HardwarePinConfig &config) {
  auto component = _hardwareFactory->createComponent(config);
  if (component) {
    std::shared_ptr<HardwareComponent> sharedComponent = std::move(component);
    _components[config.id] = sharedComponent;

    // Delegate button registration to ButtonController
    if (config.type == "Button") {
      auto button = std::static_pointer_cast<IButton>(sharedComponent);
      if (button) {
        _buttonController->registerButton(config.id, button);
      } else {
        Logger::error("[HardwareManager] Failed to cast to IButton: " +
                      config.id);
      }
    }

    Logger::info("[HardwareManager] Created component: " + config.id);
  } else {
    Logger::error("[HardwareManager] Failed to create component: " + config.id);
    return false;
  }

  return true;
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

  updateBuzzerSettings();
}

void HardwareManager::notifyMediator(EventType eventType) {
  if (_mediator) {
    _mediator->notify(this, eventType, {/* Additional event data if needed */});
  }
}

void HardwareManager::triggerBuzzer() {
  auto it = _components.find("Buzzer");
  if (it != _components.end() && it->second->type() == "Buzzer") {
    auto buzzer = std::static_pointer_cast<IBuzzer>(it->second);
    if (buzzer) {
      buzzer->beep(2731, 150);  // Example frequency and duration
    } else {
      Logger::error("[HardwareManager] Buzzer component cast failed.");
    }
  } else {
    Logger::error(
        "[HardwareManager] Buzzer component not found or type mismatch.");
  }
}

void HardwareManager::updateBuzzerSettings() {
  auto interactionConfig = _configManager->getInteractionSettingsConfig();
  if (!interactionConfig) {
    Logger::error(
        "[HardwareManager] Interaction settings configuration is not "
        "available");
    return;
  }

  auto settings = interactionConfig->getSettings();
  auto buzzerIt = _components.find("Buzzer");
  if (buzzerIt != _components.end()) {
    auto buzzer = std::static_pointer_cast<IBuzzer>(buzzerIt->second);
    if (buzzer) {
      buzzer->updateSettings(settings);
    } else {
      Logger::error("[HardwareManager] Buzzer component cast failed.");
    }
  } else {
    Logger::error("[HardwareManager] Buzzer component not found.");
  }
}
