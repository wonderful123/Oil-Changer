#include "HardwareManager.h"
#include "FSM/Events.h"
#include "FSM/StateMachine.h"
#include "HardwareManager.h"
#include "IBuzzer.h"
#include "Logger.h"

HardwareManager::HardwareManager(
    std::shared_ptr<ConfigManager> configManager,
    std::shared_ptr<HardwareFactory> hardwareFactory,
    std::shared_ptr<ButtonController> buttonController)
    : _configManager(configManager), _hardwareFactory(hardwareFactory),
      _buttonController(buttonController) {}

void HardwareManager::initializeHardware() {
  auto hardwareConfig = _configManager->getHardwareConfig();
  if (!hardwareConfig) {
    Logger::error("Hardware configuration is not available");
    return;
  }

  bool allComponentsInitialized = true;
  for (const auto &config : hardwareConfig->getHardwarePinConfigs()) {
    auto component = initializeComponent(config);
    if (!component) {
      allComponentsInitialized = false;
      break; // Stop initialization if any component fails
    }
  }

  if (allComponentsInitialized) {
    Logger::info("Hardware initialization successful");
  } else {
    Logger::error("Some hardware components failed to initialize");
  }
}

bool HardwareManager::initializeComponent(const HardwarePinConfig &config) {
  auto component = _hardwareFactory->createComponent(config);
  if (component) {
    // Convert unique_ptr to shared_ptr
    std::shared_ptr<HardwareComponent> sharedComponent = std::move(component);
    _components[config.id] = sharedComponent;
    Logger::info("Created component: " + config.id);

    // Register the component if needed
    registerComponent(config, sharedComponent);
  } else {
    Logger::error("Failed to create component: " + config.id);
    return false;
  }

  return true;
}

/**
 * Register a hardware component if needed.
 *
 * @param config The GPIO pin configuration for the component.
 * @param component The shared pointer to the hardware component to be
 * registered.
 *
 * @throws None.
 */
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
  // Add similar checks for other component types if needed
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
  // This method is called when the observed subject changes.

  // You can also log the update or perform other operations as needed.
  Logger::info("HardwareManager received an update notification.");

  // Implement specific logic based on your application's requirements.
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
      buzzer->beep(1000, 1000); // Example frequency and duration
      Logger::info("Buzzer beep triggered.");
    } else {
      Logger::error("Buzzer component cast failed.");
    }
  } else {
    Logger::error("Buzzer component not found or type mismatch.");
  }
}