// EventManager.h
#pragma once

#include "Event.h"
#include "IEventListener.h"
#include <algorithm>
#include <limits>
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
    notifyImpl(event, parameter);
  }

private:
  std::unordered_map<Event, std::vector<std::shared_ptr<IEventListener>>>
      _listeners;

  // Sentinent value used as a unique float value to check which onNotify method
  // to use
  static constexpr float SENTINEL_VALUE =
      std::numeric_limits<float>::infinity();

  void notifyImpl(Event event, Parameter parameter,
                  float value = SENTINEL_VALUE) {
    if (_listeners.count(event) > 0) {
      for (auto &listener : _listeners[event]) {
        if (value == SENTINEL_VALUE) {
          // Call the version without value
          listener->onNotify(event, parameter);
        } else {
          // Call the version with value
          listener->onNotify(event, parameter, value);
        }
      }
    }
  }
};