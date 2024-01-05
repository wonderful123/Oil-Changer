#pragma once

#include "Event.h"
#include "IEventListener.h"
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

class EventManager {
public:
  void subscribe(std::shared_ptr<IEventListener> listener,
                 EventType eventType) {
    auto &eventObservers = _listeners[eventType];
    if (std::find(eventObservers.begin(), eventObservers.end(), listener) ==
        eventObservers.end()) {
      eventObservers.push_back(
          std::move(listener)); // Use std::move to avoid copying
    }
  }

  void unsubscribe(std::shared_ptr<IEventListener> listener,
                   EventType eventType) {
    auto &eventObservers = _listeners[eventType];
    eventObservers.erase(
        std::remove(eventObservers.begin(), eventObservers.end(), listener),
        eventObservers.end());
  }

  void notify(EventType eventType, EventData &eventData) {
    if (_listeners.count(eventType) > 0) {
      for (std::shared_ptr<IEventListener> listener : _listeners[eventType]) {
        listener->onNotify(eventType, eventData);
      }
    }
  }

  void notify(EventType eventType) {
    EventData eventData;          // Create a default EventData object
    notify(eventType, eventData); // Call the existing notify method
  }

private:
  std::unordered_map<EventType, std::vector<std::shared_ptr<IEventListener>>>
      _listeners;
};
