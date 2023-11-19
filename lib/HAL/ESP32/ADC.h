#pragma once

#include "GpioPinConfig.h"
#include "IADC.h"

/**
 * @class ADC
 * @brief ESP32-specific implementation of the IADC interface.
 */
class ADC : public IADC {
private:
  unsigned int _attenuation;

public:
  /**
   * @brief Construct a new ADC object.
   *
   * @param config [in] Configuration for the ADC pin.
   *
   * The `config` object must contain the following required parameters:
   *
   * - `pinNumber` [Required]: The pin number to use for the ADC.
   *
   * The `config` object can also contain the following optional parameters
   *
   * - `attenuation` [Optional-Default=1]: The attenuation level of the ADC.
   * - `resolution` [Optional-Default-12]: The resolution of the ADC in bits.
   */
  ADC(const GpioPinConfig &config);

  /**
   * @brief Read the ADC value.
   *
   * @return int ADC reading.
   */
  virtual int read() const override;

  /**
   * @brief Get the ADC resolution.
   *
   * @return int ADC resolution.
   */
  virtual int resolution() const override;
};
