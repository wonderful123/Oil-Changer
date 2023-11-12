#pragma once

#include <IDigitalIO.h>

/**
 * @class DigitalIO
 * @brief ESP32-specific implementation of the IDigitalIO interface.
 *
 * This class provides the operations for digital I/O tailored for the ESP32
 * platform.
 */
class DigitalIO : public IDigitalIO {
public:
  /**
   * @brief Construct a new DigitalIO object for ESP32.
   *
   * @param pin The pin number for digital I/O operations.
   * @param mode Pin mode (e.g., INPUT, OUTPUT).
   * @param options Configuration options for the pin. Optional.
   */
  explicit DigitalIO(int pin, int mode, const ConfigMap &options = {});

  /**
   * @brief Read the value of the digital input pin.
   *
   * This method returns the value of the digital input pin.
   * It should return HIGH or LOW based on the voltage level.
   *
   * @return int Value of the digital input pin (HIGH/LOW).
   */
  int read() const override;

  /**
   * @brief Set the value of the digital output pin.
   *
   * This method sets the value of the digital output pin
   * to HIGH or LOW based on the provided value.
   *
   * @param value Value to set the digital output pin (HIGH/LOW).
   */
  void write(int value) override;

  /**
   * @brief Get the current pin mode.
   *
   * This method returns the current mode of the digital pin
   * (e.g., INPUT, OUTPUT).
   *
   * @return int Current pin mode.
   */
  int getMode() const override;
};
