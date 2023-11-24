#pragma once

#include "ADCBase.h"
#ifdef PLATFORM_ESP32
#include <Arduino.h>
#include <driver/adc.h>

class ESP32ADC : public ADCBase {
private:
  adc_atten_t _attenuation;

public:
  /**
   * @brief Construct a new ESP32ADC object.
   *
   * @param config Configuration for the ADC pin.
   */
  ESP32ADC(const HardwarePinConfig &config);

  virtual int read() const override;

private:
  /**
   * @brief Configures the attenuation for the ADC.
   *
   * @param config Configuration for the ADC pin.
   */
  void configureAttenuation(const HardwarePinConfig &config);
};

#endif // PLATFORM_ESP32
