#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../Mediator/IColleague.h"
#include "ConfigTypes.h"
#include "HardwareConfig.h"

// Forward declarations
class IConfig;
class Error;
class InteractionSettingsConfig;
class IFileHandler;

class ConfigManager : public IColleague {
 public:
  ConfigManager(std::shared_ptr<IMediator> mediator,
                std::shared_ptr<IFileHandler> fileHandler);

  template <typename T>
  std::shared_ptr<T> getConfig(ConfigType type) {
    // Check if the configuration is already loaded
    if (!isConfigLoaded(type)) {
      Error loadError = loadConfig(type);
      if (loadError != Error::OK) {
        Logger::error("[ConfigManager] Failed to load configuration: " +
                      std::to_string(static_cast<int>(type)));
        return nullptr;
      }
    }

    // Attempt to cast the configuration to the requested type
    std::string name = ConfigPaths::getNameForType(type);
    auto config = std::static_pointer_cast<T>(_configs[name]);
    if (!config) {
      Logger::error("[ConfigManager] Configuration casting failed for type: " +
                    std::to_string(static_cast<int>(type)));
      return nullptr;  // or handle as appropriate
    }

    return config;
  }

  void releaseConfig(ConfigType type);

  void receiveEvent(EventType eventType, const EventData *eventData) override;

 private:
  std::shared_ptr<IFileHandler> _fileHandler;
  std::shared_ptr<IMediator> _mediator;
  std::unordered_map<std::string, std::shared_ptr<IConfig>> _configs;

  Error loadConfig(ConfigType type);
  bool isConfigLoaded(ConfigType type);
};
