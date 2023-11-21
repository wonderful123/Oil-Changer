#include "ButtonESP32.h"
#ifdef PLATFORM_ESP32
#include "Logger.h"

ButtonESP32::ButtonESP32(const GpioPinConfig &config) : ButtonBase(config) {
  pinMode(_pinNumber, INPUT);
  Logger::info("Button initialized on pin: " + std::to_string(_pinNumber));
  setInitialized(true); // Mark the Button as initialized
}

void ButtonESP32::updatePressedState() {
  _isPressed = digitalRead(_pinNumber) == HIGH;
  Logger::debug("Button updatePressedState: Pin " + std::to_string(_pinNumber) +
                ", Pressed " + std::to_string(_isPressed));
}
#endif // PLATFORM_ESP32
