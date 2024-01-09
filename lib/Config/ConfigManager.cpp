// ConfigManager.cpp
#include "ConfigManager.h"
#include "HardwareConfig.h"
#include "IFileHandler.h"
#include "InteractionSettingsConfig.h"
#include "MotorSettingsConfig.h"
#include <string>

ConfigManager::ConfigManager(std::shared_ptr<IFileHandler> fileHandler)
    : _fileHandler(fileHandler) {}

Error ConfigManager::loadConfig(ConfigType type) {
  std::shared_ptr<IConfig> config;
  std::string filePath =
      ConfigPaths::getPathForType(type); // Get the path using the enum
  std::string name = ConfigPaths::getNameForType(type);
  Logger::info("[ConfigManager] Loading " + name);

  if (_fileHandler == nullptr) {
    Logger::error("[ConfigManager] _fileHandler is null");
    return Error::FileHandlerIsNull;
  }

  // Determine the specific config object and event type based on the ConfigType
  // enum
  switch (type) {
  case ConfigType::HARDWARE:
    config = std::make_shared<HardwareConfig>(_fileHandler, filePath);
    break;
  case ConfigType::INTERACTION_SETTINGS:
    config =
        std::make_shared<InteractionSettingsConfig>(_fileHandler, filePath);
    break;
  case ConfigType::MOTOR:
    config = std::make_shared<MotorSettingsConfig>(_fileHandler, filePath);
    break;
  default:
    Logger::error("[ConfigManager] Unknown configuration type");
    return Error(Error::ConfigTypeNotRecognized);
  }

  Error error = config->load();
  if (error == Error::OK) {
    Logger::info("[ConfigManager] " + ConfigPaths::getNameForType(type) +
                 " parsed successfully");
    _configs[name] = config; // Store using the ConfigType enum
  }

  return error;
}

bool ConfigManager::isConfigLoaded(ConfigType type) const {
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
