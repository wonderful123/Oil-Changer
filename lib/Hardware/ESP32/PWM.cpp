#include "PWM.h"
#ifdef PLATFORM_ESP32
#include <Arduino.h>

// Constants for ESP32 PWM
const int MAX_PWM_CHANNEL = 15; // ESP32 supports 16 channels from 0 to 15.
const int PWM_RESOLUTION = 8;   // 8-bit resolution. Adjust if needed.

PWM::PWM(const GpioPinConfig &config)
    : IPWM(config), _dutyCycle(0.0),
      _frequency(1000.0) { // Default frequency set

  // Extract frequency from config if available
  _frequency = config.getOptionAs<double>("frequency"); // Default to 1000.0 if not found

  // Set up the PWM with the provided frequency
  ledcSetup(MAX_PWM_CHANNEL, _frequency, PWM_RESOLUTION);
  ledcAttachPin(_pinNumber, MAX_PWM_CHANNEL);

  // Mark as initialized after successful setup
  setInitialized(true);
}

void PWM::setDutyCycle(double dutyCycle) {
  _dutyCycle =
      std::max(0.0, std::min(dutyCycle, 1.0)); // Clamp between 0.0 and 1.0

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
