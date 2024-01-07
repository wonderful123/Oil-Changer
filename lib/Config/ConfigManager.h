#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "ConfigTypes.h"
#include "Error.h"

// Forward declarations
class IConfig;
class IFileHandler;

class ConfigManager {
public:
  ConfigManager(std::shared_ptr<IFileHandler> fileHandler);

  template <typename T> std::shared_ptr<T> getConfig(ConfigType type) {
    std::lock_guard<std::mutex> lock(_mutex); // Ensure thread safety
    if (!isConfigLoaded(type)) {
      Error loadError = loadConfig(type);
      if (loadError != Error::OK) {
        Logger::error("[ConfigManager] Failed to load configuration: " +
                      std::to_string(static_cast<int>(type)));
        return nullptr;
      }
    }

    std::string name = ConfigPaths::getNameForType(type);
    auto config = std::static_pointer_cast<T>(_configs[name]);
    if (config) {
      _referenceCounts[name]++;
      return config;
    } else {
      Logger::error("[ConfigManager] Configuration casting failed for type: " +
                    std::to_string(static_cast<int>(type)));
      return nullptr;
    }
  }

  void releaseConfig(ConfigType type);

private:
  std::shared_ptr<IFileHandler> _fileHandler;
  std::unordered_map<std::string, std::shared_ptr<IConfig>> _configs;
  std::unordered_map<std::string, int>
      _referenceCounts;      // Reference counting for each config
  mutable std::mutex _mutex; // Mutex for thread safety

  Error loadConfig(ConfigType type);
  bool isConfigLoaded(ConfigType type) const;
};
