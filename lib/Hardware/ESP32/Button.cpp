#include "Button.h"

#ifdef PLATFORM_ESP32
#include "Logger.h"

std::unordered_map<int, Button *> Button::_instanceMap;

Button::Button(const GpioPinConfig &config) : ButtonBase(config) {
  pinMode(_pinNumber, INPUT_PULLUP);
  _debouncer.attach(_pinNumber);
  _debouncer.interval(50); // Set debounce interval
  _instanceMap[_pinNumber] = this;
  attachInterrupt(digitalPinToInterrupt(_pinNumber), Button::handleInterrupt,
                  CHANGE);
  std::string logMessage = "Button: " + id() + " initialized.";
  Logger::info(logMessage);
}

void Button::updatePressedState() {
  if (_debouncer.rose()) {
    if (_onPressCallback) {
      _onPressCallback(_id);
    }
  }
}

void IRAM_ATTR Button::handleInterrupt() {
  std::string logMessage = "button interrupt";
  Logger::info(logMessage);
  for (auto &kv : _instanceMap) {
    kv.second->_debouncer.update();
  }
}

#endif // PLATFORM_ESP32
