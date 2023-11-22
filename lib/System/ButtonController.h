#pragma once

#include "IButton.h"
#include "IButtonControllerObserver.h"
#include <memory>
#include <unordered_map>
#include <vector>

class ButtonController {
public:
  ButtonController();

  void addObserver(std::shared_ptr<IButtonControllerObserver> observer);
  virtual void registerButton(int pinNumber, std::shared_ptr<IButton> button);

  void notifyObservers(int pinNumber);

private:
  std::vector<std::shared_ptr<IButtonControllerObserver>> _observers;
  std::unordered_map<int, std::shared_ptr<IButton>> _buttons;
};
