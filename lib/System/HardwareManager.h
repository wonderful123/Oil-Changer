/*
HardwareManager Overview
Role and Purpose
The HardwareManager serves as the centralized hub for managing all
hardware-related interactions in the Oil Change Machine. Its primary role is to
abstract the hardware layer from the higher-level system logic, providing a
unified interface for hardware operations and ensuring a clean separation of
concerns.

Responsibilities
Hardware Initialization: Sets up and initializes all the hardware components,
including sensors, motors, and other peripherals. This involves configuring
hardware pins, initializing communication interfaces, and ensuring all hardware
is ready for operation. State Management: Utilizes TinyFSM for managing the
states of hardware components, overseeing state transitions based on system
events and inputs. Hardware Abstraction: Provides a consistent and simplified
interface for other system components to interact with the hardware, hiding the
complexities and specifics of hardware operations. Error Detection and Handling:
Monitors hardware for errors or malfunctions, reports these to the
SystemController, and manages error states and recovery procedures. Event-Driven
Notifications: As part of the Observer pattern, HardwareManager notifies other
components about significant hardware events or changes in state. Interaction
with Other Components SystemController: Receives commands from and reports
status to the SystemController, ensuring coordinated system operations.
SensorManager: Works closely with the SensorManager for sensor data acquisition
and processing. MotorController: Interfaces with the MotorController to manage
motor operations in response to system commands and sensor inputs. Integration
with Design Patterns Observer Pattern: As an integral part of the Observer
pattern, HardwareManager both observes changes from components like
SensorManager and is observed by SystemController for state changes or critical
hardware events. TinyFSM: Utilizes TinyFSM for robust and predictable state
management of hardware components.
*/

#pragma once

#include "ButtonController.h"
#include "ConfigManager.h"
#include "Core/IObserver.h"
#include "GpioPinConfig.h"
#include "HardwareComponent.h" // Include the base class for hardware components
#include "HardwareFactory.h"
#include <functional>
#include <map>
#include <memory>
#include <tinyfsm.hpp>
#include <unordered_map>

class HardwareManager : public IObserver {
private:
  std::shared_ptr<ConfigManager> _configManager;
  std::unique_ptr<HardwareFactory> _hardwareFactory;
  std::shared_ptr<ButtonController> _buttonController;

  // Unified map to hold all types of components by id
  std::map<std::string, std::shared_ptr<HardwareComponent>> _components;

  void registerComponent(const GpioPinConfig &config,
                         const std::shared_ptr<HardwareComponent> &component);

      void handleButtonEvent(const std::string &buttonId);
  void changeStateBasedOnButton(const std::string &buttonId);

public:
  HardwareManager(std::shared_ptr<ConfigManager> configManager,
                  std::unique_ptr<HardwareFactory> hardwareFactory,
                  std::shared_ptr<ButtonController> buttonController);

  virtual ~HardwareManager() = default;

  void setupInitializerMap();

  // Initializes all hardware components
  void initializeHardware();

  bool initializeComponent(const GpioPinConfig &config);
  bool isComponentInitialized(const std::string &componentId) const;
  std::shared_ptr<HardwareComponent> getComponentById(const std::string &id) const;

  // Manages the states of hardware components using TinyFSM
  void manageHardwareStates();

  // Notifies other components about hardware events (Observer pattern)
  void notifyEvent();

  // Receives commands and reports status to the SystemController
  void communicateWithSystemController();

  // Observer pattern implementation
  virtual void update() override;

  void onButtonEvent(const std::string &buttonId, bool pressed);

  void triggerBuzzer();
};
