#pragma once

#include <functional>
#include <string>

// Event Types
enum EventType {
  NOEVENT,
  HARDWARE_CONFIG_CHANGED,
  INTERACTION_SETTINGS_CHANGED,
  OIL_LEVEL_CHANGED,
  VOLTAGE_CHANGED,
  FLOW_RATE_CHANGED,
  BUTTON_PRESSED,
  BUTTON_AUTO_REPEAT,
  // Add more as needed
};

// Event Data
class EventData {
public:
  std::string id; // For identifying the source, like a button ID
  // Add other relevant event data fields
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
    // Comparison logic based on event type or other criteria
    // Example: lower EventType value means higher priority
    return a.eventType < b.eventType;
  }
};