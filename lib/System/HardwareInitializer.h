#pragma once

#include <map>
#include <memory>

#include "ConfigManager.h"
#include "Error.h"
#include "HardwareFactory.h"
#include "HardwarePinConfig.h"
#include "InteractionSettingsConfig.h"
#include "Logger.h"

class HardwareInitializer {
 public:
  HardwareInitializer(std::shared_ptr<ConfigManager> configManager,
                      std::shared_ptr<HardwareFactory> hardwareFactory);

  Error initialize(
      std::map<std::string, std::shared_ptr<HardwareComponent>> &components);

 private:
  std::shared_ptr<ConfigManager> _configManager;
  std::shared_ptr<HardwareFactory> _hardwareFactory;
  std::shared_ptr<IMediator> _mediator;

  Error initializeComponent(const HardwarePinConfig &config,
                            std::shared_ptr<HardwareComponent> &component);
};
