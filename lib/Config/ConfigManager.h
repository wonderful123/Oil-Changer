#pragma once

#include "Error.h"          // For error handling
#include "HardwareConfig.h" // Specific hardware configuration class
#include "IFileHandler.h"   // Interface for file handling
#include "InteractionSettingsConfig.h"
#include "ISubject.h"  // Interface for the Subject part of the Observer pattern
#include <algorithm>
#include <memory>
#include <unordered_map>

class ConfigManager : public ISubject {
public:
  explicit ConfigManager(IFileHandler *fileHandler)
      : _fileHandler(fileHandler) {}

  // Method to get hardware configuration object
  virtual std::shared_ptr<HardwareConfig> getHardwareConfig() const;

  std::shared_ptr<InteractionSettingsConfig>
  getInteractionSettingsConfig() const;

  // Method to load a configuration
  Error loadConfig(const std::string &configType);

  void notify(EventType eventType) {
    for (auto &observer : observers) {
      observer->update(eventType);
    }
  }

  void attach(IObserver *observer) override { observers.push_back(observer); }

  void detach(IObserver *observer) override {
    observers.erase(std::remove(observers.begin(), observers.end(), observer),
                    observers.end());
  }

private:
  IFileHandler *_fileHandler;
  std::unordered_map<std::string, std::shared_ptr<IConfig>> configs;
  std::vector<IObserver *> observers;
};
