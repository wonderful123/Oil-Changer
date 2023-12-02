#include "HardwareManager.h"
#include "Logger.h"

HardwareManager::HardwareManager(
    std::shared_ptr<ConfigManager> configManager,
    std::shared_ptr<HardwareFactory> hardwareFactory,
    std::shared_ptr<ButtonController> buttonController)
    : _configManager(configManager), _hardwareFactory(hardwareFactory),
      _buttonController(buttonController) {
  _configManager->attach(this);
}

void HardwareManager::initializeHardware() {
  Logger::debug("Starting hardware initialization");
  auto hardwareConfig = _configManager->getHardwareConfig();
  if (!hardwareConfig) {
    Logger::error("Hardware configuration is not available");
    return;
  }

  bool allComponentsInitialized = true;
  for (const auto &config : hardwareConfig->getHardwarePinConfigs()) {
    if (!initializeComponent(config)) {
      allComponentsInitialized = false;
      break; // Stop if any component fails
    }
  }

  if (allComponentsInitialized) {
    Logger::info("Hardware initialization successful");
  } else {
    Logger::error("Some hardware components failed to initialize");
  }
  Logger::debug("Hardware initialization complete");
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
        Logger::error("Failed to cast to IButton: " + config.id);
      }
    }

    Logger::info("Created component: " + config.id + " on pin " +
                 std::to_string(config.pinNumber));
  } else {
    Logger::error("Failed to create component: " + config.id);
    return false;
  }
  return true;
}

void HardwareManager::registerComponent(
    const HardwarePinConfig &config,
    const std::shared_ptr<HardwareComponent> &component) {
  if (config.type == "Button") {
    auto button = std::static_pointer_cast<IButton>(component);
    if (button) {
      _buttonController->registerButton(config.id, button);
    } else {
      Logger::error("Failed to cast to IButton: " + config.id);
    }
  }
  // Additional component type checks can be added here
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

std::shared_ptr<HardwareComponent>
HardwareManager::getComponentById(const std::string &id) const {
  auto it = _components.find(id);
  if (it != _components.end()) {
    return it->second;
  }

  return nullptr; // Return nullptr if component not found
}

void HardwareManager::update() {
  Logger::info("HardwareManager received an update notification.");

  auto newSettingsConfig = _configManager->getInteractionSettingsConfig();
  if (newSettingsConfig) {
    InteractionSettings newSettings = newSettingsConfig->getSettings();
    _buttonController->setInteractionSettings(newSettings);
  }

  updateBuzzerSettings();
}

void HardwareManager::onButtonEvent(const std::string &buttonId, bool pressed) {
  if (pressed) {
    handleButtonEvent(buttonId);
  }
}

void HardwareManager::handleButtonEvent(const std::string &buttonId) {
  changeStateBasedOnButton(buttonId);
}

void HardwareManager::changeStateBasedOnButton(const std::string &buttonId) {
  // State change logic based on button ID
  StateMachine stateMachine;
  ButtonPressEvent pressEvent(
      buttonId); // Assuming ButtonPressEvent now takes a string ID
  stateMachine.handleEvent(pressEvent);
}

void HardwareManager::triggerBuzzer() {
  auto it = _components.find("Buzzer");
  if (it != _components.end() && it->second->type() == "Buzzer") {
    auto buzzer = std::static_pointer_cast<IBuzzer>(it->second);
    if (buzzer) {
      buzzer->beep(2731, 150); // Example frequency and duration
    } else {
      Logger::error("Buzzer component cast failed.");
    }
  } else {
    Logger::error("Buzzer component not found or type mismatch.");
  }
}

void HardwareManager::updateBuzzerSettings() {
  auto interactionConfig = _configManager->getInteractionSettingsConfig();
  if (!interactionConfig) {
    Logger::error("Interaction settings configuration is not available");
    return;
  }

  auto settings = interactionConfig->getSettings();
  auto buzzerIt = _components.find("Buzzer");
  if (buzzerIt != _components.end()) {
    auto buzzer = std::static_pointer_cast<IBuzzer>(buzzerIt->second);
    if (buzzer) {
      buzzer->updateSettings(settings);
    } else {
      Logger::error("Buzzer component cast failed.");
    }
  } else {
    Logger::error("Buzzer component not found.");
  }
}
