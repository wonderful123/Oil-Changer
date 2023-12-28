#include "ConfigManager.h"

#include <string>

#include "BaseConfig.h"
#include "HardwareConfig.h"
#include "IFileHandler.h"
#include "InteractionSettingsConfig.h"
// Responsible for loading configuration data from a source (like a JSON file)
// and passing it to the appropriate managers

ConfigManager::ConfigManager(std::shared_ptr<IMediator> mediator,
                             std::shared_ptr<IFileHandler> fileHandler)
    : IColleague(mediator), _mediator(mediator), _fileHandler(fileHandler) {}

Error ConfigManager::loadConfig(ConfigType type) {
  std::shared_ptr<IConfig> config;
  std::string filePath =
      ConfigPaths::getPathForType(type); // Get the path using the enum
  EventType eventType;

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
    eventType = EventType::HARDWARE_CONFIG_CHANGED;
    break;
  case ConfigType::INTERACTION_SETTINGS:
    config = std::make_shared<InteractionSettingsConfig>(_fileHandler);
    eventType = EventType::INTERACTION_SETTINGS_CHANGED;
    break;
  default:
    Logger::error("[ConfigManager] Unknown configuration type");
    return Error(Error::ConfigTypeNotRecognized);
  }

  Error error = config->load(filePath);
  if (error == Error::OK) {
    Logger::info("[ConfigManager] " +
                 ConfigPaths::getNameForType(type) + " parsed successfully");
    std::string name = ConfigPaths::getNameForType(type);
    _configs[name] = config; // Store using the ConfigType enum
    _mediator->notify(this, eventType);
  }

  return error;
}

bool ConfigManager::isConfigLoaded(ConfigType type) {
  std::string name = ConfigPaths::getNameForType(type);
  return _configs.find(name) != _configs.end();
}

void ConfigManager::releaseConfig(ConfigType type) {
  std::string name = ConfigPaths::getNameForType(type);
  _configs.erase(name);
}

void ConfigManager::receiveEvent(EventType eventType,
                                 const EventData *eventData) {
  // Implementation goes here
}