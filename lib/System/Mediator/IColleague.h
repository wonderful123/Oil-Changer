#pragma once

#include "EventTypes.h"
#include "IMediator.h"

class IColleague {
protected:
  IMediator *mediator;

public:
  explicit IColleague(IMediator *mediator) : mediator(mediator) {}

  virtual ~IColleague() = default;

  // Call this to send events to the Mediator
  void sendEvent(EventType eventType) { mediator->notify(this, eventType); }

  // Implement this to handle events received from the Mediator
  virtual void receiveEvent(EventType eventType) = 0;
};