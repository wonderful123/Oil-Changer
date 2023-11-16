#include "ConfigManager.h"
#include "Error.h"
#include "HardwareConfig.h"
#include <memory>

ConfigManager::ConfigManager() {
  // Initialize other configurations if necessary
}

ConfigManager::~ConfigManager() {
  // Cleanup if necessary
}

std::shared_ptr<HardwareConfig> ConfigManager::getHardwareConfig() {
  auto it = configs.find("HardwareConfig");
  if (it != configs.end()) {
    return std::static_pointer_cast<HardwareConfig>(it->second);
  }
  return nullptr; // Return nullptr to indicate config not found
}

Error ConfigManager::loadConfig(const std::string &configType,
                                IFileHandler *fileHandler) {
  if (configType == "HardwareConfig") {
    auto hardwareConfig = std::make_shared<HardwareConfig>(fileHandler);
    Error loadError = hardwareConfig->load("hardwareConfig.json");
    if (loadError) {
      return loadError; // Propagate the error
    }
    configs[configType] = hardwareConfig;
    return Error::OK; // Return success if everything goes well
  }

  // Add else if blocks for other config types
  // Return an appropriate error if the config type is not recognized
  return Error::ConfigTypeNotRecognized;
}
