#pragma once

#include "Event.h"
#include <memory>

class IColleague;

// IMediator Interface
class IMediator {
public:
  virtual ~IMediator() = default;

  // Registers a colleague for a specific event
  virtual void registerForEvent(IColleague *colleague, EventType eventType) = 0;

  // Deregisters a colleague from a specific event
  virtual void deregisterForEvent(IColleague *colleague,
                                  EventType eventType) = 0;

  // Notify the Mediator about an event from a Colleague
  virtual void notify(const IColleague *sender, EventType eventType,
                      const EventData *data = nullptr) = 0;

  // Queue an event
  virtual void queueEvent(const IColleague *sender, EventType eventType,
                          const EventData *data = nullptr) = 0;

  // Process all queued events
  virtual void processEvents() = 0;
};

// IColleague Interface
class IColleague {
protected:
  std::shared_ptr<IMediator> _mediator;

public:
  explicit IColleague(std::shared_ptr<IMediator> mediator)
      : _mediator(std::move(mediator)) {}

  virtual ~IColleague() = default;

  void sendEvent(EventType eventType, const EventData &eventData) {
    _mediator->notify(this, eventType, &eventData);
  }

  virtual void receiveEvent(EventType eventType,
                            const EventData *eventData) = 0;
};
