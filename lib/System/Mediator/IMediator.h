#pragma once

#include <memory>

class IColleague;
#include "Event.h"

// IMediator Interface
class IMediator {
public:
  virtual ~IMediator() = default;

  // Registers a colleague for a specific event
  virtual void registerForEvent(std::shared_ptr<IColleague> colleague,
                                EventType eventType) = 0;

  // Deregisters a colleague from a specific event
  virtual void deregisterForEvent(std::shared_ptr<IColleague> colleague,
                                  EventType eventType) = 0;

  // Notify the Mediator about an event from a Colleague
  virtual void notify(const IColleague *sender, EventType eventType,
                      const EventData *data = nullptr) = 0;

  // Queue an event
  virtual void queueEvent(std::shared_ptr<IColleague> sender,
                          EventType eventType,
                          const EventData *data = nullptr) = 0;

  // Process all queued events
  virtual void processEvents() = 0;
};

// IColleague Interface
class IColleague {
protected:
  std::shared_ptr<IMediator> _mediator;

public:
  explicit IColleague(std::shared_ptr<IMediator> mediator = nullptr)
      : _mediator(std::move(mediator)) {}

  virtual ~IColleague() = default;

  void setMediator(std::shared_ptr<IMediator> mediator) {
    this->_mediator = mediator;
  }

  void sendEvent(EventType eventType, const EventData &eventData) {
    _mediator->notify(this, eventType, &eventData);
  }

  virtual void receiveEvent(EventType eventType, const EventData *eventData) {
    // Default empty implementation
    // Classes inheriting from IColleague are not forced to implement this
    // method.
  }
};
