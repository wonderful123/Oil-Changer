// ConfigManager.cpp

#include "ConfigManager.h"
#include "HardwareConfig.h"
#include "IFileHandler.h"
#include "InteractionSettingsConfig.h"
#include <string>

// Responsible for loading configuration data from a source (like a JSON file)
// and passing it to the appropriate managers

std::mutex ConfigManager::_mutex; // Define the static mutex
std::shared_ptr<ConfigManager> ConfigManager::_instance = nullptr;

ConfigManager::ConfigManager() {}

std::shared_ptr<ConfigManager> ConfigManager::getInstance() {
  std::lock_guard<std::mutex> lock(_mutex); // Ensure thread safety
  if (!_instance) {
    _instance = std::shared_ptr<ConfigManager>(
        new ConfigManager()); // Create the Singleton instance
  }
  return _instance;
}

void ConfigManager::initialize(std::shared_ptr<IFileHandler> fileHandler) {
  _fileHandler = fileHandler;
}

Error ConfigManager::loadConfig(ConfigType type) {
  std::shared_ptr<IConfig> config;
  std::string filePath =
      ConfigPaths::getPathForType(type); // Get the path using the enum

  Logger::info("[ConfigManager] Loading " + ConfigPaths::getNameForType(type));

  if (_fileHandler == nullptr) {
    Logger::error("[ConfigManager] _fileHandler is null");
    return Error::FileHandlerIsNull;
  }

  // Determine the specific config object and event type based on the ConfigType
  // enum
  switch (type) {
  case ConfigType::HARDWARE:
    config = std::make_shared<HardwareConfig>(_fileHandler);
    break;
  case ConfigType::INTERACTION_SETTINGS:
    config = std::make_shared<InteractionSettingsConfig>(_fileHandler);
    break;
  default:
    Logger::error("[ConfigManager] Unknown configuration type");
    return Error(Error::ConfigTypeNotRecognized);
  }

  Error error = config->load(filePath);
  if (error == Error::OK) {
    Logger::info("[ConfigManager] " + ConfigPaths::getNameForType(type) +
                 " parsed successfully");
    std::string name = ConfigPaths::getNameForType(type);
    _configs[name] = config; // Store using the ConfigType enum
  }

  return error;
}

bool ConfigManager::isConfigLoaded(ConfigType type) {
  std::string name = ConfigPaths::getNameForType(type);
  return _configs.find(name) != _configs.end();
}

void ConfigManager::releaseConfig(ConfigType type) {
  std::lock_guard<std::mutex> lock(_mutex);
  std::string name = ConfigPaths::getNameForType(type);
  if (_referenceCounts[name] > 0 && --_referenceCounts[name] == 0) {
    _configs.erase(name);
  }
}
