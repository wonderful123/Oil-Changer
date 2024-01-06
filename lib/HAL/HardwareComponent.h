// HardwareComponent.h
#pragma once

/**
 * @file HardwareComponent.h
 * @brief Base class for hardware components in the system.
 *
 * The HardwareComponent class serves as a base class for all specific hardware
 * component classes. It provides common functionality and properties that are
 * shared across different types of hardware components.
 */

#include "HardwarePinConfig.h"

/**
 * @class HardwareComponent
 * @brief Abstract base class for all hardware components.
 *
 * HardwareComponent provides a common interface and functionalities for
 * hardware components like ADC, PWM, DigitalIO, etc. It includes properties
 * such as pin number and initialization status. Specific hardware component
 * classes should inherit from this class to ensure consistent behavior and
 * interface.
 */
class HardwareComponent {
public:
  explicit HardwareComponent(const HardwarePinConfig &config)
      : _pinNumber(config.pinNumber), _id(config.id), _type(config.type),
        _initialized(false), _pins(config.pins) {}

  virtual ~HardwareComponent() = default;

  virtual bool isInitialized() const { return _initialized; }

  std::string id() const { return _id; }
  std::string type() const { return _type; }
  unsigned int pinNumber() const { return _pinNumber; }

  // Returns a map of pin labels to pin numbers for multi-pin components
  const std::unordered_map<std::string, int> &pins() const { return _pins; }

protected:
  void setInitialized(bool state) { _initialized = state; }

  unsigned int _pinNumber; // GPIO pin number (single-pin components)
  std::unordered_map<std::string, int>
      _pins; // Pin numbers for multi-pin components
  std::string _id;
  std::string _type;
  bool _initialized;
};
