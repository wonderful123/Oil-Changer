#include "ADC.h"

#ifdef PLATFORM_ESP32
#include <Arduino.h>

ADC::ADC(int pin, int resolution, const ConfigMap &options)
    : _pinNumber(pin), _resolution(resolution), _configOptions(options) {

  if (_configOptions.find("attenuation") != _configOptions.end()) {
    int attenuationValue = std::stoi(_configOptions["attenuation"]);
    // Set the ADC attenuation using attenuationValue here.
    // This is platform-specific and may require additional logic or a function.
  }
}

int ADC::read() {
  // ESP32-specific logic for reading ADC value.
  return analogRead(_pinNumber);
}

int ADC::resolution() const { return _resolution; }

#endif // PLATFORM_ESP32
