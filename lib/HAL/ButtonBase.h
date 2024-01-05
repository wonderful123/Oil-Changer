#pragma once

#include "IButton.h"
#include <chrono>

class ButtonBase : public IButton {
public:
  explicit ButtonBase(const HardwarePinConfig &config)
      : IButton(config), _isPressed(false), _wasPressed(false), _state(State::Released),
        _idString(config.id), _holdDurationThreshold(500),
        _longHoldDurationThreshold(1000) {}

  std::string stateToString(IButton::State state) const override {
    std::string states[] = {"Released", "Pressed", "Held", "HeldLong"};
    return states[static_cast<int>(state)];
  }

  virtual void updateButtonState() override {
    updateButton();

    if (_isPressed && !_wasPressed) {
      handleButtonPress();
      return;
    }

    if (_wasPressed) {
      if (!_isPressed) {
        handleButtonRelease();
        return;
      }

      auto currentTime = std::chrono::steady_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                          currentTime - _pressTime)
                          .count();

      if (duration > _longHoldDurationThreshold) {
        handleButtonHoldLong();
      } else if (duration > _holdDurationThreshold) {
        handleButtonHold();
      }
    }
  }

  void adjustLastPressTimeForTesting(std::chrono::milliseconds duration) {
    _pressTime += duration;
  }

  void handleButtonPress() {
    _pressTime = std::chrono::steady_clock::now();
    _state = State::Pressed;
    _wasPressed = true;
  }

  void handleButtonRelease() {
    _state = State::Released;
    _wasPressed = false;
  }

  void handleButtonHold() { _state = State::Held; }

  void handleButtonHoldLong() { _state = State::HeldLong; }

  IButton::State getState() const override { return _state; }
  const std::string &idString() const override { return _idString; }

  void setHoldDurationThreshold(int duration) override {
    _holdDurationThreshold = duration;
  }

  void setLongHoldDurationThreshold(int duration) override {
    _longHoldDurationThreshold = duration;
  }

protected:
  bool _isPressed;

private:
  std::string _idString;
  State _state;
  bool _wasPressed;

  std::chrono::steady_clock::time_point _pressTime;
  int _holdDurationThreshold;
  int _longHoldDurationThreshold;
};
