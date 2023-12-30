#pragma once

#include "IMediator.h"
#include <algorithm>
#include <functional>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <vector>

class ConcreteMediator : public IMediator {
  std::unordered_map<EventType, std::vector<std::weak_ptr<IColleague>>>
      eventColleagues;
  std::priority_queue<EventInfo, std::vector<EventInfo>, EventComparator>
      eventQueue; // Uses a custom comparator for priority
  std::mutex mutex;
  bool processing = false; // Flag to handle reentrant calls to processEvents

public:
  // Event registration for specific events
  void registerForEvent(std::shared_ptr<IColleague> colleague,
                        EventType eventType) {
    std::lock_guard<std::mutex> lock(mutex);
    eventColleagues[eventType].push_back(colleague);
  }

  void deregisterForEvent(std::shared_ptr<IColleague> colleague,
                          EventType eventType) {
    std::lock_guard<std::mutex> lock(mutex);
    auto &colleagues = eventColleagues[eventType];
    colleagues.erase(
        std::remove_if(colleagues.begin(), colleagues.end(),
                       [&colleague](const std::weak_ptr<IColleague> &ptr) {
                         return !ptr.owner_before(colleague) &&
                                !colleague.owner_before(ptr);
                       }),
        colleagues.end());
  }

  void notify(const IColleague *sender, EventType eventType,
              const EventData *data = nullptr) override {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = eventColleagues.find(eventType);
    if (it != eventColleagues.end()) {
      for (auto &weakColleague : it->second) {
        if (auto colleague = weakColleague.lock()) {
          if (colleague.get() != sender) {
            colleague->receiveEvent(eventType, data);
          }
        }
      }
    }
  }

  // Queue an event with its data
  void queueEvent(std::shared_ptr<IColleague> sender, EventType eventType,
                  const EventData *data = nullptr) override {
    std::lock_guard<std::mutex> lock(mutex);
    eventQueue.push(EventInfo(sender, eventType, data));
  }

  // Process all queued events in order of priority
  void processEvents() override {
    // Early exit if already processing
    {
      std::lock_guard<std::mutex> lock(mutex);
      if (processing)
        return;
      processing = true;
    }

    while (true) {
      EventInfo event;
      {
        std::lock_guard<std::mutex> lock(mutex);
        if (eventQueue.empty()) {
          processing = false; // Reset the flag when done processing
          break;              // Exit the loop if no events are left to process
        }

        event = eventQueue.top();
        eventQueue.pop();
      } // Release lock immediately after accessing shared resources

      // Notify only interested colleagues
      auto it = eventColleagues.find(event.eventType);
      if (it != eventColleagues.end()) {
        for (auto &weakColleague : it->second) {
          if (auto colleague = weakColleague.lock()) {
            // Ensure not to send the event back to the sender
            if (colleague.get() != event.sender.lock().get()) {
              colleague->receiveEvent(event.eventType, event.data);
            }
          }
        }
      }
    }
  }
  
};