#pragma once

#include <IADC.h>
#include <ComponentConfig.h>

/**
 * @class ADC
 * @brief ESP32-specific implementation of the IADC interface.
 */
class ADC : public IADC {
private:
  int _pinNumber;           ///< Pin number for ADC.
  int _resolution;          ///< ADC resolution.
  ConfigMap _configOptions; ///< Configuration options.

public:
  /**
   * @brief Construct a new ADC object.
   *
   * @param pin       The pin number for ADC.
   * @param resolution ADC resolution. Default value is 12.
   * @param options   Configuration options for ADC. Default is an empty map.
   */
  ADC(int pin, int resolution = 12, const ConfigMap &options = {});

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
