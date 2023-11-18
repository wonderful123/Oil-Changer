#include "DigitalIO.h"
#ifdef PLATFORM_ESP32
#include <Arduino.h>

DigitalIO::DigitalIO(const GpioPinConfig &config) : IDigitalIO(config) {
  pinMode(_pinNumber, static_cast<uint8_t>(config.mode));
  setInitialized(true); // Mark as initialized
}

int DigitalIO::read() const { return digitalRead(_pinNumber); }

void DigitalIO::write(int value) { digitalWrite(_pinNumber, value); }

IDigitalIO::Mode DigitalIO::getMode() const { return _mode; }

#endif // PLATFORM_ESP32
