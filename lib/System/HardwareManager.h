/*
HardwareManager Overview
Role and Purpose
The HardwareManager serves as the centralized hub for managing all
hardware-related interactions in the Oil Change Machine. Its primary role is to
abstract the hardware layer from the higher-level system logic, providing a
unified interface for hardware operations and ensuring a clean separation of
concerns.

*/

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <tinyfsm.hpp>
#include <unordered_map>

#include "Mediator/IColleague.h"

class ButtonController;
class ConfigManager;
class HardwareComponent;
class HardwareFactory;
class HardwarePinConfig;

class HardwareManager : public IColleague {
 private:
  std::shared_ptr<ConfigManager> _configManager;
  std::shared_ptr<HardwareFactory> _hardwareFactory;
  std::shared_ptr<ButtonController> _buttonController;
  std::shared_ptr<IMediator> _mediator;
  // Unified map to hold all types of components by id
  std::map<std::string, std::shared_ptr<HardwareComponent>> _components;

  virtual bool initializeComponent(const HardwarePinConfig &config);
  void notifyMediator(EventType eventType);
  void changeStateBasedOnButton(const std::string &buttonId);

 public:
  HardwareManager(std::shared_ptr<IMediator> mediator,
                  std::shared_ptr<ConfigManager> configManager,
                  std::shared_ptr<HardwareFactory> hardwareFactory,
                  std::shared_ptr<ButtonController> buttonController);

  virtual void initializeHardware();
  virtual bool isComponentInitialized(const std::string &componentId) const;
  virtual std::shared_ptr<HardwareComponent> getComponentById(
      const std::string &id) const;
  virtual void triggerBuzzer();
  void updateBuzzerSettings();

  void receiveEvent(EventType eventType, const EventData *eventData) override;
};
