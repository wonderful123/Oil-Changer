#pragma once

#include "IButtonControllerObserver.h"
#include <memory>
#include <vector>

class ButtonController {
public:
  ButtonController();

  // Register an observer
  void addObserver(std::shared_ptr<IButtonControllerObserver> observer);

  // Method to be called when a button is pressed
  void onButtonPress(int buttonId);

  // Other methods for button management
  // ...

private:
  std::vector<std::shared_ptr<IButtonControllerObserver>> _observers;

  // Method to notify all observers about a button press
  void notifyObservers(int buttonId);
};
