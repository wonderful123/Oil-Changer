#pragma once

#include <string>

#include "IColleague.h"

class IDisplay : public IColleague {
 public:
  virtual ~IDisplay() {}

  // Display data on the screen
  virtual void displayData(const std::string &data) = 0;

  // Mediator pattern integration
  virtual void setMediator(std::shared_ptr<IMediator> mediator) = 0;
  virtual void receiveEvent(EventType eventType, const std::string &data) = 0;
};
