// EventManager.h
#pragma once

#include "Event.h"
#include "IEventListener.h"
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

class EventManager {
public:
  void subscribe(std::shared_ptr<IEventListener> listener, Event eventType) {
    auto &eventObservers = _listeners[eventType];
    if (std::find(eventObservers.begin(), eventObservers.end(), listener) ==
        eventObservers.end()) {
      eventObservers.push_back(
          std::move(listener)); // Use std::move to avoid copying
    }
  }

  void unsubscribe(std::shared_ptr<IEventListener> listener, Event eventType) {
    auto &eventObservers = _listeners[eventType];
    eventObservers.erase(
        std::remove(eventObservers.begin(), eventObservers.end(), listener),
        eventObservers.end());
  }

  // Notify method for events that require a value
  void notify(Event event, Parameter parameter, float value) {
    notifyImpl(event, parameter, value);
  }

  // Overloaded notify method for events that don't require a value
  void notify(Event event, Parameter parameter) {
    notifyImpl(event, parameter, 0.0f); // Using a default value
  }

private:
  std::unordered_map<Event, std::vector<std::shared_ptr<IEventListener>>>
      _listeners;

  void notifyImpl(Event event, Parameter parameter, float value) {
    if (_listeners.count(event) > 0) {
      for (auto &listener : _listeners[event]) {
        listener->onNotify(event, parameter, value);
      }
    }
  }
};
