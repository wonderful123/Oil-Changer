#include "DigitalIO.h"
#ifdef PLATFORM_ESP32
#include <Arduino.h>
#include "Logger.h"

DigitalIO::DigitalIO(const GpioPinConfig &config) : IDigitalIO(config) {
  std::string modeStr = config.options.at("mode");
  if (modeStr == "INPUT") {
    _mode = IDigitalIO::INPUT_MODE;
  } else if (modeStr == "OUTPUT") {
    _mode = IDigitalIO::OUTPUT_MODE;
  } else {
    Logger::error("Invalid mode in DigitalIO configuration.");
    setInitialized(false);
    return;
  }

  pinMode(_pinNumber,
          _mode == IDigitalIO::INPUT_MODE ? INPUT_MODE : OUTPUT_MODE);
  setInitialized(true);
}

int DigitalIO::read() const { return digitalRead(_pinNumber); }

void DigitalIO::write(int value) { digitalWrite(_pinNumber, value); }

IDigitalIO::Mode DigitalIO::getMode() const { return _mode; }

#endif // PLATFORM_ESP32
