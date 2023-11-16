#ifdef PLATFORM_ESP32
#include "DigitalIO.h"
#include <Arduino.h>

DigitalIO::DigitalIO(const GpioPinConfig &config) : _config(config) {
  // Set the pin mode based on the configuration
  pinMode(_config.pinNumber,
          _config.mode); // Assuming mode is part of GpioPinConfig

  // Additional configurations can be applied here, if needed, using
  // _config.options For example, pull-up/pull-down settings could be configured
  // based on options
}

int DigitalIO::read() const { return digitalRead(_config.pinNumber); }

void DigitalIO::write(int value) { digitalWrite(_config.pinNumber, value); }

int DigitalIO::getMode() const {
  // Assuming mode is part of GpioPinConfig and is stored as an integer
  return _config.mode;
}

#endif // PLATFORM_ESP32