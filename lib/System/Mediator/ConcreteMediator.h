#pragma once

#include "Event.h"
#include <algorithm>
#include <functional>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <vector>

class ConcreteMediator : public IMediator {
  std::unordered_map<EventType, std::vector<IColleague *>> eventColleagues;
  std::priority_queue<EventInfo, std::vector<EventInfo>, EventComparator>
      eventQueue; // Uses a custom comparator for priority
  std::mutex mutex;
  bool processing = false; // Flag to handle reentrant calls to processEvents

public:
  // Event registration for specific events
  void registerForEvent(IColleague *colleague, EventType eventType) override {
    std::lock_guard<std::mutex> lock(mutex);
    eventColleagues[eventType].push_back(colleague);
  }

  void deregisterForEvent(IColleague *colleague, EventType eventType) override {
    std::lock_guard<std::mutex> lock(mutex);
    auto &colleagues = eventColleagues[eventType];
    colleagues.erase(
        std::remove(colleagues.begin(), colleagues.end(), colleague),
        colleagues.end());
  }

  // Queue an event with its data
  void queueEvent(const IColleague *sender, EventType eventType,
                  const EventData *data = nullptr) override {
    std::lock_guard<std::mutex> lock(mutex);
    eventQueue.push(EventInfo(sender, eventType, data));
  }

  // Process all queued events in order of priority
  void processEvents() override {
    if (processing)
      return; // Avoid reentrant calls
    processing = true;

    while (!eventQueue.empty()) {
      std::lock_guard<std::mutex> lock(mutex); // Lock for each event

      auto event = eventQueue.top();
      eventQueue.pop();

      // Notify only interested colleagues
      for (auto &colleague : eventColleagues[event.eventType]) {
        if (colleague != event.sender) {
          colleague->receiveEvent(event.eventType, event.data);
        }
      }
    }

    processing = false;
  }
};