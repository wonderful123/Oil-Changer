#include "PWM.h"
#include "Logger.h"

#ifdef PLATFORM_ESP32
#include <Arduino.h>

PWM::PWM(const GpioPinConfig &config) : PWMBase(config) {
  initializePWM();
  setInitialized(true);
  Logger::info("PWM initialized on pin: " + std::to_string(config.pinNumber) +
               " with frequency: " + std::to_string(getFrequency()) +
               " Hz and duty cycle: " + std::to_string(getDutyCycle() * 100) +
               "%");
}

void PWM::applyDutyCycle(double dutyCycle) {
  int pwmValue = static_cast<int>(dutyCycle * (1 << PWM_RESOLUTION));
  ledcWrite(MAX_PWM_CHANNEL, pwmValue);
  Logger::info("PWM duty cycle set to: " + std::to_string(dutyCycle * 100) +
               "%");
}

void PWM::applyFrequency(double frequency) {
  ledcSetup(MAX_PWM_CHANNEL, frequency, PWM_RESOLUTION);
  Logger::info("PWM frequency set to: " + std::to_string(frequency) + " Hz");
}

void PWM::initializePWM() {
  ledcSetup(MAX_PWM_CHANNEL, getFrequency(), PWM_RESOLUTION);
  ledcAttachPin(_pinNumber, MAX_PWM_CHANNEL);
  applyDutyCycle(getDutyCycle());
}

#endif // PLATFORM_ESP32
