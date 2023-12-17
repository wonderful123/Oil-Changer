#pragma once

#include "Error.h"
#include "EventData.h"
#include "EventType.h"
#include "IMediator.h"
#include <memory>

class IColleague {
protected:
  std::shared_ptr<IMediator> mediator;

public:
  explicit IColleague(std::shared_ptr<IMediator> mediator)
      : mediator(mediator) {}

  virtual ~IColleague() = default;

  void sendEvent(EventType eventType, const EventData &eventData) {
    mediator->notify(this, eventType, &eventData);
  }

  // Implement this to handle events received from the Mediator
  virtual void receiveEvent(EventType eventType,
                            const EventData *eventData) = 0;
};