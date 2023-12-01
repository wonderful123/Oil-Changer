#include "ConfigManager.h"
#include "Error.h"
#include "HardwareConfig.h"
#include "InteractionSettingsConfig.h"
#include <map>
#include <memory>
#include <string>

// Responsible for loading configuration data from a source (like a JSON file)
// and passing it to the appropriate managers

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

  if (configType == "HardwareConfig") {
    config = std::make_shared<HardwareConfig>(_fileHandler);
    filePath = "/config/hardwareConfig.json";
  } else if (configType == "InteractionSettingsConfig") {
    config = std::make_shared<InteractionSettingsConfig>(_fileHandler);
    filePath = "/config/interactionSettingsConfig.json";
  } else {
    return Error(Error::ConfigTypeNotRecognized);
  }

  Error error = config->load(filePath);
  if (error == Error::OK) {
    configs[configType] = config;
    notifyObservers(); // Notify observers after updating config
  }

  return error;
}
