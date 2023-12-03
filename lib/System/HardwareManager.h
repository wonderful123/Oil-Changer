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

#include "ButtonController.h"
#include "ConfigManager.h"
#include "FSM/Events.h"
#include "FSM/StateMachine.h"
#include "FSM/States.h"
#include "HardwareComponent.h"
#include "HardwareFactory.h"
#include "HardwarePinConfig.h"
#include "IBuzzer.h"
#include "IObserver.h"
#include "Observer/EventTypes.h"
#include <functional>
#include <map>
#include <memory>
#include <tinyfsm.hpp>
#include <unordered_map>

class HardwareManager : public IObserver {
private:
  std::shared_ptr<ConfigManager> _configManager;
  std::shared_ptr<HardwareFactory> _hardwareFactory;
  std::shared_ptr<ButtonController> _buttonController;

  // Unified map to hold all types of components by id
  std::map<std::string, std::shared_ptr<HardwareComponent>> _components;

  virtual bool initializeComponent(const HardwarePinConfig &config);

  void registerComponent(const HardwarePinConfig &config,
                         const std::shared_ptr<HardwareComponent> &component);

  virtual void onButtonEvent(const std::string &buttonId, bool pressed);

  void handleButtonEvent(const std::string &buttonId);

  void changeStateBasedOnButton(const std::string &buttonId);

public:
  HardwareManager(std::shared_ptr<ConfigManager> configManager,
                  std::shared_ptr<HardwareFactory> hardwareFactory,
                  std::shared_ptr<ButtonController> buttonController);

  virtual ~HardwareManager() {
    _configManager->detach(this); // Detach when destroyed
  }

  // Initializes all hardware components
  virtual void initializeHardware();

  virtual bool isComponentInitialized(const std::string &componentId) const;

  virtual std::shared_ptr<HardwareComponent>
  getComponentById(const std::string &id) const;

  // Observer pattern implementation
  virtual void update(EventType eventType);

  virtual void triggerBuzzer();
  void updateBuzzerSettings();
};
