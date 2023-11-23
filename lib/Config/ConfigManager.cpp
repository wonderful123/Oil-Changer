#include "ConfigManager.h"
#include "Error.h"
#include "HardwareConfig.h"
#include <memory>

std::shared_ptr<HardwareConfig> ConfigManager::getHardwareConfig() const {
  auto it = configs.find("HardwareConfig");
  if (it != configs.end()) {
    return std::static_pointer_cast<HardwareConfig>(it->second);
  }
  return nullptr; // Return nullptr to indicate config not found
}

Error ConfigManager::loadConfig(const std::string &configType) {
  if (configType == "HardwareConfig") {
    auto hardwareConfig = std::make_shared<HardwareConfig>(_fileHandler);
    Error loadError = hardwareConfig->load("/config/hardwareConfig.json");
    if (loadError) {
      return loadError; // Propagate the error
    }
    configs[configType] = hardwareConfig;
    return Error(Error::OK); // Return success if everything goes well
  }

  // Add else if blocks for other config types
  // Return an appropriate error if the config type is not recognized
  return Error(Error::ConfigTypeNotRecognized);
}
