#pragma once

#include "IButton.h"
#include "IButtonControllerObserver.h"
#include "InteractionSettings.h"
#include <algorithm>
#include <chrono>
#include <memory>
#include <unordered_map>
#include <vector>

class ButtonController {
public:
  struct ButtonState {
    std::shared_ptr<IButton> button;
    bool isPressed;
    std::chrono::steady_clock::time_point lastPressTime;
    std::chrono::steady_clock::time_point lastRepeatTime;
    bool isInAutoRepeatMode;
  };

  virtual void addObserver(std::shared_ptr<IButtonControllerObserver> observer);
  virtual void registerButton(const std::string &id,
                              std::shared_ptr<IButton> button);
  virtual void processButtonStates();
  virtual void notifyObservers(const std::string &id);
  virtual std::shared_ptr<IButton> getButtonById(const std::string &id) const;

private:
  std::vector<std::shared_ptr<IButtonControllerObserver>> _observers;
  std::unordered_map<std::string, std::shared_ptr<IButton>> _buttons;
  std::unordered_map<std::string, ButtonState> _buttonStates;
  InteractionSettings _settings;

  void handleAutoRepeat(const std::string &id, ButtonState &state);
};
