#pragma once

#include "EventType.h"

class IColleague;
class EventData;

class IMediator {
public:
  virtual ~IMediator() = default;

  virtual void registerColleague(IColleague *colleague) = 0;

  // Method to be called when a colleague has an event to share
  virtual void notify(const IColleague *sender, EventType eventType,
                      const EventData *data = nullptr) = 0;
};