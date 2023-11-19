#include "ADC.h"
#ifdef PLATFORM_ESP32
#include "Error.h"
#include <Arduino.h>
#include <driver/adc.h>

ADC::ADC(const GpioPinConfig &config)
    : IADC(config), _resolution(12), _attenuation(ADC_0db) {
  // Set default values for resolution and attenuation
  // Default resolution for ESP32 is usually 12 bits
  // Default attenuation is 0 dB

  // Configure resolution
  _resolution = config.getOptionAs<int>("resolution");
  analogReadResolution(_resolution);

  // Configure attenuation
  int attenuationValue = config.getOptionAs<int>("attenuation");
  if (attenuationValue >= 0 && attenuationValue <= 3) {
    _attenuation = static_cast<adc_atten_t>(attenuationValue);
    adc1_config_channel_atten(static_cast<adc1_channel_t>(_pinNumber),
                              _attenuation);
  } else {
    // Handle invalid attenuation values
    Error(ADCInvalidAttenuationValue);
    setInitialized(false);
    return;
  }

  setInitialized(true); // Mark ADC as initialized
}

int ADC::read() const { return analogRead(_pinNumber); }

int ADC::resolution() const { return _resolution; }

#endif // PLATFORM_ESP32
