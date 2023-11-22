/**
 * @file HardwareComponent.h
 * @brief Base class for hardware components in the system.
 *
 * The HardwareComponent class serves as a base class for all specific hardware
 * component classes. It provides common functionality and properties that are
 * shared across different types of hardware components.
 */

#pragma once

#include "Error.h"
#include "GpioPinConfig.h"

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
  /**
   * @brief Protected constructor for the HardwareComponent.
   *
   * Initializes the hardware component with the given GPIO pin configuration.
   * Marked explicit to prevent accidental conversions.
   *
   * @param config The GPIO pin configuration.
   */
  explicit HardwareComponent(const GpioPinConfig &config)
      : _pinNumber(config.pinNumber), _id(config.id), _type(config.type),
        _initialized(false) {}

  /**
   * @brief Virtual destructor.
   *
   * Ensures derived classes are destructed properly.
   */
  virtual ~HardwareComponent() = default;

  /**
   * @brief Checks if the hardware component is initialized.
   *
   * @return true if the component is initialized, false otherwise.
   */
  virtual bool isInitialized() const { return _initialized; }

  // Getters
  std::string id() const { return _id; }

  std::string type() const { return _type; }

  unsigned int pinNumber() const { return _pinNumber; }

protected:
  /**
   * @brief Sets the initialization state of the hardware component.
   *
   * @param state The initialization state to set (true or false).
   */
  void setInitialized(bool state) { _initialized = state; }

  unsigned int _pinNumber; // GPIO pin number associated with the component.
  std::string _id;         // Unique identifier for the component.
  std::string _type;       // Type of the component.
  bool _initialized; // Flag indicating whether the component is initialized.
};
