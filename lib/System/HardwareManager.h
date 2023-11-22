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
#include "HardwareFactory.h"
#include "IADC.h"
#include "IBuzzer.h"
#include "IDigitalIO.h"
#include "IPWM.h"
#include <map>
#include <memory>
#include <tinyfsm.hpp>

/**
 * @file HardwareManager.h
 * @brief Manages all hardware-related interactions in the Oil Change Machine.
 *
 * The HardwareManager serves as a centralized hub for managing hardware
 * interactions. It abstracts the hardware layer from higher-level system logic,
 * providing a unified interface for hardware operations and ensuring a clean
 * separation of concerns.
 */
class HardwareManager : public IObserver {
private:
  std::shared_ptr<ConfigManager> _configManager;
  std::unique_ptr<HardwareFactory> _hardwareFactory;
  std::shared_ptr<ButtonController> _buttonController;
  std::unordered_map<std::string, int> _buttonIdToPinMap;

  using InitializerFunction = std::function<bool(const GpioPinConfig &)>;
  std::unordered_map<std::string, InitializerFunction> initializerMap;

  std::map<int, std::unique_ptr<IButton>> buttons;
  std::map<int, std::unique_ptr<IDAC>> dacs;
  std::map<int, std::unique_ptr<IADC>> adcs;
  std::map<int, std::unique_ptr<IDigitalIO>> digitalIOs;
  std::map<int, std::unique_ptr<IPWM>> pwms;
  std::unique_ptr<IBuzzer> buzzer;

  bool initializeADC(const GpioPinConfig &config);
  bool initializeDAC(const GpioPinConfig &config);
  bool initializeDigitalIO(const GpioPinConfig &config);
  bool initializeButton(const GpioPinConfig &config);
  bool initializePWM(const GpioPinConfig &config);
  bool initializeBuzzer(const GpioPinConfig &config);

  void handleButtonEvent(int buttonId);
  void changeStateBasedOnButton(int buttonId);
  std::string findButtonIdByPin(int pin);

public:
  HardwareManager(std::shared_ptr<ConfigManager> configManager,
                           std::unique_ptr<HardwareFactory> hardwareFactory,
                           std::shared_ptr<ButtonController> buttonController);

  virtual ~HardwareManager() = default;

  void setupInitializerMap();

  // Initializes all hardware components
  void initializeHardware();

  bool initializeComponent(const GpioPinConfig &config);
  bool isComponentInitialized(const std::string &componentType);

  // Manages the states of hardware components using TinyFSM
  void manageHardwareStates();

  // Notifies other components about hardware events (Observer pattern)
  void notifyEvent();

  // Receives commands and reports status to the SystemController
  void communicateWithSystemController();

  // Observer pattern implementation
  virtual void update() override;

  void onButtonEvent(int buttonId, bool pressed);
};
