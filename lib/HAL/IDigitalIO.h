#pragma once

#include "GpioPinConfig.h"

/**
 * @class IDigitalIO
 * @brief Interface for digital input/output operations.
 *
 * This interface provides the fundamental operations for digital I/O
 * operations, such as reading a digital input or setting a digital output.
 * Implementations of this interface should provide platform-specific details
 * for these operations, using the GpioPinConfig structure for configuration.
 */
class IDigitalIO {
protected:
  GpioPinConfig _config; ///< GPIO Pin Configuration for Digital I/O

public:
  /**
   * @brief Constructs the IDigitalIO object using GpioPinConfig.
   *
   * @param config Configuration for the digital I/O pin.
   */
  explicit IDigitalIO(const GpioPinConfig &config) : _config(config) {
    // Initialization with the GpioPinConfig structure.
    // Note: Setting the pin mode and other configurations should be done in
    // derived classes.
  }

  virtual ~IDigitalIO() = default;

  /**
   * @brief Read the value of the digital input pin.
   *
   * This method returns the value of the digital input pin.
   * It should return HIGH or LOW based on the voltage level.
   *
   * @return int Value of the digital input pin (HIGH/LOW).
   */
  virtual int read() const = 0;

  /**
   * @brief Set the value of the digital output pin.
   *
   * This method sets the value of the digital output pin
   * to HIGH or LOW based on the provided value.
   *
   * @param value Value to set the digital output pin (HIGH/LOW).
   */
  virtual void write(int value) = 0;

  /**
   * @brief Get the current pin mode.
   *
   * This method returns the current mode of the digital pin
   * (e.g., INPUT, OUTPUT), which is part of the GpioPinConfig.
   *
   * @return int Current pin mode.
   */
  virtual int getMode() const = 0;
};
