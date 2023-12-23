#pragma once

#include <map>
#include <memory>

#include "ButtonController.h"
#include "BuzzerManager.h"
#include "ConfigManager.h"
#include "Error.h"
#include "HardwareFactory.h"
#include "HardwarePinConfig.h"
#include "InteractionSettingsConfig.h"
#include "Logger.h"
#include "Mediator/IMediator.h"

class HardwareInitializer {
 public:
  HardwareInitializer(std::shared_ptr<ConfigManager> configManager,
                      std::shared_ptr<HardwareFactory> hardwareFactory,
                      std::shared_ptr<ButtonController> buttonController,
                      std::shared_ptr<BuzzerManager> buzzerManager,
                      std::shared_ptr<IMediator> mediator);

  Error initialize(
      std::map<std::string, std::shared_ptr<HardwareComponent>> &components);

 private:
  std::shared_ptr<ConfigManager> _configManager;
  std::shared_ptr<HardwareFactory> _hardwareFactory;
  std::shared_ptr<ButtonController> _buttonController;
  std::shared_ptr<BuzzerManager> _buzzerManager;
  std::shared_ptr<IMediator> _mediator;

  Error initializeComponents(
      std::map<std::string, std::shared_ptr<HardwareComponent>> &components);
  Error initializeComponent(const HardwarePinConfig &config,
                            std::shared_ptr<HardwareComponent> &component);
  Error initializeBuzzerManager(std::shared_ptr<IBuzzer> buzzer);
};
