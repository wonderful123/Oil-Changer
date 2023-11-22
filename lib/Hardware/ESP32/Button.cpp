#include "Button.h"

#ifdef PLATFORM_ESP32
#include "Logger.h"

std::unordered_map<int, Button *> Button::_instanceMap;

Button::Button(const GpioPinConfig &config,
                         ButtonController &controller)
    : ButtonBase(config), _controller(controller) {
  pinMode(_pinNumber, INPUT_PULLUP);
  _debouncer.attach(_pinNumber);
  _debouncer.interval(50); // Set debounce interval

  _instanceMap[_pinNumber] = this;
  attachInterrupt(digitalPinToInterrupt(_pinNumber),
                  Button::handleInterrupt, CHANGE);
}

void Button::updatePressedState() {
  if (_debouncer.rose()) {
    _controller.notifyObservers(_pinNumber);
  }
}

void Button::handleInterrupt() {
  for (auto &kv : _instanceMap) {
    kv.second->_debouncer.update();
  }
}

#endif // PLATFORM_ESP32
