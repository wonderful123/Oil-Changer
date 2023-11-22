#pragma once
#include "Core/IObserver.h"

class IButtonControllerObserver : public IObserver {
public:
  virtual ~IButtonControllerObserver() = default;

  // Callback for button press
  virtual void onButtonPress(const std::string &id) = 0;
};

