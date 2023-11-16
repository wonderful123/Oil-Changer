#include "ADC.h"

#ifdef PLATFORM_ESP32
#include <Arduino.h>

ADC::ADC(const GpioPinConfig &config) : _config(config) {
  // Extract the pin number
  _pinNumber = _config.pinNumber;

  // Set default values
  _resolution = 12;       // Default resolution for ESP32 is usually 12 bits
  _attenuation = ADC_0db; // Default attenuation (0 dB)

  // Configure resolution
  if (_config.options.find("resolution") != _config.options.end()) {
    _resolution = std::stoi(_config.options.at("resolution"));
    analogReadResolution(_resolution);
  }

  // Configure attenuation
  if (_config.options.find("attenuation") != _config.options.end()) {
    _attenuation = std::stoi(_config.options.at("attenuation"));

    if (_attenuation >= 0 && _attenuation <= 3) {
      // Assuming the attenuation value in config is from 0 to 3
      // corresponding to 0 dB to 11 dB in ESP32
      adc1_config_channel_atten((adc1_channel_t)_pinNumber,
                                (adc_atten_t)_attenuation);
    }
    // Handle invalid attenuation values appropriately
  }
}

int ADC::read() const {
  // Read ADC value from the specified pin
  return analogRead(_pinNumber);
}

int ADC::resolution() const { return _resolution; }

#endif // PLATFORM_ESP32