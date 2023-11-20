#include "DigitalIO.h"
#ifdef PLATFORM_ESP32
#include "Error.h"
#include <Arduino.h>

explicit DigitalIO::DigitalIO(const GpioPinConfig &config)
    : DigitalIOBase(config) {
  pinMode(config.pinNumber, _mode);
}

int DigitalIO::read() const { return digitalRead(_pinNumber); }

void DigitalIO::write(int value) { digitalWrite(_pinNumber, value); }

#endif // PLATFORM_ESP32
