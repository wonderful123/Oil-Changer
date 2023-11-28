#pragma once

#include "IButton.h"
#include "IButtonControllerObserver.h"
#include <memory>
#include <unordered_map>
#include <vector>

class ButtonController {
public:
  void addObserver(std::shared_ptr<IButtonControllerObserver> observer);
  void registerButton(const std::string &id, std::shared_ptr<IButton> button);
  void processButtonStates();
  std::shared_ptr<IButton> getButtonById(const std::string &id) const;

private:
  void notifyObservers(const std::string &id);
  std::vector<std::shared_ptr<IButtonControllerObserver>> _observers;
  std::unordered_map<std::string, std::shared_ptr<IButton>> _buttons;
};
