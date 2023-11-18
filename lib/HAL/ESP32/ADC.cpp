#include "ADC.h"
#ifdef PLATFORM_ESP32
#include <Arduino.h>

ADC::ADC(const GpioPinConfig &config)
    : IADC(config), _resolution(12), _attenuation(ADC_0db) {
  // Set default values for resolution and attenuation
  // Default resolution for ESP32 is usually 12 bits
  // Default attenuation is 0 dB

  // Configure resolution
  auto resIt = config.options.find("resolution");
  if (resIt != config.options.end()) {
    _resolution = std::stoi(resIt->second);
    analogReadResolution(_resolution);
  }

  // Configure attenuation
  auto attIt = config.options.find("attenuation");
  if (attIt != config.options.end()) {
    _attenuation = std::stoi(attIt->second);

    if (_attenuation >= 0 && _attenuation <= 3) {
      // Configure ADC attenuation
      adc1_config_channel_atten(static_cast<adc1_channel_t>(_pinNumber),
                                static_cast<adc_atten_t>(_attenuation));
    } else {
      // Handle invalid attenuation values
      Logger::error("Invalid ADC attenuation value.");
      setInitialized(false);
      
      return;
    }
  }

  setInitialized(true); // Mark ADC as initialized
}

int ADC::read() const {
  if (!isInitialized()) {
    Logger::error("Attempted to read uninitialized ADC.");
    return -1; // or some error code
  }
  return analogRead(_pinNumber);
}

int ADC::resolution() const { return _resolution; }

#endif // PLATFORM_ESP32
