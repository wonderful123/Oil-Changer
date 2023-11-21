#pragma once

#include "IButtonControllerObserver.h"
#include <memory>
#include <vector>

class ButtonController {
public:
  ButtonController();

  void addObserver(std::shared_ptr<IButtonControllerObserver> observer);
  void registerButton(int buttonId, std::shared_ptr<ButtonBase> button);

  void notifyObservers(int buttonId);

private:
  std::vector<std::shared_ptr<IButtonControllerObserver>> _observers;
  std::unordered_map<int, std::shared_ptr<ButtonBase>> _buttons;
};
