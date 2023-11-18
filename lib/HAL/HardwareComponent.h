/**
 * @file HardwareComponent.h
 * @brief Base class for hardware components in the system.
 *
 * The HardwareComponent class serves as a base class for all specific hardware
 * component classes. It provides common functionality and properties that are
 * shared across different types of hardware components.
 */

#pragma once

#include "GpioPinConfig.h"

/**
 * @class HardwareComponent
 * @brief Abstract base class for all hardware components.
 *
 * HardwareComponent defines common attributes and functionalities for hardware
 * components such as ADC, PWM, DigitalIO, etc. It includes basic properties
 * like pin number and initialization status. This class should be inherited by
 * all specific hardware component classes to ensure consistent behavior and
 * interface.
 */
class HardwareComponent {
public:
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

protected:
  /**
   * @brief Protected constructor for the HardwareComponent.
   *
   * Initializes the hardware component with the given GPIO pin configuration.
   * Marked explicit to prevent accidental conversions.
   *
   * @param config The GPIO pin configuration.
   */
  explicit HardwareComponent(const GpioPinConfig &config)
      : _pinNumber(config.pinNumber), _initialized(false) {}

  /**
   * @brief Sets the initialization state of the hardware component.
   *
   * @param state The initialization state to set (true or false).
   */
  void setInitialized(bool state) { _initialized = state; }

  unsigned int _pinNumber; ///< GPIO pin number associated with the component.
  bool _initialized; ///< Flag indicating whether the component is initialized.
};
