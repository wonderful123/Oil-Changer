#pragma once

#include "Error.h"          // For error handling
#include "HardwareConfig.h" // Specific hardware configuration class
#include "IConfig.h"        // Interface for configuration classes
#include "IFileHandler.h"   // Interface for file handling
#include <memory>
#include <unordered_map>

class ConfigManager {
public:
  explicit ConfigManager(IFileHandler *fileHandler)
      : _fileHandler(fileHandler) {}

  // Method to get hardware configuration object
  virtual std::shared_ptr<HardwareConfig> getHardwareConfig() const;

  // Add methods to get other specific config types if necessary...

  // Method to load a configuration
  Error loadConfig(const std::string &configType);

private:
  IFileHandler *_fileHandler;
  std::unordered_map<std::string, std::shared_ptr<IConfig>> configs;
};
