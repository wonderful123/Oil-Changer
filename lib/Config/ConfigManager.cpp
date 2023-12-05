#include "ConfigManager.h"
#include <string>

// Responsible for loading configuration data from a source (like a JSON file)
// and passing it to the appropriate managers

ConfigManager::ConfigManager() {
  _fileHandler = DIContainer::resolve<IFileHandler>();
}

std::shared_ptr<HardwareConfig> ConfigManager::getHardwareConfig() const {
  auto it = configs.find("HardwareConfig");
  if (it != configs.end()) {
    return std::static_pointer_cast<HardwareConfig>(it->second);
  }
  return nullptr; // Return nullptr to indicate config not found
}

std::shared_ptr<InteractionSettingsConfig>
ConfigManager::getInteractionSettingsConfig() const {
  auto it = configs.find("InteractionSettingsConfig");
  if (it != configs.end()) {
    return std::static_pointer_cast<InteractionSettingsConfig>(it->second);
  }
  return nullptr; // Return nullptr to indicate config not found
}

Error ConfigManager::loadConfig(const std::string &configType) {
  std::shared_ptr<BaseConfig> config;
  std::string filePath;
  EventType eventType;

  // Map config type to the corresponding file path and event type
  if (configType == "HardwareConfig") {
    config = std::make_shared<HardwareConfig>(_fileHandler);
    filePath = "/config/hardwareConfig.json";
    eventType = EventType::HARDWARE_CONFIG_CHANGED;
  } else if (configType == "InteractionSettings") {
    config = std::make_shared<InteractionSettingsConfig>(_fileHandler);
    filePath = "/config/interactionSettingsConfig.json";
    eventType = EventType::INTERACTION_SETTINGS_CHANGED;
  } else {
    return Error(Error::ConfigTypeNotRecognized);
  }

  Error error = config->load(filePath);
  if (error == Error::OK) {
    configs[configType] = config;
    notify(eventType);
  }

  return error;
}