#pragma once

#include "IButtonControllerObserver.h"
#include <memory>
#include <unordered_map>
#include <vector>

class ButtonController {
public:
  ButtonController();

  void addObserver(std::shared_ptr<IButtonControllerObserver> observer);
  void registerButton(int buttonId,
                      int pin);  // Register a button and its associated pin
  static void handleInterrupt(); // Static method to handle interrupts

private:
  std::vector<std::shared_ptr<IButtonControllerObserver>> _observers;
  static std::unordered_map<int, int>
      _buttonPinMap; // Maps button IDs to pin numbers
  static std::unordered_map<int, ButtonController *>
      _instances; // Maps button IDs to controller instances

  void notifyObservers(int buttonId);
};
