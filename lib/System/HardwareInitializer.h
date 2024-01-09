// HardwareInitializer.h
#pragma once

#include "ConfigManager.h"
#include "Error.h"
#include "HardwareComponent.h"
#include "HardwareFactory.h"
#include <map>
#include <memory>

class HardwareInitializer {
public:
  HardwareInitializer(std::shared_ptr<ConfigManager> configManager,
                      std::shared_ptr<HardwareFactory> hardwareFactory);

  Error initializeHardware();
  std::map<std::string, std::shared_ptr<HardwareComponent>>
  getHardwareComponents() const;
  std::map<std::string, std::shared_ptr<IDisplay>>
  getDisplays() const;

private:
  std::map<std::string, std::shared_ptr<HardwareComponent>> _hardware;
  std::map<std::string, std::shared_ptr<IDisplay>> _displays;
  std::shared_ptr<ConfigManager> _configManager;
  std::shared_ptr<HardwareFactory> _hardwareFactory;

  Error initializeComponent(const HardwarePinConfig &config);
  Error initializeDisplay(const DisplayConfig &config);
  std::shared_ptr<HardwareComponent>
  getHardwareComponentById(const std::string &id) const;
  Error logAndReturnError(const std::string &message, const std::string &id,
                         Error error);
};
