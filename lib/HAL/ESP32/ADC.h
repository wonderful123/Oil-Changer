#pragma once

#include "GpioPinConfig.h"
#include "IADC.h"

/**
 * @class ADC
 * @brief ESP32-specific implementation of the IADC interface.
 */
class ADC : public IADC {
private:
  GpioPinConfig _config; ///< GPIO Pin Configuration for ADC.

public:
  /**
   * @brief Construct a new ADC object.
   *
   * @param config Configuration for the ADC pin.
   */
  ADC(const GpioPinConfig &config) : _config(config) {
    // Initialize ADC with values from _config.
    // You can access resolution and other options from _config.options.
    // Example: _resolution = std::stoi(_config.options.at("resolution"));
  }

  /**
   * @brief Read the ADC value.
   *
   * @return int ADC reading.
   */
  virtual int read() = 0;

  /**
   * @brief Get the ADC resolution.
   *
   * @return int ADC resolution.
   */
  virtual int resolution() const = 0;
};
