// IDigitalIO.h
#pragma once

#include "HardwareComponent.h"
#include "HardwarePinConfig.h"

/**
 * @class IDigitalIO
 * @brief Interface for digital input/output operations.
 *
 * This interface provides the fundamental operations for digital I/O
 * operations, such as reading a digital input or setting a digital output.
 * Implementations of this interface should provide platform-specific details
 * for these operations, using the HardwarePinConfig structure for
 * configuration.
 */
class IDigitalIO : public HardwareComponent {
public:
  IDigitalIO(const HardwarePinConfig &config) : HardwareComponent(config) {}
  virtual ~IDigitalIO() = default;

  enum class Mode { MODE_INPUT, MODE_OUTPUT, MODE_PULLUP, MODE_PULLDOWN };

  static const int DIGITAL_HIGH = 1;
  static const int DIGITAL_LOW = 0;

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
   * (e.g., INPUT, OUTPUT), which is part of the HardwarePinConfig.
   *
   * @return int Current pin mode.
   */
  virtual Mode getMode() const = 0;

  /**
   * @brief Set the pin mode.
   *
   * This method sets the pin mode to INPUT, OUTPUT, PULLUP, or PULLDOWN
   * depending on the provided value.
   *
   */
  virtual void setMode(Mode mode) = 0;

protected:
  Mode _mode;
};
