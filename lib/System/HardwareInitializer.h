#pragma once

#include <map>
#include <memory>

#include "Error.h"
#include "Logger.h"

class HardwarePinConfig;
class ConfigManager;
class HardwareFactory;
class HardwareComponent;

class HardwareInitializer {
 public:
  HardwareInitializer(std::shared_ptr<ConfigManager> configManager,
                      std::shared_ptr<HardwareFactory> hardwareFactory);

  Error initialize(
      std::map<std::string, std::shared_ptr<HardwareComponent>> &components);

 private:
  std::shared_ptr<ConfigManager> _configManager;
  std::shared_ptr<HardwareFactory> _hardwareFactory;

  Error initializeComponent(const HardwarePinConfig &config,
                            std::shared_ptr<HardwareComponent> &component);
};
