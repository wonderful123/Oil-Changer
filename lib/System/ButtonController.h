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
  virtual void registerButton(const std::string &id,
                              std::shared_ptr<IButton> button);

  void notifyObservers(const std::string &id);

  void checkButtonStates();

private:
  std::vector<std::shared_ptr<IButtonControllerObserver>> _observers;
  std::unordered_map<std::string, std::shared_ptr<IButton>>
      _buttons; // Store buttons by ID
};
