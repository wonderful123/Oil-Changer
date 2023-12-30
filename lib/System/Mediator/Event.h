#pragma once

#include <functional>
#include <string>

// Lower EventType value means higher priority
enum EventType {
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
  const void *sender; // Changed to void* to handle any type of sender
  EventType eventType;
  const EventData *data;

  EventInfo(const void *sender, EventType type, const EventData *data)
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