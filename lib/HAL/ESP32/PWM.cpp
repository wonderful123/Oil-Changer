#include "PWM.h"

#ifdef PLATFORM_ESP32
#include <Arduino.h>

// Constants for ESP32 PWM
const int MAX_PWM_CHANNEL = 15; // ESP32 supports 16 channels from 0 to 15.
const int PWM_RESOLUTION = 8;   // 8-bit resolution. Adjust if needed.

PWM::PWM(const GpioPinConfig &config)
    : _pinNumber(config.pinNumber), _dutyCycle(0.0),
      _frequency(1000.0) { // Default frequency

  // Extract and handle frequency from config if available
  auto frequencyIter = config.options.find("frequency");
  if (frequencyIter != config.options.end()) {
    // Attempt to convert the string to double
    char *end;
    double freq = strtod(frequencyIter->second.c_str(), &end);

    if (end != frequencyIter->second.c_str()) {
      _frequency = freq; // Successful conversion
    }
  }

  // Set up the PWM with the provided frequency
  ledcSetup(MAX_PWM_CHANNEL, _frequency, PWM_RESOLUTION);
  ledcAttachPin(_pinNumber, MAX_PWM_CHANNEL);
}

void PWM::setDutyCycle(double dutyCycle) {
  if (dutyCycle < 0.0)
    dutyCycle = 0.0;
  if (dutyCycle > 1.0)
    dutyCycle = 1.0;

  _dutyCycle = dutyCycle;
  int pwmValue = static_cast<int>(_dutyCycle * (1 << PWM_RESOLUTION));
  ledcWrite(MAX_PWM_CHANNEL, pwmValue);
}

double PWM::getDutyCycle() const { return _dutyCycle; }

void PWM::setFrequency(double frequency) {
  _frequency = frequency;
  ledcSetup(MAX_PWM_CHANNEL, _frequency, PWM_RESOLUTION);
}

double PWM::getFrequency() const { return _frequency; }

#endif // PLATFORM_ESP32