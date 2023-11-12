#ifdef PLATFORM_ESP32
#include "DigitalIO.h"
#include <Arduino.h>

DigitalIO::DigitalIO(int pin, int mode, const ConfigMap &options)
    : IDigitalIO(pin, mode, options) {
  pinMode(_pinNumber, _mode);

  // If there are ESP32-specific configurations you'd like to apply,
  // handle them here using the _configOptions map.
}

int DigitalIO::read() const { return digitalRead(_pinNumber); }

void DigitalIO::write(int value) { digitalWrite(_pinNumber, value); }

int DigitalIO::getMode() const { return _mode; }

#endif // PLATFORM_ESP32