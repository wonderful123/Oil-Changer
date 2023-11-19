#include "DigitalIO.h"
#ifdef PLATFORM_ESP32
#include "Error.h"
#include <Arduino.h>

DigitalIO::DigitalIO(const GpioPinConfig &config) : IDigitalIO(config) {
  setPinModeFromConfig(config);
  setInitialized(true);
}

int DigitalIO::read() const { return digitalRead(_pinNumber); }

void DigitalIO::write(int value) { digitalWrite(_pinNumber, value); }

IDigitalIO::Mode DigitalIO::getMode() const { return _mode; }

void IDigitalIO::setPinModeFromConfig(const GpioPinConfig &config) const {
  _pinNumber = config.pinNumber;

  std::string mode = config.getOptionAs<std::string>("mode");

  if (mode == "INPUT") {
    _mode = IDigitalIO::INPUT_MODE;
    pinMode(_pinNumber, INPUT);
  } else if (mode == "OUTPUT") {
    _mode = IDigitalIO::OUTPUT_MODE;
    pinMode(_pinNumber, OUTPUT);
  } else {
    // Handle invalid mode
    Error(DigitalIOModeOptionMissingOrInvalid);
    _mode = IDigitalIO::INPUT_MODE; // Default to INPUT mode
    pinMode(_pinNumber, INPUT);
  }
}

#endif // PLATFORM_ESP32
