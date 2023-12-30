#pragma once

#include "IMediator.h"
#include <functional>
#include <string>
#include <memory>

// Lower EventType value means higher priority
enum class EventType {
  NOEVENT,
  BUTTON_PRESSED,
  OIL_CHANGE_TRACKER_UPDATE,
  HARDWARE_CONFIG_CHANGED,
  INTERACTION_SETTINGS_CHANGED,
  // Add more as needed
};

// Event Data
class EventData {
public:
  std::string id; // For identifying the source, like a button ID
  int value;
};

// Event Information
struct EventInfo {
  std::weak_ptr<IColleague> sender;
  EventType eventType = EventType::NOEVENT;
  const EventData *data = nullptr;

  EventInfo() = default;
  EventInfo(std::weak_ptr<IColleague> sender, EventType type,
            const EventData *data)
      : sender(sender), eventType(type), data(data) {}
};

// Custom comparator for the priority queue (define how to compare events)
struct EventComparator {
  bool operator()(const EventInfo &a, const EventInfo &b) {
    // Comparison logic based on event type
    // Lower EventType value means higher priority
    return a.eventType < b.eventType;
  }
};