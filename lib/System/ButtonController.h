#pragma once

#include "IButton.h"
#include "IButtonControllerObserver.h"
#include <memory>
#include <unordered_map>
#include <vector>

class ButtonController {
public:
  virtual void addObserver(std::shared_ptr<IButtonControllerObserver> observer);

  virtual void registerButton(const std::string &id,
                             std::shared_ptr<IButton> button);

  virtual void notifyObservers(const std::string &id);

  virtual void checkButtonStates();

  static void buttonCallback(const std::string &id);

private:
  std::vector<std::shared_ptr<IButtonControllerObserver>> _observers;
  std::unordered_map<std::string, std::shared_ptr<IButton>> _buttons;
  static std::unordered_map<std::string, ButtonController *> _controllers;
};
