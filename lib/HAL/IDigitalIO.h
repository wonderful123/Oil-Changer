#pragma once

#include <ComponentConfig.h>

/**
 * @class IDigitalIO
 * @brief Interface for digital input/output operations.
 *
 * This interface provides the fundamental operations for digital
 * I/O operations such as reading a digital input or setting a digital
 * output. Implementations of this interface should provide platform-specific
 * details for these operations.
 */
class IDigitalIO {
protected:
  int _pinNumber;           ///< Pin associated with this digital IO
  int _mode;                ///< Pin mode (e.g., INPUT, OUTPUT).
  ConfigMap _configOptions; ///< Configuration options for the pin

public:
  /**
   * @brief Constructs the IDigitalIO object.
   *
   * @param pin The pin number to use for digital IO operations.
   * @param mode Pin mode (e.g., INPUT, OUTPUT).
   * @param options Configurations options for the pin. Optional.
   */
  IDigitalIO(int pin, int mode, const ConfigMap &options = {})
      : _pinNumber(pin), _mode(mode), _configOptions(options) {
    // Note: Abstract base class should not set pin mode. This should be done in
    // the derived classes.
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
   * (e.g., INPUT, OUTPUT).
   *
   * @return int Current pin mode.
   */
  virtual int getMode() const = 0;
};
