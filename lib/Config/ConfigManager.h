#pragma once

#include "../Mediator/IColleague.h"
#include <memory>
#include <string>
#include <unordered_map>

// Forward declarations
class IConfig;
class Error;
class HardwareConfig;
class InteractionSettingsConfig;
class IFileHandler;

class ConfigManager : public IColleague {
public:
  ConfigManager(std::shared_ptr<IMediator> mediator,
                std::shared_ptr<IFileHandler> fileHandler);

  // Method to get hardware configuration object
  virtual std::shared_ptr<HardwareConfig> getHardwareConfig() const;

  std::shared_ptr<InteractionSettingsConfig>
  getInteractionSettingsConfig() const;

  // Method to load a configuration
  Error loadConfig(const std::string &configType);

  void receiveEvent(EventType eventType, const EventData *eventData) override;

private:
  std::shared_ptr<IFileHandler> _fileHandler;
  std::unordered_map<std::string, std::shared_ptr<IConfig>> configs;
  std::shared_ptr<IMediator> _mediator;
};
