#pragma once

#include "EventTypes.h"

class IColleague; // Forward declaration

class IMediator {
public:
  virtual ~IMediator() = default;

  // Method to be called when a colleague has an event to share
  virtual void notify(const IColleague *sender, EventType eventType) = 0;
};