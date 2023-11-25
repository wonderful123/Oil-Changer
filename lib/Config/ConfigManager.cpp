#include "ConfigManager.h"
#include "Error.h"
#include "HardwareConfig.h"
#include <memory>

// Responsible for loading configuration data from a source (like a JSON file)
// and passing it to the appropriate managers

std::shared_ptr<HardwareConfig> ConfigManager::getHardwareConfig() const {
  auto it = configs.find("HardwareConfig");
  if (it != configs.end()) {
    return std::static_pointer_cast<HardwareConfig>(it->second);
  }
  return nullptr; // Return nullptr to indicate config not found
}

Error ConfigManager::loadConfig(const std::string &configType) {
  // Check if the config type is recognized
  if (configType == "HardwareConfig") {
    // Create and load the HardwareConfig object
    auto hardwareConfig = std::make_shared<HardwareConfig>(_fileHandler);
    Error error = hardwareConfig->load("/config/hardwareConfig.json");

    // Add the loaded config to the `configs` map if loading was successful
    if (error == Error::OK) {
      configs[configType] = hardwareConfig;
    }

    return error;
  } else {
    // Handle other config types here
  }

  // Config type not recognized
  return Error(Error::ConfigTypeNotRecognized);
}